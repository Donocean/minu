#include "minu_item.h"
#include "minu_types.h"
#include <string.h>
#include <assert.h>
#include <stdint.h>

void minu_item_setName(minu_item_t *const me, const char *name)
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

    if (me->ops->onUpdate)
        me->ops->onUpdate(me);
}

state_t minu_item_onEntry(minu_item_t *me, minu_item_para_t *para)
{
    assert(me->ops != NULL);
    return me->ops->onEntry(me, para);
}

void minu_item_onHandling(minu_item_t *me, minu_event_id_t e)
{
    assert(me->ops != NULL);
    me->ops->onHandling(me, e);
}

void minu_item_drawAppendage(minu_item_t *me, void *menu, minu_pos_t *target)
{
    assert(me->ops != NULL);
    me->ops->drawAppendage(me, menu, target);
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
