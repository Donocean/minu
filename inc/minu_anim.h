#ifndef __MINU_ANIM_H_
#define __MINU_ANIM_H_

#include <stdlib.h>
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "minu_anim_easing.h"

typedef struct
{
    int16_t start;     /* animation's start value */
    int16_t end;       /* animation's end value */
    int16_t value;     /* value of the transition at the current time */
    uint16_t duration; /* animation time in ms. must smaller than maxT */

    uint32_t time;     /* record the system time when the animation starts */
    easingPath path;   /* animation's transition path */

    void *var;         /* variable to be animated */
    void (*exec_cb)(void *var, int16_t); /* function to execute to animate */
} minu_anim_t;

/* animation needs tick to perform */
uint32_t minu_tick_get(void);
uint32_t minu_tick_elaps(uint32_t prev_tick);
void minu_tick_inc(uint32_t tick_period);

static inline void minu_anim_setStart(minu_anim_t *me, int16_t start)
{
    me->start = start;
}

static inline void minu_anim_setEnd(minu_anim_t *me, int16_t end)
{
    me->end = end;
}

static inline void minu_anim_setTarget(minu_anim_t *me,
                                       int16_t start,
                                       int16_t end)
{
    me->start = start;
    me->end = end;
}

static inline void minu_anim_setDuration(minu_anim_t *me, uint16_t duration)
{
    me->duration = duration;
}

static inline void minu_anim_setEasingPath(minu_anim_t *me, easingPath path)
{
    me->path = path;
}

static inline void minu_anim_start(minu_anim_t *me, const uint32_t currentTime)
{
    me->time = me->time + currentTime;
}

static inline void minu_anim_setVarCb(minu_anim_t *me,
                                      void *var,
                                      void (*cb)(void *, int16_t))
{
    me->var = var;
    me->exec_cb = cb;
}

static inline void minu_anim_set(minu_anim_t *me,
                                 uint32_t start,
                                 uint32_t end,
                                 uint32_t duration,
                                 easingPath path)
{
    me->start = start;
    me->end = end;
    me->duration = duration;
    me->path = path;
}

static inline int16_t minu_anim_getValue(minu_anim_t *me)
{
    return me->value;
}

void minu_anim_init(minu_anim_t *me);
void minu_anim_update(minu_anim_t *me, const uint32_t currentTime);

#ifdef __cplusplus
}
#endif

#endif /* __MINU_ANIM_H_ */
