#include "minu.h"
#include "minu_base.h"
#include "minu_disp.h"
#include "minu_item.h"
#include "minu_vector.h"
#include <stdint.h>
#include <assert.h>
#include <sys/queue.h>
#include MINU_MEM_CUSTOM_INCLUDE

static void minu_setLayoutDefault(minu_t *const me)
{
    me->layout.border_gap = 2;
    me->layout.u.item_gap = 0;
    me->layout.bar_width = 5;
}

minu_t *minu_creat(const char *title,
                   uint8_t type,
                   uint16_t x,
                   uint16_t y,
                   uint16_t w,
                   uint16_t h)
{
    minu_t *ret = NULL;

    ret = MINU_MEM_CUSTOM_ALLOC(sizeof(minu_t));
    assert(ret != NULL);

    ret->title = title;
    ret->menuType = type;
    ret->is_usingAnim = 1;
    ret->item_index = 0;
    ret->movingOffset = 0;
    ret->cotainer_menu = NULL;

    minu_vector_init(&ret->items);

    minu_setLayoutDefault(ret);
    /* set menu attributes */
    minu_base_setAttr(ret, x, y, w, h);
    /* set selector attributes */
    minu_base_set_pos(&ret->selector, x, y);

    return ret;
}

void minu_addItem(minu_t *const me,
                  char *name,
                  minu_item_cb cb,
                  void *user_data)
{
    uint16_t x, y, w, h;
    minu_item_t new_item = {0};
    minu_layout_t *layout = &me->layout;

    w = minu_disp_getStrWidth(name);
    h = minu_disp_getFontHeight();

    x = me->super.x;
    y = me->super.y + (h + layout->u.item_gap) * VECTOR_SIZE(me->items);

    minu_base_setAttr(&new_item, x, y, w, h);
    minu_item_set(&new_item, name, cb, user_data);
    minu_vector_push_back(&me->items, &new_item);
}

/**
 *     .------------------------------------------------------. <- canvas top
 *     |                           ↑                          |
 *     |             border_gap -> |                          |
 *     |                           ↓                          |
 *     |  ui_pos(x,y)  ->  .---------------.<- UI area top    |
 *     |                   |1.-------------|                  |
 *     |                   |---------------|                  |
 *     |                   |2.-------------|<- item2          |
 *     |                   |---------------|<- item_gap       |
 *     |       UI area ->  |3.-------------|<- item3          | <- canvas
 *     |                   |---------------|                  |
 *     |                   |4.-------------|                  |
 *     |                   |---------------|                  |
 *     | <---------------> |5.-------------|                  |
 *     |        ↑          .---------------.<- UI area bottom |
 *     |        |                  ↑                          |
 *     |     border_gap ---------> |                          |
 *     |                           ↓                          |
 *     .------------------------------------------------------. <- canvas bottom
 */
void minu_setLayout(minu_t *const me, minu_layout_t *layout)
{
    me->layout.u.item_gap = layout->u.item_gap;
    me->layout.border_gap = layout->border_gap;
}

/* TODO: this function can not delete recursive menu in the items. Maybe I
 * should fix it */
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
}

void minu_goPrevious(minu_t *me)
{
    assert(VECTOR_SIZE(me->items) != 0);

    if (me->item_index-- == 0)
    {
        if (me->is_loopItem)
            me->item_index = VECTOR_SIZE(me->items) - 1;
        else
            me->item_index = 0;
    }
}

void minu_goIn(minu_t **act_menu)
{
    assert(VECTOR_SIZE((*act_menu)->items) != 0);

    minu_t *me = *act_menu;
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

// TODO: occurs bugs when remaining 2 items
void minu_deleteItem(minu_t *me)
{
    assert(VECTOR_SIZE(me->items) != 0);

    /* modfiy every items position */
    for (uint16_t i = VECTOR_SIZE(me->items) - 1; i > me->item_index; i--)
    {
        minu_item_t *now = &VECTOR_AT(me->items, i);
        minu_base_t prev_pos = minu_base_getAttr(&VECTOR_AT(me->items, i - 1));
        minu_base_set_pos(now, prev_pos.x, prev_pos.y);
    }

    minu_vector_erase(&me->items, me->item_index);
}
