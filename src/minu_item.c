#include "minu_item.h"
#include <assert.h>
#include <stdint.h>

void minu_item_setName(minu_item_t *const me, char *name)
{
#ifdef MINU_ITEM_USING_CONST_NAME
    me->name = name;
#else
    memcpy(me->name, name, strlen(name) + 1);
#endif
}

void minu_item_onUpdate(minu_item_t *me)
{
    assert(me->ops != NULL);
    me->ops->onUpdate(me);
}

/* static void minu_appendage_submenu(minu_item_t *me, void *data) */
/* { */
/*     const char *tag = (const char *)data; */
/**/
/* } */
/**/
/* static void minu_appendage_variable(minu_item_t *me, void *data) */
/* { */
/*     const char *var_data = (const char *)data; */
/**/
/* } */
/**/
/* static void minu_appendage_checkbox(minu_item_t *me, void *data) */
/* { */
/*     const minu_icon_t *checkbox = (const minu_icon_t *)data; */
/* } */
