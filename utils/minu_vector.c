#include "minu_vector.h"
#include "minu.h"
#include "minu_conf.h"
#include "minu_item.h"
#include <assert.h>
#include <string.h>
#include MINU_MEM_CUSTOM_INCLUDE

void minu_vector_init(minu_vector_itme_t *const me)
{
    me->size = 0;
    me->capacity = 1;
    me->items = MINU_MEM_CUSTOM_ALLOC(me->capacity * sizeof(minu_item_t));
}

void minu_vector_push_back(minu_vector_itme_t *const me, minu_item_t *item)
{
    if (me->size == me->capacity)
    {
        minu_item_t *new;
        new = MINU_MEM_CUSTOM_ALLOC(me->capacity * 2 * sizeof(minu_item_t));
        assert(new != NULL);

        memcpy(new, me->items, sizeof(minu_item_t) * me->size);
        MINU_MEM_CUSTOM_FREE(me->items);

        me->items = new;
        me->capacity = me->capacity * 2;
    }

    memcpy(&me->items[me->size], item, sizeof(minu_item_t));
    me->size++;
}

void minu_vector_pop_back(minu_vector_itme_t *const me)
{
    me->size--;
}

void minu_vector_erase(minu_vector_itme_t *const me, uint16_t index)
{
    //  size: 4 | index = 1 -> remain: 2
    int remain = me->size - index - 1;
    minu_item_t *del_item = &me->items[index];

    memcpy(del_item, &me->items[index + 1], sizeof(minu_item_t) * remain);

    me->size--;
}

inline void minu_vector_del(minu_vector_itme_t *const me)
{
    MINU_MEM_CUSTOM_FREE(me->items);
}
