#include "minu_item_checkbox.h"
#include "minu_item.h"
#include "minu_conf.h"
#include MINU_MEM_CUSTOM_INCLUDE

typedef struct minu_checkbox_t minu_checkbox_t;
struct minu_checkbox_t
{
    minu_item_t super;
    bool *var_bool;
};

minu_item_t *minu_item_checkbox_new(char *name, bool *var_bool)
{
    minu_checkbox_t *new_item = MINU_MEM_CUSTOM_ALLOC(sizeof(minu_checkbox_t));

    if (!new_item)
        return NULL;

    new_item->var_bool = var_bool;
    minu_item_setName(&new_item->super, name);

    return &new_item->super;
}

