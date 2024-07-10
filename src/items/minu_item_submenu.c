#include "minu_item_submenu.h"
#include "minu_item.h"
#include "minu_conf.h"
#include MINU_MEM_CUSTOM_INCLUDE

typedef struct minu_submenu_t minu_submenu_t;
struct minu_submenu_t
{
    minu_item_t super;
    minu_handle_t submenu;
};

minu_item_t *minu_item_submenu_new(char *name, minu_handle_t submenu)
{
    minu_submenu_t *new_item = MINU_MEM_CUSTOM_ALLOC(sizeof(minu_submenu_t));

    if (!new_item)
        return NULL;

    new_item->submenu = submenu;
    minu_item_setName(&new_item->super, name);

    return &new_item->super;
}
