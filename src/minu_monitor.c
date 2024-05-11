#include "minu_monitor.h"
#include "minu_base.h"
#include "minu_disp.h"
#include "minu_item.h"
#include "minu_vector.h"
#include <stdint.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * @brief get the event from the message queue
 * @param me handler object
 * @param val @ref minu_event_id_t minu_event_id_t
 */
void minu_monitor_event_post_to(minu_monitor_t *const me, uint8_t val)
{
    minu_event_t *evt = &me->evt;
    uint8_t       in  = evt->in & (MINU_EVENT_QUEUE_SIZE - 1);

    evt->buf[in] = val;
    evt->in++;
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

/**
 * @brief let monitor focus on the specific menu
 *
 * @param me    monitor object
 * @param menu  menu to be focused
 */
void minu_monitor_focusOn(minu_monitor_t *const me, minu_t *menu)
{
    me->act_menu     = menu;
    menu->selector.w = VECTOR_AT(menu->items, 0).super.w;
    menu->selector.h = VECTOR_AT(menu->items, 0).super.h;

    /* refresh the screen */
    minu_monitor_event_post_to(me, MINU_EVENT_REFRESH);
}

static void render(minu_monitor_t *const me)
{
    minu_t            *menu      = me->act_menu;
    minu_vector_itme_ *vec_items = &menu->items;
    minu_layout_t     *layout    = &menu->layout;

    for (uint8_t i = 0; i < PVECTOR_SIZE(vec_items); i++)
    {
        minu_base_t item_attr = minu_base_getAttribute((minu_base_t *)&PVECTOR_AT(vec_items, i));

        minu_disp_drawStr(item_attr.x + layout->border_gap,
                          item_attr.y + layout->border_gap + menu->movingOffset,
                          PVECTOR_AT(vec_items, i).name);
    }

    minu_disp_fillRectInDiff(menu->selector.x,
                             menu->selector.y + layout->border_gap,
                             menu->selector.w + layout->border_gap * 2,
                             menu->selector.h);
    minu_disp_flush();
}

void minu_monitor_update(minu_monitor_t *const me)
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
        case MINU_EVENT_BACK:
            minu_goIn(&me->act_menu);
            break;
        default:
            break;
    }

    render(me);
}
