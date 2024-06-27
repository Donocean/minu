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

#define MINU_GET_SUPER(x) &((x).super)

static inline void minu_base_setAttribute(void *me, uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
    minu_base_t *pme = me;

    pme->x = x;
    pme->y = y;
    pme->w = w;
    pme->h = h;
}

static inline void minu_base_set_pos(void *me, uint16_t x, uint16_t y)
{
    minu_base_t *pme = me;

    pme->x = x;
    pme->y = y;
}

static inline void minu_base_set_size(void *me, uint16_t w, uint16_t h)
{
    minu_base_t *pme = me;

    pme->w = w;
    pme->h = h;
}

static inline minu_base_t minu_base_getAttribute(void *me)
{
    minu_base_t ret = {0};
    minu_base_t *pme = me;
    ret.w = pme->w;
    ret.h = pme->h;
    ret.x = pme->x;
    ret.y = pme->y;

    return ret;
}

#ifdef __cplusplus
}
#endif

#endif /* __MINU_BASE_H_ */
