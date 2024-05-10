#ifndef __MINU_BASE_H_
#define __MINU_BASE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef struct
{
    uint16_t x;
    uint16_t y;
    uint16_t w;
    uint16_t h;
} minu_base_t;

static inline void minu_base_setToZero(minu_base_t *me)
{
    me->x = 0;
    me->y = 0;
    me->w = 0;
    me->h = 0;
}

static inline void minu_base_set(minu_base_t *me, uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
    me->x = x;
    me->y = y;
    me->w = w;
    me->h = h;
}

static inline void minu_base_set_x(minu_base_t *me, uint16_t x)
{
    me->x = x;
}
static inline void minu_base_set_y(minu_base_t *me, uint16_t y)
{
    me->y = y;
}
static inline void minu_base_set_pos(minu_base_t *me, uint16_t x, uint16_t y)
{
    me->x = x;
    me->y = y;
}

static inline void minu_base_set_w(minu_base_t *me, uint16_t w)
{
    me->w = w;
}

static inline void minu_base_set_h(minu_base_t *me, uint16_t h)
{
    me->h = h;
}

static inline void minu_base_set_size(minu_base_t *me, uint16_t w, uint16_t h)
{
    me->w = w;
    me->h = h;
}

static inline minu_base_t minu_base_getAttribute(minu_base_t *const me)
{
    minu_base_t ret = {0};
    ret.w = me->w;
    ret.h = me->h;
    ret.x = me->x;
    ret.y = me->y;
    return ret;
}


#ifdef __cplusplus
}
#endif

#endif /* __MINU_BASE_H_ */
