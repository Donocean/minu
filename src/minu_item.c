#include "minu_item.h"
#include "xtensa/hal.h"
#include <stdint.h>
#include <stdio.h>

static void minu_appendage_submenu(minu_item_t *me, void *data);
static void minu_appendage_variable(minu_item_t *me, void *data);
static void minu_appendage_checkbox(minu_item_t *me, void *data);

static minu_item_appendage_cb appendages[] = {
    &minu_appendage_submenu,
    &minu_appendage_variable,
    &minu_appendage_checkbox,
};

void minu_item_setName(minu_item_t *const me, char *name)
{
#ifdef MINU_ITEM_USING_CONST_NAME
    me->name = name;
#else
    memcpy(me->name, name, strlen(name) + 1);
#endif
}

void minu_item_set(minu_item_t *const me,
                   char *name,
                   uint8_t type,
                   minu_item_cb cb,
                   void *data)
{
    me->cb = cb;
    me->type = type;
    me->u.user_data = data;
    me->appendage_cb = appendages[type];
    minu_item_setName(me, name);
}

void minu_item_drawAppendage(minu_item_t *me, void *data)
{
    assert(me->appendage_cb != NULL);
    me->appendage_cb(me, data);
}

static void minu_appendage_submenu(minu_item_t *me, void *data)
{
    const char *tag = (const char *)data;

}

static void minu_appendage_variable(minu_item_t *me, void *data)
{
    /* user need convert variable to string before call this function */
    const char *var_data = (const char *)data;

}

static void minu_appendage_checkbox(minu_item_t *me, void *data)
{
    const minu_icon_t *checkbox = (const minu_icon_t *)data;
}

