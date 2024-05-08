#include "minu_anim.h"
#include "minu_conf.h"
#include <stdint.h>

static uint32_t         sys_time = 0;
static volatile uint8_t tick_irq_flag;

/**
 * @brief You have to call this function periodically
 * @param tick_period the call period of this function in milliseconds
 */
void minu_tick_inc(uint32_t tick_period)
{
    tick_irq_flag  = 0;
    sys_time      += tick_period;
}

/**
 * @brief Get the elapsed milliseconds since start up
 * @return the elapsed milliseconds
 */
uint32_t minu_tick_get(void)
{
#ifndef MENU_TICK_CUSTOM

    /*If `lv_tick_inc` is called from an interrupt while `sys_time` is read
     *the result might be corrupted.
     *This loop detects if `lv_tick_inc` was called while reading `sys_time`.
     *If `tick_irq_flag` was cleared in `lv_tick_inc` try to read again
     *until `tick_irq_flag` remains `1`.*/
    uint32_t result;
    do
    {
        tick_irq_flag = 1;
        result        = sys_time;
    } while (!tick_irq_flag); /*Continue until see a non interrupted cycle*/

    return result;
#else
    return MENU_TICK_CUSTOM_SYS_TIME_EXPR;
#endif
}

/**
 * Get the elapsed milliseconds since a previous time stamp
 * @param prev_tick a previous time stamp (return value of lv_tick_get() )
 * @return the elapsed milliseconds since 'prev_tick'
 */
uint32_t minu_tick_elaps(uint32_t prev_tick)
{
    uint32_t act_time = minu_tick_get();

    /*If there is no overflow in sys_time simple subtract*/
    if (act_time >= prev_tick)
    {
        prev_tick = act_time - prev_tick;
    }
    else
    {
        prev_tick  = UINT32_MAX - prev_tick + 1;
        prev_tick += act_time;
    }

    return prev_tick;
}

void minu_anim_init(minu_anim_t *me)
{
    me->time     = 0;
    me->start    = 0;
    me->end      = 0;
    me->value    = 0;
    me->duration = 500;
    me->path     = easeOutQuad;
}

void minu_anim_update(minu_anim_t *me, const uint32_t currentTime)
{
    if (me->value != me->end)
    {
        uint32_t time_elaps = currentTime - me->time;

        if (time_elaps < me->duration)
        {
            // range: 0 ~ 1 -> 0 ~ maxT
            uint32_t t_current = maxT * (1.0f * currentTime - me->time) / me->duration;
            me->value          = (me->end - me->start) * me->path(t_current) / maxT + me->start;
        }
        else
        {
            me->value = me->end;
        }
    }
}
