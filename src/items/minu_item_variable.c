#include "minu_item_variable.h"
#include "minu_item.h"
#include "minu_disp.h"
#include "minu_conf.h"
#include MINU_MEM_CUSTOM_INCLUDE

#include <stdint.h>

typedef struct minu_variable_t minu_variable_t;
struct minu_variable_t
{
    minu_item_t super;
    void *var;
    minu_item_cb var_cb;
    void (*varToString)(char *str);
};

static minu_item_status_t variable_onUpdate(minu_item_t *me,
                                            minu_item_para_t *para)
{
    minu_item_status_t ret = MINU_ITEM_STATUS_IGNORE;
    minu_variable_t *item = (minu_variable_t *)me;

    /* item of variable has callbakc fucntion, */
    /* which means we need handle the item until user quit. otherwise just
     * showing the variable */
    if (item->var_cb)
    {
        ret = MINU_ITEM_STATUS_TRANSFER;
        item->var_cb(item->var, para->event);
    }

    return ret;
}

static void variable_draw_appendage(minu_item_t *me, minu_pos_t *target)
{
    char var_str[MINU_ITEM_NAME_SIZE];
    minu_variable_t *item = (minu_variable_t *)me;

    item->varToString(var_str);

    /* minu_disp_drawStr(); */
}

minu_item_t *minu_item_variable_new(char *name,
                                    void *var,
                                    void (*varToString)(char *),
                                    minu_item_cb var_cb)
{
    static minu_item_ops_t ops = {
        .onUpdate = &variable_onUpdate,
        .drawAppendage = &variable_draw_appendage,
    };

    minu_variable_t *new_item = MINU_MEM_CUSTOM_ALLOC(sizeof(minu_variable_t));

    if (!new_item)
        return NULL;

    new_item->var = var;
    new_item->var_cb = var_cb;
    new_item->super.ops = &ops;
    new_item->varToString = varToString;
    minu_item_setName(&new_item->super, name);

    return &new_item->super;
}
