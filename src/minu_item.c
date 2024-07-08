#include "minu_item.h"

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
                   minu_item_cb cb,
                   void *user_data)
{
    me->cb = cb;
    me->user_data = user_data;
    minu_item_setName(me, name);
}



