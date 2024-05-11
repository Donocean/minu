#include "minu.h"
#include "minu_base.h"
#include "minu_disp.h"
#include "minu_item.h"
#include "minu_vector.h"
#include <stdint.h>
#include <stdio.h>
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
    ret->movingOffset  = 0;
    ret->cotainer_menu = NULL;

    minu_vector_init(&ret->items);

    minu_setLayoutDefault(ret);
    /* set menu attributes */
    minu_base_set((minu_base_t *)ret, x, y, w, h);
    /* set selector attributes */
    minu_base_set_pos(&ret->selector, x, y);

    return ret;
}

void minu_addItem(minu_t *const me, char *name, minu_item_cb cb, void *user_data)
{
    uint16_t       x, y, w, h;
    minu_item_t    new_item = {0};
    minu_layout_t *layout   = &me->layout;

    w = minu_disp_getStrWidth(name);
    h = minu_disp_getFontHeight();

    x = me->super.x;
    y = me->super.y + (h + layout->u.item_gap) * VECTOR_SIZE(me->items);

    minu_base_set((minu_base_t *)&new_item, x, y, w, h);
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
    minu_base_t    item_attr, menu_attr;
    minu_layout_t *layout = &me->layout;
    int8_t         font_h = minu_disp_getFontHeight();
    int16_t        offset_y;

    if (++me->item_index == VECTOR_SIZE(me->items))
    {
        if (me->is_loopItem)
        {
            me->item_index = 0;
        }
        else
            me->item_index = VECTOR_SIZE(me->items) - 1;
    }
    menu_attr = minu_base_getAttribute((minu_base_t *)me);
    item_attr = minu_base_getAttribute((minu_base_t *)&VECTOR_AT(me->items, me->item_index));
    offset_y  = item_attr.y + item_attr.h + me->movingOffset;

    if (offset_y > menu_attr.y && offset_y <= (menu_attr.y + menu_attr.h))
        me->selector.y = item_attr.y + me->movingOffset;
    else
    {
        /* let selector back to the first item position */
        if (me->is_loopItem && me->item_index == 0)
        {
            me->selector.y   = item_attr.y;
            me->movingOffset = 0;
        }
        else
            me->movingOffset -= font_h + layout->u.item_gap;
    }
    me->selector.w = item_attr.w;

    printf("\nr_y = %d s_y = %d m_y =%d\n", offset_y, me->selector.y, me->movingOffset);
}

void minu_goPrevious(minu_t *me)
{
    assert(VECTOR_SIZE(me->items) != 0);
    minu_base_t    item_attr, menu_attr;
    int8_t         font_h = minu_disp_getFontHeight();
    minu_layout_t *layout = &me->layout;
    int16_t        offset_y;

    if (me->item_index-- == 0)
    {
        if (me->is_loopItem)
        {
            me->item_index = VECTOR_SIZE(me->items) - 1;
        }
        else
            me->item_index = 0;
    }
    menu_attr = minu_base_getAttribute((minu_base_t *)me);
    item_attr = minu_base_getAttribute((minu_base_t *)&VECTOR_AT(me->items, me->item_index));
    offset_y  = item_attr.y + item_attr.h + me->movingOffset;

    if (offset_y > menu_attr.y && offset_y <= (menu_attr.y + menu_attr.h))
        me->selector.y = item_attr.y + me->movingOffset;
    else
    {
        if (me->is_loopItem && me->item_index == (VECTOR_SIZE(me->items) - 1))
        {
            uint8_t last_pos = menu_attr.h / (font_h + layout->u.item_gap) - 1;
            me->selector.y   = menu_attr.y + last_pos * (font_h + layout->u.item_gap);
            me->movingOffset = -1 * (item_attr.y - me->selector.y);
        }
        else
            me->movingOffset += font_h + layout->u.item_gap;
    }
    me->selector.w = item_attr.w;

    printf("\nr_y = %d s_y = %d m_y =%d\n", offset_y, me->selector.y, me->movingOffset);
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
