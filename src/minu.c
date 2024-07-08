#include "minu.h"
#include "minu_base.h"
#include "minu_disp.h"
#include "minu_item.h"
#include <assert.h>
#include <stdbool.h>
#include <sys/queue.h>
#include MINU_MEM_CUSTOM_INCLUDE

typedef struct minu_t minu_t;
struct minu_t
{
    minu_base_t super;

    const char *title;
    minu_t *cotainer_menu;
    minu_vector_itme_t items;

    /* for graph */
    minu_layout_t layout;

    int16_t item_index; /* -1 means there is nothing in the items vector */
    uint8_t menuType     : 3; /* @ref enum minu_type_t */
    uint8_t is_usingAnim : 1;
    uint8_t is_loopItem  : 1;
};

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
static void minu_setLayoutDefault(const minu_handle_t me)
{
    me->layout.item_gap = 0;
    me->layout.border_gap = 2;
    me->layout.bar_width = 5;
}

void minu_setLayout(const minu_handle_t me, minu_layout_t *layout)
{
    me->layout.item_gap = layout->item_gap;
    me->layout.border_gap = layout->border_gap;
}

void minu_loopItem_on_off(const minu_handle_t me, bool enable)
{
    me->is_loopItem = enable;
}

minu_handle_t minu_creat(uint8_t type,
                         const char *title,
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
    ret->item_index = 0;
    ret->is_loopItem = 1;
    ret->is_usingAnim = 1;
    ret->cotainer_menu = NULL;

    minu_vector_init(&ret->items);

    minu_setLayoutDefault(ret);
    /* set menu attributes */
    minu_base_setAttr(ret, x, y, w, h);

    return ret;
}

void minu_addItem(const minu_handle_t me,
                  minu_item_type_t type,
                  char *name,
                  minu_item_cb cb,
                  void *user_data)
{
    uint16_t x, y, w, h;
    minu_item_t new_item = {0};
    minu_layout_t *layout = &me->layout;
    minu_base_t *menu_attr = minu_base_getAttr(me);

    w = minu_disp_getStrWidth(name);
    h = minu_disp_getFontHeight();

    x = menu_attr->x;
    y = menu_attr->y + (h + layout->item_gap) * VECTOR_SIZE(me->items);

    minu_base_setAttr(&new_item, x, y, w, h);
    minu_item_set(&new_item, name, cb, user_data);
    minu_vector_push_back(&me->items, &new_item);
}

/**
 * @brief return the current selected item of minu
 */
minu_item_t *minu_getSelectedItem(minu_handle_t me)
{
    return &VECTOR_AT(me->items, me->item_index);
}

/**
 * @brief return the number of the items in the minu
 */
uint16_t minu_getSize(minu_handle_t me)
{
    return VECTOR_SIZE(me->items);
}

int16_t minu_getIndex(minu_handle_t me)
{
    return me->item_index;
}

minu_layout_t *minu_getLayout(minu_handle_t me)
{
    return &me->layout;
}

minu_vector_itme_t *minu_getItems(minu_handle_t me)
{
    return &me->items;
}

/* TODO: this function can not delete recursive menu in the items. Maybe I
 * should fix it */
void minu_delete(minu_handle_t me)
{
    minu_vector_del(&me->items);
    MINU_MEM_CUSTOM_FREE(me);
}

void minu_goNext(minu_handle_t me)
{
    assert(VECTOR_SIZE(me->items) != 0);

    if (++me->item_index == VECTOR_SIZE(me->items))
    {
        if (me->is_loopItem)
            me->item_index = 0;
        else
            me->item_index = VECTOR_SIZE(me->items) - 1;
    }
}

void minu_goPrevious(minu_handle_t me)
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

void minu_goIn(minu_handle_t *act_menu)
{
    assert(VECTOR_SIZE((*act_menu)->items) != 0);

    minu_t *me = *act_menu;
    minu_item_t *item = &VECTOR_AT(me->items, me->item_index);

    if (item->cb)
        item->cb(item->user_data);

    if (item->sub_menu)
        *act_menu = item->sub_menu;
}

void minu_goOut(minu_handle_t *act_menu)
{
    assert(VECTOR_SIZE((*act_menu)->items) != 0);

    minu_t *me = *act_menu;

    if (VECTOR_SIZE(me->items) == 0)
        return;

    *act_menu = me->cotainer_menu;
}

void minu_deleteItem(minu_handle_t me)
{
    assert(VECTOR_SIZE(me->items) != 0);

    uint8_t is_end = 0;

    /* is last item? */
    if (me->item_index == (VECTOR_SIZE(me->items) - 1))
        is_end = 1;

    /* modfiy every items position */
    for (uint16_t i = VECTOR_SIZE(me->items) - 1; i > me->item_index; i--)
    {
        minu_item_t *now = &VECTOR_AT(me->items, i);
        minu_base_t *prev_pos = minu_base_getAttr(&VECTOR_AT(me->items, i - 1));
        minu_base_setPos(now, prev_pos->x, prev_pos->y);
    }

    /* this function will modfiy vector size,
     * therefore we need to record is_end */
    minu_vector_erase(&me->items, me->item_index);

    if (is_end)
        me->item_index--;
}
