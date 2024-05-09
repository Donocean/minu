#include "minu_monitor.h"
#include "minu_disp.h"
#include "minu_vector.h"
#include <stdint.h>
#include <assert.h>

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

static void render(minu_monitor_t *const me)
{
    minu_t            *menu  = me->act_menu;
    minu_vector_itme_ *items = &menu->items;

    for (uint8_t i = 0; i < PVECTOR_SIZE(items); i++)
    {
        uint8_t temp = minu_disp_getFontHeight(me->disp) * i + y;

        minu_disp_drawStr((me->disp, menu->super.x, temp, PVECTOR_AT(items, i).name);
    }
}

/**
 * @brief let monitor focus on the specific menu
 *
 * @param me    monitor object
 * @param menu  menu to be focused
 */
void minu_monitor_focusOn(minu_monitor_t *const me, minu_t *menu)
{
    me->act_menu = menu;
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
