#include "minu.h"
#include "minu_base.h"
#include "minu_conf.h"
#include "minu_item.h"
#include "minu_vector.h"
#include <stdint.h>
#include <assert.h>
#include MINU_MEM_CUSTOM_INCLUDE

minu_t *minu_creat(uint8_t type, uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
    minu_t *ret = NULL;

    ret = MINU_MEM_CUSTOM_ALLOC(sizeof(minu_t));
    assert(ret != NULL);

    ret->menuType = type;
    ret->is_usingAnim = 1;
    ret->item_index    = 0;
    ret->cotainer_menu = NULL;

    minu_vector_init(&ret->items);
    minu_base_set((minu_base_t *)ret, x, y, w, h);

    return ret;
}

/* TODO: this function can not delete recursive menu in the items. Maybe I should fix it */
void minu_delete(minu_t *me)
{
    minu_vector_del(&me->items);
    MINU_MEM_CUSTOM_FREE(me);
}

void minu_addItem(minu_t *const me, char *name, minu_item_cb cb, void *user_data)
{
    minu_item_t new_item = {0};

    minu_base_setToZero((minu_base_t *)me);
    minu_item_set(&new_item, name, cb, user_data);
    minu_vector_push_back(&me->items, &new_item);
}

void minu_goNext(minu_t *me)
{
    assert(VECTOR_SIZE(me->items) != 0);

    if (++me->item_index == VECTOR_SIZE(me->items))
    {
        me->item_index = 0;
    }
}

void minu_goPrevious(minu_t *me)
{
    assert(VECTOR_SIZE(me->items) != 0);

    if (me->item_index-- == 0)
    {
        me->item_index = VECTOR_SIZE(me->items) - 1;
    }
}

void minu_goIn(minu_t **act_menu)
{
    assert(VECTOR_SIZE((*act_menu)->items) != 0);

    minu_t      *me   = *act_menu;
    minu_item_t *item = &VECTOR_AT(me->items, me->item_index);

    if (item->cb)
        item->cb(item->user_data);

    if (item->sub_menu)
        *act_menu = item->sub_menu;
}

void minu_goOut(minu_t **act_menu)
{
    assert(VECTOR_SIZE((*act_menu)->items) != 0);

    minu_t *me = *act_menu;

    if (VECTOR_SIZE(me->items) == 0)
        return;

    *act_menu = me->cotainer_menu;
}
