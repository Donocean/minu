#include "minu_vector.h"
#include "minu.h"
#include "minu_conf.h"
#include <assert.h>
#include <string.h>
#include MINU_MEM_CUSTOM_INCLUDE

void minu_vector_init(minu_vector_itme *const me)
{
    me->size     = 0;
    me->capacity = 1;
    me->items    = MINU_MEM_CUSTOM_ALLOC(me->capacity);
}

void minu_vector_push_back(minu_vector_itme *const me, minu_item_t *item)
{
    if (me->size == me->capacity)
    {
        minu_item_t *new = MINU_MEM_CUSTOM_ALLOC(me->capacity * 2);
        assert(new != NULL);

        memcpy(new, me->items, sizeof(minu_item_t) * me->size);
        MINU_MEM_CUSTOM_FREE(me->items);

        me->items    = new;
        me->capacity = me->capacity * 2;
    }

    memcpy(&me->items[me->size], item, sizeof(minu_item_t));
    me->size++;
}

inline void minu_vector_del(minu_vector_itme *const me)
{
    MINU_MEM_CUSTOM_FREE(me->items);
}
