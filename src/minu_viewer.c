#include "minu_viewer.h"
#include "minu.h"
#include "minu_base.h"
#include "minu_disp.h"
#include "minu_item.h"
#include "minu_vector.h"
#include MINU_MEM_CUSTOM_INCLUDE

#include <stdint.h>
#include <assert.h>
#include <stdlib.h>

#include "esp_log.h"

typedef enum
{
    STATUS_TRAN,
    STATUS_IGNORED,
} state_t;

typedef enum
{
    /* for triggering the entry action in a state */
    EVENT_STATE_ENTRY = MINU_EVENT_MAX,
    /* for triggering the exit action from a state */
    EVENT_STATE_EXIT,
} reserved_event;

typedef struct minu_viewer_t minu_viewer_t;
typedef state_t (*stateHandler)(minu_viewer_t *me, minu_event_id_t e);

typedef struct
{
    uint8_t in;
    uint8_t out;
    uint8_t buf[MINU_EVENT_QUEUE_SIZE]; /* @ref minu_event_id_t */
} minu_event_t;

struct minu_viewer_t
{
    stateHandler state;
    minu_t *act_menu;
    minu_event_t evt;
    minu_base_t selector;
    struct
    {
        uint16_t x;
        uint16_t y;
    } offset;
};

#define TRAN_STATE(target) (me->state = (target), STATUS_TRAN)

static state_t viewer_handleMenu(minu_viewer_t *me, minu_event_id_t e)
{
    minu_t *menu = me->act_menu;
    state_t status = STATUS_IGNORED;

    if (VECTOR_SIZE(menu->items) == 0)
        return status;

    switch ((uint8_t)e)
    {
        case EVENT_STATE_ENTRY:
            break;
        case EVENT_STATE_EXIT:
            break;

        case MINU_EVENT_UP:
            minu_goPrevious(menu);
            break;
        case MINU_EVENT_DOWN:
            minu_goNext(menu);
            break;
        case MINU_EVENT_ENTER:
            minu_goIn(&menu);
            break;
        case MINU_EVENT_QUIT:
            minu_goOut(&menu);
            break;
        case MINU_EVENT_DELETE:
            minu_deleteItem(menu);
            break;
        default:
            status = STATUS_IGNORED;
            break;
    }

    return status;
}

static state_t viewer_handleItem(minu_viewer_t *me, minu_event_id_t e)
{
    state_t status = STATUS_IGNORED;

    switch ((uint8_t)e)
    {
        case EVENT_STATE_ENTRY:
            break;
        case EVENT_STATE_EXIT:
            break;
        default:
            break;
    }

    return status;
}

/**
 * @brief get the event from the message queue
 * @param me handler object
 * @param evt @ref minu_event_id_t
 */
void minu_viewer_event_post_to(minu_viewer_handle_t me, minu_event_id_t e)
{
    minu_event_t *evt = &me->evt;
    uint8_t in = evt->in & (MINU_EVENT_QUEUE_SIZE - 1);

    evt->buf[in] = e;
    evt->in++;
}

/**
 * @brief let viewer focus on the specific menu
 *
 * @param menu  menu to be focused
 */
minu_viewer_handle_t minu_viewer_create(minu_t *menu)
{
    minu_viewer_handle_t ret;
    minu_base_t first_attr = minu_base_getAttr(&VECTOR_BEGIN(menu->items));

    ret = MINU_MEM_CUSTOM_ALLOC(sizeof(minu_viewer_t));
    assert(ret != NULL);

    ret->act_menu = menu;
    ret->state = viewer_handleMenu;
    ret->state(ret, EVENT_STATE_ENTRY);
    ret->offset.x = ret->offset.y = 0;

    /* refresh the screen */
    minu_viewer_event_post_to(ret, MINU_EVENT_REFRESH);

    /* set selector's attribute */
    minu_base_setAttrWith(&ret->selector, &first_attr);

    return ret;
}

/**
 * @brief get the event from the message queue
 * @note  this function can be replaced with rtos queue
 * @return  @ref minu_event_id_t
 */
static uint8_t _get_event(minu_event_t *const me)
{
    uint8_t ret;
    uint8_t out = me->out & (MINU_EVENT_QUEUE_SIZE - 1);

    if (me->in == me->out)
        return MINU_EVENT_NONE;

    ret = me->buf[out];
    me->out++;

    return ret;
}

static void viewer_updateOffsetWindow(minu_viewer_t *me)
{
    int new_offset_x, new_offset_y;
    int16_t item_edge_x, item_edge_y;
    minu_base_t item_attr, menu_attr;

    minu_t *menu = me->act_menu;
    new_offset_x = me->offset.x;
    new_offset_y = me->offset.y;
    menu_attr = minu_base_getAttr(menu);
    item_attr = minu_base_getAttr(&VECTOR_AT(menu->items, menu->item_index));

    item_edge_x = item_attr.x + item_attr.w;
    item_edge_y = item_attr.y + item_attr.h;

    if (item_edge_y > (new_offset_y + menu_attr.h))
        new_offset_y = item_edge_y - menu_attr.h;
    else if (item_attr.y < new_offset_y)
        new_offset_y = item_attr.y;

    me->offset.x = new_offset_x;
    me->offset.y = new_offset_y;
}

static void viewer_updateSelector(minu_viewer_t *me)
{
    minu_base_t tar_sel;
    minu_base_t item_attr;
    minu_t *menu = me->act_menu;

    if (VECTOR_SIZE(menu->items) == 0)
        return;

    item_attr = minu_base_getAttr(&VECTOR_AT(menu->items, menu->item_index));

    tar_sel.y = item_attr.y - me->offset.y;
    tar_sel.x = item_attr.x;
    tar_sel.w = item_attr.w;
    tar_sel.h = item_attr.h;
    minu_base_setAttrWith(&me->selector, &tar_sel);

    ESP_LOGI("",
             "select_y=%d, offset_y=%d, i=%d\n",
             me->selector.y,
             me->offset.y,
             menu->item_index);
}

static void draw_selector(minu_viewer_t *me)
{
    minu_disp_fillRectInDiff(me->selector.x,
                             me->selector.y + me->act_menu->layout.border_gap,
                             me->selector.w +
                                 me->act_menu->layout.border_gap * 2,
                             me->selector.h);
}

static void draw_progress_bar(minu_t *menu)
{
    minu_base_t menu_attr = minu_base_getAttr(menu);
    int16_t bar_offseted_x = menu_attr.x + menu_attr.w;

    // draw bar top width
    minu_disp_drawHLine(bar_offseted_x - menu->layout.bar_width,
                        menu_attr.y,
                        menu->layout.bar_width);

    // draw bar bottom width
    minu_disp_drawHLine(bar_offseted_x - menu->layout.bar_width,
                        menu_attr.y + menu_attr.h - 1,
                        menu->layout.bar_width);

    // draw bar height
    minu_disp_drawVLine(bar_offseted_x - (menu->layout.bar_width / 2 + 1),
                        menu_attr.y,
                        menu_attr.h);

    // items count from 0
    uint8_t item_size = VECTOR_SIZE(menu->items);
    int16_t h_per_progress = item_size ? menu_attr.h / item_size : 0;
    int16_t progress = (menu->item_index + 1) != item_size
                           ? (menu->item_index + 1) * h_per_progress
                           : menu_attr.h;

    minu_disp_fillRect(bar_offseted_x - menu->layout.bar_width,
                       menu_attr.y,
                       menu->layout.bar_width,
                       progress);
}

static void viewer_render(minu_viewer_t *me)
{
    minu_t *menu = me->act_menu;
    minu_base_t menu_attr = minu_base_getAttr(menu);

    viewer_updateOffsetWindow(me);
    viewer_updateSelector(me);

    /* draw all the items in the screen */
    for (uint8_t i = 0; i < VECTOR_SIZE(menu->items); i++)
    {
        minu_base_t item_attr;
        uint16_t item_tar_x, item_tar_y;

        item_attr = minu_base_getAttr(&VECTOR_AT(menu->items, i));
        item_tar_x = item_attr.x + menu->layout.border_gap;
        item_tar_y = item_attr.y + menu->layout.border_gap - me->offset.y;

        /* check if the item is in the menu's area */
        if (item_tar_y > menu_attr.y &&
            item_tar_y <= (menu_attr.y + menu_attr.h))
        {
            const char *item_name = VECTOR_AT(menu->items, i).name;
            minu_disp_drawStr(item_tar_x, item_tar_y, item_name);
        }
    }

    /* draw selector */
    draw_selector(me);

    // draw progress bar
    draw_progress_bar(menu);

    // flush buffer to the screen
    minu_disp_flush();
}

static void state_dispatch(minu_viewer_t *me, uint16_t e)
{
    assert(me->state != NULL);

    state_t status;
    stateHandler prev_state = me->state;

    status = me->state(me, e);

    if (status == STATUS_TRAN)
    {
        prev_state(me, EVENT_STATE_EXIT);
        me->state(me, EVENT_STATE_ENTRY);
    }
}

void minu_viewer_update(minu_viewer_handle_t me)
{
    assert(me->act_menu != NULL);

    uint8_t evt = _get_event(&me->evt);
    if (evt == MINU_EVENT_NONE)
        return;

    /* for event dispatch */
    state_dispatch(me, evt);

    /* for graph */
    viewer_render(me);
}
