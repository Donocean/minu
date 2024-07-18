#include "minu_base.h"
#include "minu_anim.h"

void minu_base_init(void *me, uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
    minu_base_t *obj = (minu_base_t *)me;

#ifdef MINU_USE_ANIMATION
    minu_anim_init(&obj->anim_x);
    minu_anim_init(&obj->anim_y);
    minu_anim_init(&obj->anim_w);
    minu_anim_init(&obj->anim_h);
    minu_anim_setTarget(&obj->anim_x, 0, x);
    minu_anim_setTarget(&obj->anim_y, 0, y);
    minu_anim_setTarget(&obj->anim_w, 0, w);
    minu_anim_setTarget(&obj->anim_h, 0, h);
    obj->x = obj->y = obj->w = obj->h = 0;
    return;
#endif

    obj->x = x, obj->y = y, obj->w = w, obj->h = h;
}

void minu_base_initWith(void *dest, minu_attr_t *src)
{
    minu_base_init(dest, src->x, src->y, src->w, src->h);
}

void minu_base_set(void *me, uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
    minu_base_t *obj = (minu_base_t *)me;
    if (x == obj->anim_x.end && y == obj->anim_y.end && w == obj->anim_w.end &&
        h == obj->anim_h.end)
        return;

#ifdef MINU_USE_ANIMATION
    minu_anim_setTarget(&obj->anim_x, obj->x, x);
    minu_anim_setTarget(&obj->anim_y, obj->y, y);
    minu_anim_setTarget(&obj->anim_w, obj->w, w);
    minu_anim_setTarget(&obj->anim_h, obj->h, h);
    return;
#endif
    obj->x = x, obj->y = y, obj->w = w, obj->h = h;
}

void minu_base_setAttrWith(void *dest, minu_attr_t *src)
{
    minu_base_t *me = (minu_base_t *)dest;

    if (src->x == me->anim_x.end && src->y == me->anim_y.end &&
        src->w == me->anim_w.end && src->h == me->anim_h.end)
        return;

#ifdef MINU_USE_ANIMATION
    minu_anim_setTarget(&me->anim_x, me->x, src->x);
    minu_anim_setTarget(&me->anim_y, me->y, src->y);
    minu_anim_setTarget(&me->anim_w, me->w, src->w);
    minu_anim_setTarget(&me->anim_h, me->h, src->h);
    return;
#endif
    me->x = src->x, me->y = src->y, me->w = src->w, me->h = src->h;
}

void minu_base_setPos(void *me, uint16_t x, uint16_t y)
{
    minu_base_t *obj = (minu_base_t *)me;

    if (x == obj->anim_x.end && y == obj->anim_y.end)
        return;

#ifdef MINU_USE_ANIMATION
    minu_anim_setTarget(&obj->anim_x, obj->x, x);
    minu_anim_setTarget(&obj->anim_y, obj->y, y);
    return;
#endif

    obj->x = x;
    obj->y = y;
}

void minu_base_setSize(void *me, uint16_t w, uint16_t h)
{
    minu_base_t *obj = (minu_base_t *)me;

    if (w == obj->anim_w.end && h == obj->anim_h.end)
        return;

#ifdef MINU_USE_ANIMATION
    minu_anim_setTarget(&obj->anim_w, obj->w, w);
    minu_anim_setTarget(&obj->anim_h, obj->h, h);
    return;
#endif

    obj->w = w;
    obj->h = h;
}

void minu_pos_init(minu_pos_t *me, uint16_t x, uint16_t y)
{
#ifdef MINU_USE_ANIMATION
    me->x = me->y = 0;
    minu_anim_init(&me->anim_x);
    minu_anim_init(&me->anim_y);
    minu_anim_setTarget(&me->anim_x, 0, x);
    minu_anim_setTarget(&me->anim_y, 0, y);
    return;
#endif
    me->x = x;
    me->y = y;
}

void minu_pos_set(minu_pos_t *me, uint16_t x, uint16_t y)
{
    if (x == me->anim_x.end && y == me->anim_y.end)
        return;

#ifdef MINU_USE_ANIMATION
    minu_anim_setTarget(&me->anim_x, me->x, x);
    minu_anim_setTarget(&me->anim_y, me->y, y);
    return;
#endif
    me->x = x;
    me->y = y;
}

#ifdef MINU_USE_ANIMATION

void minu_pos_update(minu_pos_t *me)
{
    minu_anim_update(&me->anim_x);
    minu_anim_update(&me->anim_y);

    me->x = minu_anim_getValue(&me->anim_x);
    me->y = minu_anim_getValue(&me->anim_y);
}

void minu_base_update(void *me)
{
    minu_base_t *obj = (minu_base_t *)me;

    minu_anim_update(&obj->anim_x);
    minu_anim_update(&obj->anim_y);
    minu_anim_update(&obj->anim_w);
    minu_anim_update(&obj->anim_h);

    obj->x = minu_anim_getValue(&obj->anim_x);
    obj->y = minu_anim_getValue(&obj->anim_y);
    obj->w = minu_anim_getValue(&obj->anim_w);
    obj->h = minu_anim_getValue(&obj->anim_h);
}

void minu_base_reset(void *me)
{
    minu_base_t *obj = (minu_base_t *)me;

    minu_anim_reset(&obj->anim_x);
    minu_anim_reset(&obj->anim_y);
    minu_anim_reset(&obj->anim_w);
    minu_anim_reset(&obj->anim_h);
    obj->x = obj->y = obj->w = obj->h = 0;
}

void minu_base_end(void *me)
{
    minu_base_t *obj = (minu_base_t *)me;

    minu_anim_end(&obj->anim_x);
    minu_anim_end(&obj->anim_y);
    minu_anim_end(&obj->anim_w);
    minu_anim_end(&obj->anim_h);

    obj->x = minu_anim_getValue(&obj->anim_x);
    obj->y = minu_anim_getValue(&obj->anim_y);
    obj->w = minu_anim_getValue(&obj->anim_w);
    obj->h = minu_anim_getValue(&obj->anim_h);
}

#endif /* MINU_USE_ANIMATION */
