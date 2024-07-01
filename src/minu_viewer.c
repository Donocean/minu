#include "minu_viewer.h"
#include "minu.h"
#include "minu_base.h"
#include "minu_disp.h"
#include "minu_item.h"
#include "minu_vector.h"
#include <stdint.h>
#include <assert.h>
#include <stdlib.h>

#include "esp_log.h"

/**
 * @brief get the event from the message queue
 * @param me handler object
 * @param val @ref minu_event_id_t minu_event_id_t
 */
void minu_viewer_event_post_to(minu_viewer_t *const me, uint8_t val)
{
    minu_event_t *evt = &me->evt;
    uint8_t in = evt->in & (MINU_EVENT_QUEUE_SIZE - 1);

    evt->buf[in] = val;
    evt->in++;
}

/**
 * @brief let viewer focus on the specific menu
 *
 * @param me    viewer object
 * @param menu  menu to be focused
 */
void minu_viewer_focusOn(minu_viewer_t *const me, minu_t *menu)
{
    me->act_menu = menu;
    menu->selector.w = VECTOR_AT(menu->items, 0).super.w;
    menu->selector.h = VECTOR_AT(menu->items, 0).super.h;

    /* refresh the screen */
    minu_viewer_event_post_to(me, MINU_EVENT_REFRESH);
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
    if (last_index == me->item_index)
        return;

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

static void viewer_render(minu_viewer_t *const me)
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

void minu_dispatch(minu_viewer_t *const me, uint16_t e)
{
}

void minu_viewer_update(minu_viewer_t *const me)
{
    assert(me->act_menu != NULL);

    uint8_t evt = _get_event(&me->evt);
    if (evt == MINU_EVENT_NONE)
        return;

    switch (evt)
    {
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
            break;
    }

    viewer_render(me);
}
