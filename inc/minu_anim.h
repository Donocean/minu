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
    int16_t start;    /* animation's start value */
    int16_t end;      /* animation's end value */
    int16_t value;    /* value of the transition at the current time */
    int16_t duration; /* animation time in ms. must smaller than maxT */
    uint32_t time;    /* record the system time when the animation starts */
    minu_easingPath_t path; /* animation's transition path */
} minu_anim_t;

/* NOTE: animation needs tick to perform */
uint32_t minu_tick_get(void);
uint32_t minu_tick_elaps(uint32_t prev_tick);
void minu_tick_inc(uint32_t tick_period);

static inline void minu_anim_setTarget(minu_anim_t *me,
                                       int16_t start,
                                       int16_t end)
{
    me->start = start;
    me->end = end;
    me->time = minu_tick_get();
}

static inline void minu_anim_setDuration(minu_anim_t *me, uint16_t duration)
{
    me->duration = duration;
}

static inline void minu_anim_setEasingPath(minu_anim_t *me,
                                           minu_easingPath_t path)
{
    me->path = path;
}

static inline void minu_anim_set(minu_anim_t *me,
                                 uint32_t start,
                                 uint32_t end,
                                 uint32_t duration,
                                 minu_easingPath_t path)
{
    me->start = start;
    me->end = end;
    me->duration = duration;
    me->path = path;
}

static inline void minu_anim_end(minu_anim_t *me)
{
    me->value = me->start = me->end;
}

static inline int16_t minu_anim_getValue(minu_anim_t *me)
{
    return me->value;
}

void minu_anim_init(minu_anim_t *me);
void minu_anim_update(minu_anim_t *me);

#ifdef __cplusplus
}
#endif

#endif /* __MINU_ANIM_H_ */
