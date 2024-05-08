#include "minu_monitor.h"
#include <stdint.h>
#include <assert.h>

/**
 * @brief get the event from the message queue
 *
 * @param me handler object
 *
 * @return  @ref minu_event_id_t
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
 *
 * @param me minu_event_t object
 *
 * @return  @ref minu_event_id_t
 */
static uint8_t minu_event_get(minu_event_t *const me)
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
    minu_ops_t *ops = &me->ops;

    if (ops->flush)
        ops->flush();
}

void minu_monitor_init(minu_monitor_t *const me, minu_t *menu, minu_ops_t *ops)
{
    me->act_menu = menu;
    me->ops      = *ops;
}

void minu_monitor_update(minu_monitor_t *const me)
{
    assert(me->act_menu != NULL);

    /* get the event. this function can be replaced with rtos queue  */
    uint8_t evt = minu_event_get(&me->evt);

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
