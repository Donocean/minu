#include "minu_base.h"
#include "minu_anim.h"

void minu_base_setAttrWith(void *dest, void *src)
{
    *((minu_base_t *)dest) = *((minu_base_t *)src);
}

void minu_base_set(void *me, uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
    ((minu_base_t *)me)->x = x;
    ((minu_base_t *)me)->y = y;
    ((minu_base_t *)me)->w = w;
    ((minu_base_t *)me)->h = h;
}

void minu_base_setPos(void *me, uint16_t x, uint16_t y)
{
    ((minu_base_t *)me)->x = x;
    ((minu_base_t *)me)->y = y;
}

void minu_base_setSize(void *me, uint16_t w, uint16_t h)
{
    ((minu_base_t *)me)->w = w;
    ((minu_base_t *)me)->h = h;
}

void minu_pos_set(minu_pos_t *me, uint16_t x, uint16_t y)
{
    me->x = x;
    me->y = y;
}

void minu_size_set(minu_size_t *me, uint16_t w, uint16_t h)
{
    me->w = w;
    me->h = h;
}


