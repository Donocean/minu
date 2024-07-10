#include "minu_viewer.h"
#include "minu.h"
#include "minu_base.h"
#include "minu_disp.h"
#include "minu_item.h"
#include "minu_vector.h"
#include <stdbool.h>
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

enum reserved_event
{
    /* for triggering the entry action in a state */
    EVENT_STATE_ENTRY = MINU_EVENT_MAX,
    /* for triggering the exit action from a state */
    EVENT_STATE_EXIT,
};

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
    minu_handle_t act_menu;

    minu_event_t evt;
    minu_base_t selector;
    minu_pos_t offset;
};


#define TRAN_STATE(target) (me->state = (target), STATUS_TRAN)

static void _refresh_state(minu_viewer_t *me);
static state_t _handleMenu(minu_viewer_t *me, minu_event_id_t e);

static state_t _handleItem(minu_viewer_t *me, minu_event_id_t e)
{
    state_t status = STATUS_IGNORED;
    minu_item_t *item  = minu_getSelectedItem(me->act_menu);

    switch ((uint8_t)e)
    {
        case EVENT_STATE_ENTRY:
            break;
        case EVENT_STATE_EXIT:
            break;
        case MINU_EVENT_QUIT:
            status = TRAN_STATE(_handleMenu);
            break;
        default:
            item->cb(item->u.user_data, e);
            break;
    }

    return status;
}

static state_t _handleMenu(minu_viewer_t *me, minu_event_id_t e)
{
    state_t status = STATUS_IGNORED;

    if (!minu_getSize(me->act_menu))
        return status;

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
            if (minu_goIn(&me->act_menu, e))
            {
                minu_item_t *item = minu_getSelectedItem(me->act_menu);

                if (item->type == MINU_ITEM_TYPE_SUBMENU)
                    _refresh_state(me);
                else
                    status = TRAN_STATE(&_handleItem);
            }
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
minu_viewer_handle_t minu_viewer_create(minu_handle_t menu)
{
    minu_viewer_handle_t ret;
    const minu_base_t *first_attr, *menu_attr;

    ret = MINU_MEM_CUSTOM_ALLOC(sizeof(minu_viewer_t));
    assert(ret != NULL);

    ret->offset.x = 0;
    ret->offset.y = 0;
    ret->act_menu = menu;
    ret->state = _handleMenu;
    ret->state(ret, EVENT_STATE_ENTRY);

    /* set selector's attribute */
    first_attr = minu_base_getAttr(minu_getSelectedItem(menu));
    minu_base_setAttrWith(&ret->selector, (void *)first_attr);

    /* restricts all graphics output to the specified range */
    menu_attr = minu_base_getAttr(menu);
    minu_disp_setWindow(menu_attr->x, menu_attr->y, menu_attr->w, menu_attr->h);

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

static void _update_offsetWindow(minu_viewer_t *me)
{
    minu_pos_t offset = {0};
    int16_t item_edge_x, item_edge_y;
    const minu_base_t *item_attr, *menu_attr;
    const minu_layout_t *layout = minu_getLayout(me->act_menu);

    if (!minu_getSize(me->act_menu))
        return;

    offset.x = me->offset.x;
    offset.y = me->offset.y;
    menu_attr = minu_base_getAttr(me->act_menu);
    item_attr = minu_base_getAttr(minu_getSelectedItem(me->act_menu));

    item_edge_x = item_attr->x + item_attr->w;
    item_edge_y = item_attr->y + item_attr->h;

    if (item_edge_x > (offset.x + menu_attr->w + +menu_attr->x))
        offset.x = item_edge_x - menu_attr->w - menu_attr->x;
    else if ((item_attr->x - menu_attr->x) < offset.x)
        offset.x = item_attr->x - menu_attr->x - layout->border_gap;

    /* `menu_attr->y` here means we need align the coordinate to the position of
     * origin of the screen. e.g. the menu is at (x:20, y:20, w: 128-20, h:
     * 64-20) instead of origin(x:0, y:0, w: 128, h: 64) */
    if (item_edge_y > (offset.y + menu_attr->h + menu_attr->y))
        offset.y = item_edge_y - menu_attr->h - menu_attr->y;
    else if ((item_attr->y - menu_attr->y) < offset.y)
        offset.y = item_attr->y - menu_attr->y - layout->border_gap;

    minu_pos_set(&me->offset, offset.x, offset.y);
}

static void _update_selector(minu_viewer_t *me)
{
    minu_base_t tar_sel = {0};
    const minu_base_t *item_attr = NULL;
    const minu_layout_t *layout = minu_getLayout(me->act_menu);

    if (!minu_getSize(me->act_menu))
        return;

    item_attr = minu_base_getAttr(minu_getSelectedItem(me->act_menu));

    tar_sel.w = item_attr->w + layout->border_gap * 2;
    tar_sel.h = item_attr->h;
    tar_sel.y = item_attr->y - me->offset.y;
    tar_sel.x = item_attr->x - me->offset.x - layout->border_gap;
    minu_base_setAttrWith(&me->selector, &tar_sel);

    ESP_LOGI("",
             "select_x=%d, offset_x=%d, item_x=%d",
             me->selector.x,
             me->offset.x,
             item_attr->x);
    ESP_LOGI("",
             "select_y=%d, offset_y=%d, item_y=%d\n",
             me->selector.y,
             me->offset.y,
             item_attr->y);
}

static void _draw_selector(minu_viewer_t *me)
{
    minu_disp_fillRectInDiff(me->selector.x,
                             me->selector.y,
                             me->selector.w,
                             me->selector.h);
}

static void _draw_progress_bar(minu_handle_t menu)
{
    const minu_base_t *menu_attr = minu_base_getAttr(menu);
    int16_t bar_offseted_x = menu_attr->x + menu_attr->w;
    minu_layout_t *layout = minu_getLayout(menu);

    // draw bar top width
    minu_disp_drawHLine(bar_offseted_x - layout->bar_width,
                        menu_attr->y,
                        layout->bar_width);

    // draw bar bottom width
    minu_disp_drawHLine(bar_offseted_x - layout->bar_width,
                        menu_attr->y + menu_attr->h - 1,
                        layout->bar_width);

    // draw bar height
    minu_disp_drawVLine(bar_offseted_x - (layout->bar_width / 2 + 1),
                        menu_attr->y,
                        menu_attr->h);

    // items count from 0
    uint8_t item_size = minu_getSize(menu);
    int16_t h_per_progress = item_size ? menu_attr->h / item_size : 0;
    int16_t progress = (minu_getIndex(menu) + 1) != item_size
                           ? (minu_getIndex(menu) + 1) * h_per_progress
                           : menu_attr->h;

    minu_disp_fillRect(bar_offseted_x - layout->bar_width,
                       menu_attr->y,
                       layout->bar_width,
                       progress);
}

static void _draw_items(minu_viewer_t *me)
{
    minu_item_t *item = NULL;
    minu_vector_itme_t *vec = minu_getItems(me->act_menu);

    /* draw all the items in the screen */
    while (minu_vector_iter_next(vec, &item))
    {
        minu_pos_t target;
        const minu_base_t *item_attr;

        item_attr = minu_base_getAttr(item);
        target.x = item_attr->x - me->offset.x;
        target.y = item_attr->y - me->offset.y;

        minu_disp_drawStr(target.x, target.y, item->name);

        /* draw appendage */
        minu_item_drawAppendage(item, );
    }
}

static void _render(minu_viewer_t *me)
{
    _update_offsetWindow(me);
    _update_selector(me);

    /* draw items */
    _draw_items(me);

    /* draw selector */
    _draw_selector(me);

    // draw progress bar
    _draw_progress_bar(me->act_menu);

    // flush buffer to the screen
    minu_disp_flush();
}

static void _refresh_state(minu_viewer_t *me)
{
    me->state(me, EVENT_STATE_EXIT);
    me->state(me, EVENT_STATE_ENTRY);
}

static void _state_dispatch(minu_viewer_t *me, uint16_t e)
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
    _state_dispatch(me, evt);

    /* for graph */
    _render(me);
}
