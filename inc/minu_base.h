#ifndef __MINU_BASE_H_
#define __MINU_BASE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

// TODO: add animation support within the base class

typedef struct
{
    uint16_t x;
    uint16_t y;
} minu_pos_t;

typedef struct
{
    uint16_t w;
    uint16_t h;
} minu_size_t;

typedef struct
{
    uint16_t x;
    uint16_t y;
    uint16_t w;
    uint16_t h;
} minu_base_t;

#define MINU_GET_SUPER(x) &((x).super)

static inline void minu_base_setAttr(void *me,
                                     uint16_t x,
                                     uint16_t y,
                                     uint16_t w,
                                     uint16_t h)
{
    ((minu_base_t *)me)->x = x;
    ((minu_base_t *)me)->y = y;
    ((minu_base_t *)me)->w = w;
    ((minu_base_t *)me)->h = h;
}

static inline void minu_base_setAttrWith(void *dest, void *src)
{
    *((minu_base_t *)dest) = *((minu_base_t *)src);
}

static inline minu_base_t *minu_base_getAttr(void *me)
{
    return (minu_base_t *)me;
}

static inline void minu_base_setPos(void *me, uint16_t x, uint16_t y)
{
    ((minu_base_t *)me)->x = x;
    ((minu_base_t *)me)->y = y;
}

static inline void minu_base_setSize(void *me, uint16_t w, uint16_t h)
{
    ((minu_base_t *)me)->w = w;
    ((minu_base_t *)me)->h = h;
}

static inline void minu_pos_set(minu_pos_t *me, uint16_t x, uint16_t y)
{
    me->x = x;
    me->y = y;
}

static inline void minu_size_set(minu_size_t *me, uint16_t w, uint16_t h)
{
    me->w = w;
    me->h = h;
}

#ifdef __cplusplus
}
#endif

#endif /* __MINU_BASE_H_ */
