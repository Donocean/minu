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
#ifdef MINU_USE_ANIMATION
    minu_anim_t anim_x;
    minu_anim_t anim_y;
#endif
} minu_pos_t;

typedef struct
{
    uint16_t w;
    uint16_t h;

#ifdef MINU_USE_ANIMATION
    minu_anim_t anim_w;
    minu_anim_t anim_h;
#endif
} minu_size_t;

typedef struct
{
    uint16_t x;
    uint16_t y;
    uint16_t w;
    uint16_t h;

#ifdef MINU_USE_ANIMATION
    minu_anim_t anim_x;
    minu_anim_t anim_y;
#endif
} minu_base_t;

#define MINU_GET_SUPER(x) &((x).super)

void minu_base_setAttrWith(void *dest, void *src);
void minu_base_set(void *me, uint16_t x, uint16_t y, uint16_t w, uint16_t h);
void minu_base_setPos(void *me, uint16_t x, uint16_t y);
void minu_base_setSize(void *me, uint16_t w, uint16_t h);
void minu_pos_set(minu_pos_t *me, uint16_t x, uint16_t y);
void minu_size_set(minu_size_t *me, uint16_t w, uint16_t h);

static inline minu_base_t *minu_base_getAttr(void *me)
{
    return (minu_base_t *)me;
}

#ifdef __cplusplus
}
#endif

#endif /* __MINU_BASE_H_ */
