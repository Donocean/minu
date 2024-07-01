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
};

#define TRAN_STATE(target) (me->state = (target), STATUS_TRAN)

static state_t viewer_handleMenu(minu_viewer_t *me, minu_event_id_t e);
static state_t viewer_handleItem(minu_viewer_t *me, minu_event_id_t e);


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

    ret = MINU_MEM_CUSTOM_ALLOC(sizeof(minu_viewer_t));
    assert(ret != NULL);

    menu->selector.w = VECTOR_AT(menu->items, 0).super.w;
    menu->selector.h = VECTOR_AT(menu->items, 0).super.h;

    ret->act_menu = menu;
    ret->state = viewer_handleMenu;
    ret->state(ret, EVENT_STATE_ENTRY);

    /* refresh the screen */
    minu_viewer_event_post_to(ret, MINU_EVENT_REFRESH);

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

static void update_selector(minu_t *me)
{
    static uint8_t last_index = 0;

    minu_base_t tar_sel;
    int16_t font_h, offset_y;
    minu_base_t item_attr, menu_attr;
    minu_layout_t *layout = &me->layout;

    /* didn't move the index of the selector */
    /* if (last_index == me->item_index) */
    /*     return; */

    font_h = minu_disp_getFontHeight();
    menu_attr = minu_base_getAttr(me);
    item_attr = minu_base_getAttr(&VECTOR_AT(me->items, me->item_index));

    /* y end coordinates of the current selected item */
    offset_y = item_attr.y + item_attr.h + me->movingOffset;

    /* if the position of selected item is within the area of the menu, make
     * selector jump to the item's position */
    if (offset_y > menu_attr.y && offset_y <= (menu_attr.y + menu_attr.h))
        tar_sel.y = item_attr.y + me->movingOffset;
    else
    {
        /* selector should remain in place. just making item move */
        tar_sel.y = me->selector.y;
        if (me->item_index - last_index > 0)
            me->movingOffset -= font_h + layout->u.item_gap; /* move down */
        else
            me->movingOffset += font_h + layout->u.item_gap; /* move up */

        if (me->is_loopItem)
        {
            if (me->item_index == 0)
            {
                /* make selector back to the first item position */
                tar_sel.y = item_attr.y;
                me->movingOffset = 0;
            }
            else if (me->item_index == (VECTOR_SIZE(me->items) - 1))
            {
                /* make selector back to the final item */
                uint8_t end = menu_attr.h / (font_h + layout->u.item_gap) - 1;
                tar_sel.y = menu_attr.y + end * (font_h + layout->u.item_gap);
                me->movingOffset = -1 * (item_attr.y - tar_sel.y);
            }
        }
    }
    tar_sel.x = item_attr.x;
    tar_sel.w = item_attr.w;
    tar_sel.h = item_attr.h;
    me->selector = tar_sel;

    last_index = me->item_index;

    ESP_LOGI("test",
             "r_y = %d s_y = %d m_y =%d\n",
             offset_y,
             me->selector.y,
             me->movingOffset);
}

static void draw_selector(minu_t *menu)
{
    minu_disp_fillRectInDiff(menu->selector.x,
                             menu->selector.y + menu->layout.border_gap,
                             menu->selector.w + menu->layout.border_gap * 2,
                             menu->selector.h);
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
    uint8_t item_size = VECTOR_SIZE(menu->items) - 1;
    int16_t h_per_progress = menu_attr.h / item_size;
    int16_t progress = menu->item_index != item_size
                           ? menu->item_index * h_per_progress
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

    update_selector(menu);

    /* draw all the items in the screen */
    for (uint8_t i = 0; i < VECTOR_SIZE(menu->items); i++)
    {
        minu_base_t item_attr;
        uint16_t item_tar_x, item_tar_y;

        item_attr = minu_base_getAttr(&VECTOR_AT(menu->items, i));
        item_tar_x = item_attr.x + menu->layout.border_gap;
        item_tar_y = item_attr.y + menu->layout.border_gap + menu->movingOffset;

        /* check if the item is in the menu's area */
        if (item_tar_y > menu_attr.y &&
            item_tar_y <= (menu_attr.y + menu_attr.h))
        {
            const char *item_name = VECTOR_AT(menu->items, i).name;
            minu_disp_drawStr(item_tar_x, item_tar_y, item_name);
        }
    }

    /* draw selector */
    draw_selector(menu);

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

static state_t viewer_handleMenu(minu_viewer_t *me, minu_event_id_t e)
{
    state_t status = STATUS_IGNORED;

    switch ((uint8_t)e)
    {
        case EVENT_STATE_ENTRY:
            break;
        case EVENT_STATE_EXIT:
            break;

        case MINU_EVENT_UP:
            minu_goPrevious(me->act_menu);
            break;
        case MINU_EVENT_DOWN:
            minu_goNext(me->act_menu);
            break;
        case MINU_EVENT_ENTER:
            minu_goIn(&me->act_menu);
            break;
        case MINU_EVENT_QUIT:
            minu_goOut(&me->act_menu);
            break;
        case MINU_EVENT_DELETE:
            minu_deleteItem(me->act_menu);
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

