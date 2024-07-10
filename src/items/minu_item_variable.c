#include "minu_item_variable.h"
#include "minu_item.h"
#include "minu_conf.h"
#include MINU_MEM_CUSTOM_INCLUDE

typedef struct minu_variable_t minu_variable_t;
struct minu_variable_t
{
    minu_item_t super;
    void *var;
    minu_item_cb var_cb;
};

minu_item_t *minu_item_variable_new(char *name, void *var, minu_item_cb var_cb)
{
    minu_variable_t *new_item = MINU_MEM_CUSTOM_ALLOC(sizeof(minu_variable_t));

    if (!new_item)
        return NULL;

    new_item->var = var;
    new_item->var_cb = var_cb;
    minu_item_setName(&new_item->super, name);

    return &new_item->super;
}


