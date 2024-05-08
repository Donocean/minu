#include "minu.h"
#include "minu_base.h"
#include "minu_conf.h"
#include "minu_vector.h"
#include <stdint.h>
#include <assert.h>
#include MINU_MEM_CUSTOM_INCLUDE

static void minu_initDefault(minu_t *me)
{
    me->item_index    = 0;
    me->item_num      = 0;
    me->cotainer_menu = NULL;
    minu_vector_init(&me->items);
    minu_base_setToZero(&me->super);
}

minu_t *minu_creat(void)
{
    minu_t *ret = NULL;

    ret = MINU_MEM_CUSTOM_ALLOC(sizeof(minu_t));
    if (!ret)
        return NULL;
    minu_initDefault(ret);

    return ret;
}

/* TODO: this function can not delete recursive menu in the items. Maybe I should fix it */
void minu_delete(minu_t *me)
{
    minu_vector_del(&me->items);
    MINU_MEM_CUSTOM_FREE(me);
}

void minu_goNext(minu_t *me)
{
    assert(me->item_num != 0);

    if (++me->item_index == me->item_num)
    {
        me->item_index = 0;
    }
}

void minu_goPrevious(minu_t *me)
{
    assert(me->item_num != 0);

    if (me->item_index-- == 0)
    {
        me->item_index = me->item_num - 1;
    }
}

void minu_goIn(minu_t **act_menu)
{
    assert((*act_menu)->item_num != 0);

    minu_t      *me   = *act_menu;
    minu_item_t *item = &VECTOR_AT(me->items, me->item_index);

    if (item->cb)
        item->cb(item->user_data);

    if (item->sub_menu)
        *act_menu = item->sub_menu;
}

void minu_goOut(minu_t **act_menu)
{
    assert((*act_menu)->item_num != 0);

    minu_t *me = *act_menu;

    if (me->item_num == 0)
        return;

    *act_menu = me->cotainer_menu;
}

