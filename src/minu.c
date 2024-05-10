#include "minu.h"
#include "minu_base.h"
#include "minu_disp.h"
#include "minu_item.h"
#include "minu_vector.h"
#include <stdint.h>
#include <assert.h>
#include MINU_MEM_CUSTOM_INCLUDE

static void minu_setLayoutDefault(minu_t *const me)
{
    me->layout.border_gap = 2;
    me->layout.u.item_gap = 0;
}

minu_t *minu_creat(const char *title, uint8_t type, uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
    minu_t *ret = NULL;

    ret = MINU_MEM_CUSTOM_ALLOC(sizeof(minu_t));
    assert(ret != NULL);

    ret->title         = title;
    ret->menuType      = type;
    ret->is_usingAnim  = 1;
    ret->item_index    = 0;
    ret->cotainer_menu = NULL;

    minu_vector_init(&ret->items);

    minu_setLayoutDefault(ret);
    /* set menu attributes */
    minu_base_set((minu_base_t *)ret, x, y, w, h);
    /* set selector attributes */
    minu_base_set_pos(&ret->selector, x, y + ret->layout.border_gap);

    return ret;
}

void minu_addItem(minu_t *const me, char *name, minu_item_cb cb, void *user_data)
{
    minu_item_t    new_item = {0};
    minu_layout_t *layout   = &me->layout;
    int8_t         font_h   = minu_disp_getFontHeight();
    uint16_t       x, y, w;

    x = me->super.x;
    y = me->super.y + (font_h + layout->u.item_gap) * VECTOR_SIZE(me->items) + layout->border_gap;
    w = minu_disp_getStrWidth(name);

    minu_base_set_x((minu_base_t *)&new_item, x);
    minu_base_set_y((minu_base_t *)&new_item, y);
    minu_base_set_w((minu_base_t *)&new_item, w);
    minu_base_set_h((minu_base_t *)&new_item, font_h);
    minu_item_set(&new_item, name, cb, user_data);
    minu_vector_push_back(&me->items, &new_item);
}

void minu_setLayout(minu_t *const me, minu_layout_t *layout)
{
    me->layout.u.item_gap = layout->u.item_gap;
    me->layout.border_gap = layout->border_gap;
}

/* TODO: this function can not delete recursive menu in the items. Maybe I should fix it */
void minu_delete(minu_t *me)
{
    minu_vector_del(&me->items);
    MINU_MEM_CUSTOM_FREE(me);
}

void minu_goNext(minu_t *me)
{
    assert(VECTOR_SIZE(me->items) != 0);

    if (++me->item_index == VECTOR_SIZE(me->items))
    {
        if (me->is_loopItem)
        {
            me->item_index = 0;
        }
        else
            me->item_index = VECTOR_SIZE(me->items) - 1;
    }

    me->selector.w = VECTOR_AT(me->items, me->item_index).super.w + me->layout.border_gap * 2;
    me->selector.y = VECTOR_AT(me->items, me->item_index).super.y;
}

void minu_goPrevious(minu_t *me)
{
    assert(VECTOR_SIZE(me->items) != 0);

    if (me->item_index-- == 0)
    {
        if (me->is_loopItem)
        {
            me->item_index = VECTOR_SIZE(me->items) - 1;
        }
        else
            me->item_index = 0;
    }
    me->selector.w = VECTOR_AT(me->items, me->item_index).super.w + me->layout.border_gap * 2;
    me->selector.y = VECTOR_AT(me->items, me->item_index).super.y;
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
