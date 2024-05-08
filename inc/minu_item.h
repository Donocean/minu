#ifndef __MINU_ITEM_H_
#define __MINU_ITEM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <string.h>
#include "minu_conf.h"
#include "minu_base.h"

struct _menu;
typedef void (*minu_item_cb)(void *para);

typedef struct
{
    uint16_t width;
    uint16_t height;
    void    *icon;
} minu_icon_t;

typedef struct _menu_item
{
    minu_base_t super;
    /* uint8_t is_hide : 1; */

#ifdef MINU_ITEM_USING_CONST_NAME
    const char *name;
#else
    char name[MINU_ITEM_NAME_SIZE];
#endif

    minu_item_cb  cb;
    void         *user_data;
    struct _menu *sub_menu;
} minu_item_t;

static inline void minu_item_setName(minu_item_t *me, char *name)
{
#ifdef MINU_ITEM_USING_CONST_NAME
    me->name = name;
#else
    memcpy(me->name, name, strlen(name) + 1);
#endif
}
static inline void minu_item_setCb(minu_item_t *me, minu_item_cb cb)
{
    me->cb = cb;
}
static inline void minu_item_setData(minu_item_t *me, void *data)
{
    me->user_data = data;
}
static inline void minu_item_set(minu_item_t *me, char *name, minu_item_cb cb, void *user_data)
{
    minu_item_setName(me, name);
    minu_item_setCb(me, cb);
    minu_item_setData(me, user_data);
}

#ifdef __cplusplus
}
#endif

#endif /* __MINU_ITEM_H_ */
