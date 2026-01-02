#include "minu_item_submenu.h"
#include "minu.h"
#include "minu_item.h"
#include "minu_disp.h"
#include "minu_conf.h"
#include "minu_types.h"
#include <stdbool.h>
#include MINU_MEM_CUSTOM_INCLUDE

typedef struct minu_submenu_t minu_submenu_t;
struct minu_submenu_t
{
    minu_item_t super;
    minu_handle_t submenu;
};

static state_t submenu_onEntry(minu_item_t *me, minu_item_para_t *para)
{
    minu_submenu_t *item = (minu_submenu_t *)me;

    (void)para;
    if (item->submenu)
        *para->act_menu = item->submenu;

    return STATUS_REFRESH;
}

static void submenu_draw_appendage(minu_item_t *me, void *menu, minu_pos_t *target)
{
    char *tag = "->";
    minu_attr_t menu_attr = minu_base_getAttr(menu);
    const minu_layout_t *layout = minu_getLayout(menu);

    uint16_t str_w = minu_disp_getStrWidth(tag);

    /* recalculate the variable x position in the screen */
    target->x = menu_attr.w - str_w - layout->bar_width - layout->border_gap;

    minu_disp_drawStr(target->x, target->y, tag);
}

minu_item_t *minu_item_submenu_new(const char *name, minu_handle_t submenu)
{
    const static minu_item_ops_t ops = {
        .onEntry = &submenu_onEntry,
        .onHandling = NULL,
        .drawAppendage = &submenu_draw_appendage,
    };
    minu_submenu_t *new_item = MINU_MEM_CUSTOM_ALLOC(sizeof(minu_submenu_t));

    if (!new_item)
        return NULL;

    new_item->super.ops = &ops;
    new_item->submenu = submenu;
    minu_item_setName(&new_item->super, name);

    return &new_item->super;
}
