#include "minu.h"
#include "minu_base.h"
#include "minu_disp.h"
#include "minu_item.h"
#include "minu_types.h"
#include "minu_vector.h"
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/queue.h>
#include MINU_MEM_CUSTOM_INCLUDE

#include "minu_item_submenu.h"
#include "minu_item_checkbox.h"
#include "minu_item_variable.h"
#include "minu_item_window.h"

typedef struct minu_t minu_t;
struct minu_t
{
    minu_base_t super;

    const char *title;
    minu_t *cotainer_menu;
    minu_vector_itme_t items;

    /* for graph */
    minu_layout_t layout;
    /* @ref marco minu type */
    minu_type_cb type_cb;

    int16_t item_index; /* -1 means there is nothing in the items vector */
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
static void minu_setLayoutDefault(minu_handle_t me)
{
    me->layout.item_vgap = 2;
    me->layout.item_hgap = 20;
    me->layout.border_gap = 2;
    me->layout.bar_width = 5;
}

void minu_type_selector_vertical(minu_handle_t me,
                                 minu_item_t *new_item,
                                 minu_size_t *str_size,
                                 minu_icon_t *icon)
{
    minu_base_t item_attr = {0};
    uint16_t item_size = minu_getSize(me);
    minu_layout_t *layout = minu_getLayout(me);
    minu_vector_itme_t *vec = minu_getItems(me);
    minu_base_t *menu_attr = minu_base_getAttr(me);

    if (item_size)
    {
        const minu_base_t *last_item_attr =
            minu_base_getAttr(PVECTOR_AT(vec, item_size - 1));
        item_attr.y = last_item_attr->y + last_item_attr->h + layout->item_vgap;
    }
    else
        item_attr.y = menu_attr->y + layout->border_gap;

    item_attr.x = menu_attr->x + layout->border_gap;
    item_attr.w = str_size->w;
    item_attr.h = str_size->h;

    minu_base_setAttrWith(new_item, &item_attr);
}

void minu_type_selector_horizontal(minu_handle_t me,
                                   minu_item_t *new_item,
                                   minu_size_t *str_size,
                                   minu_icon_t *icon)
{
    minu_base_t item_attr = {0};
    uint16_t item_size = minu_getSize(me);
    minu_layout_t *layout = minu_getLayout(me);
    minu_vector_itme_t *vec = minu_getItems(me);
    minu_base_t *menu_attr = minu_base_getAttr(me);

    if (item_size)
    {
        const minu_base_t *last_item_attr =
            minu_base_getAttr(PVECTOR_AT(vec, item_size - 1));
        item_attr.x = last_item_attr->x + last_item_attr->w + layout->item_hgap;
    }
    else
        item_attr.x = menu_attr->x + layout->border_gap;

    item_attr.y = menu_attr->y + layout->border_gap;
    item_attr.w = str_size->w;
    item_attr.h = str_size->h;

    minu_base_setAttrWith(new_item, &item_attr);
}

minu_handle_t minu_creat(minu_type_cb type,
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
    ret->type_cb = type;
    ret->item_index = 0;
    ret->is_loopItem = 1;
    ret->is_usingAnim = 1;
    ret->cotainer_menu = NULL;

    minu_vector_init(&ret->items);

    minu_setLayoutDefault(ret);
    /* set menu attributes */
    minu_base_set(ret, x, y, w, h);

    return ret;
}

void minu_addSubmenu(minu_handle_t me, char *item_name, minu_handle_t submenu)
{
    minu_size_t str_size = {0};
    minu_item_t *new_item = NULL;

    new_item = minu_item_submenu_new(item_name, submenu);
    if (!new_item)
        return;

    submenu->cotainer_menu = me;
    str_size.w = minu_disp_getStrWidth(item_name);
    str_size.h = minu_disp_getFontHeight();

    /* set the base attributes of the new item */
    me->type_cb(me, new_item, &str_size, NULL);

    minu_vector_push_back(&me->items, new_item);
}

void minu_addCheckBox(minu_handle_t me, char *item_name, bool *flag)
{
    minu_size_t str_size = {0};
    minu_item_t *new_item = NULL;

    new_item = minu_item_checkbox_new(item_name, flag);
    if (!new_item)
        return;

    str_size.w = minu_disp_getStrWidth(item_name);
    str_size.h = minu_disp_getFontHeight();

    /* set the base attributes of the new item */
    me->type_cb(me, new_item, &str_size, NULL);

    minu_vector_push_back(&me->items, new_item);
}

void minu_addVariable(minu_handle_t me,
                      char *item_name,
                      void *var,
                      void (*varToString)(void *var, char *str),
                      minu_item_cb var_cb)
{
    minu_size_t str_size = {0};
    minu_item_t *new_item = NULL;

    new_item = minu_item_variable_new(item_name, var, varToString, var_cb);
    if (!new_item)
        return;

    str_size.w = minu_disp_getStrWidth(item_name);
    str_size.h = minu_disp_getFontHeight();

    /* set the base attributes of the new item */
    me->type_cb(me, new_item, &str_size, NULL);

    minu_vector_push_back(&me->items, new_item);
}

void minu_addWindow(minu_handle_t me, char *item_name, minu_base_t win, void *user_data, minu_item_cb user_cb)
{
    minu_size_t str_size = {0};
    minu_item_t *new_item = NULL;

    new_item = minu_item_window_new(item_name, &win, user_data, user_cb);
    if (!new_item)
        return;

    str_size.w = minu_disp_getStrWidth(item_name);
    str_size.h = minu_disp_getFontHeight();

    /* set the base attributes of the new item */
    me->type_cb(me, new_item, &str_size, NULL);

    minu_vector_push_back(&me->items, new_item);
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

state_t minu_goIn(minu_handle_t *act_menu, minu_event_id_t e)
{
    assert(VECTOR_SIZE((*act_menu)->items) != 0);

    minu_item_t *item = VECTOR_AT((*act_menu)->items, (*act_menu)->item_index);
    minu_item_para_t para = {act_menu, e};
    state_t ret =  minu_item_onEntry(item, &para);

    return ret;
}

/**
 * @brief entry the selected item
 * @return 1: need to transfer state, otherwise no
 */
bool minu_goOut(minu_handle_t *act_menu)
{
    assert(VECTOR_SIZE((*act_menu)->items) != 0);
    bool ret = false;

    minu_t *me = *act_menu;
    if (me->cotainer_menu)
    {
        ret = true;
        *act_menu = me->cotainer_menu;
    }

    return ret;
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
        minu_item_t *now = VECTOR_AT(me->items, i);
        minu_base_t *prev_pos = minu_base_getAttr(VECTOR_AT(me->items, i - 1));
        minu_base_setPos(now, prev_pos->x, prev_pos->y);
    }

    /* this function will modfiy vector size,
     * therefore we need to record `is_end` */
    minu_vector_erase(&me->items, me->item_index);

    if (is_end)
        me->item_index--;
}

void minu_setLayout(minu_handle_t me, minu_layout_t *layout)
{
    me->layout.item_vgap = layout->item_vgap;
    me->layout.border_gap = layout->border_gap;
}

void minu_loopItem_on_off(minu_handle_t me, bool enable)
{
    me->is_loopItem = enable;
}

minu_layout_t *minu_getLayout(minu_handle_t me)
{
    return &me->layout;
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

/**
 * @brief return the current selected item of minu
 */
minu_item_t *minu_getSelectedItem(minu_handle_t me)
{
    return VECTOR_AT(me->items, me->item_index);
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
