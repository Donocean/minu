#ifndef __MINU_BASE_H_
#define __MINU_BASE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "minu_conf.h"
#ifdef MINU_USE_ANIMATION
#include "minu_anim.h"
#endif

// TODO: add animation support within the base class
typedef struct
{
    uint16_t x;
    uint16_t y;
    uint16_t w;
    uint16_t h;

#ifdef MINU_USE_ANIMATION
    minu_anim_t anim_x;
    minu_anim_t anim_y;
    minu_anim_t anim_w;
    minu_anim_t anim_h;
#endif
} minu_base_t;

typedef struct
{
    uint16_t x;
    uint16_t y;
#ifdef MINU_USE_ANIMATION
    minu_anim_t anim_x;
    minu_anim_t anim_y;
#endif
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
} minu_attr_t;

void minu_base_init(void *me, uint16_t x, uint16_t y, uint16_t w, uint16_t h);
void minu_base_initWith(void *dest, minu_attr_t *src);

void minu_base_set(void *me, uint16_t x, uint16_t y, uint16_t w, uint16_t h);
void minu_base_setAttrWith(void *dest, minu_attr_t *src);
void minu_base_setPos(void *me, uint16_t x, uint16_t y);
void minu_base_setSize(void *me, uint16_t w, uint16_t h);

static inline minu_attr_t minu_base_getAttr(void *me)
{
    minu_base_t *obj = me;
    minu_attr_t ret = {0};

#ifdef MINU_USE_ANIMATION
    ret.x = obj->anim_x.end;
    ret.y = obj->anim_y.end;
    ret.w = obj->anim_w.end;
    ret.h = obj->anim_h.end;
    return ret;
#endif
    ret.x = obj->x;
    ret.y = obj->h;
    ret.w = obj->w;
    ret.h = obj->h;

    return ret;
}

static inline minu_base_t *minu_base_get(void *me)
{
    return (minu_base_t *)me;
}

/* for minu_pos_t api */
void minu_pos_init(minu_pos_t *me, uint16_t x, uint16_t y);
void minu_pos_set(minu_pos_t *me, uint16_t x, uint16_t y);

#ifdef MINU_USE_ANIMATION
/* for minu_pos_t api */
void minu_pos_update(minu_pos_t *me);

void minu_base_end(void *me);
void minu_base_reset(void *me);
void minu_base_update(void *me);
#endif

#ifdef __cplusplus
}
#endif

#endif /* __MINU_BASE_H_ */
