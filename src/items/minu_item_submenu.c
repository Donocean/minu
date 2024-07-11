#include "minu_item_submenu.h"
#include "minu.h"
#include "minu_item.h"
#include "minu_conf.h"
#include <stdbool.h>
#include MINU_MEM_CUSTOM_INCLUDE

typedef struct minu_submenu_t minu_submenu_t;
struct minu_submenu_t
{
    minu_item_t super;
    minu_handle_t submenu;
};

static minu_item_status_t submenu_onUpdate(minu_item_t *me, minu_item_para_t *para)
{
    minu_item_status_t ret = MINU_ITEM_STATUS_IGNORE;
    minu_submenu_t *item = (minu_submenu_t *)me;
    minu_handle_t *act_menu = (minu_handle_t *)para->act_menu;

    if (item->submenu)
    {
        ret = MINU_ITEM_STATUS_REFRESH;
        *act_menu = item->submenu;
    }

    return ret;
}

minu_item_t *minu_item_submenu_new(char *name, minu_handle_t submenu)
{
    static minu_item_ops_t ops = {
        .onUpdate = &submenu_onUpdate,
    };
    minu_submenu_t *new_item = MINU_MEM_CUSTOM_ALLOC(sizeof(minu_submenu_t));

    if (!new_item)
        return NULL;

    new_item->super.ops = &ops;
    new_item->submenu = submenu;
    minu_item_setName(&new_item->super, name);

    return &new_item->super;
}
