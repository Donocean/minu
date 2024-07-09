#include "minu_item.h"
#include <stdint.h>

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
    minu_item_setName(me, name);
}

