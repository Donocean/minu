#include "minu_item_variable.h"
#include "esp_log.h"
#include "minu.h"
#include "minu_item.h"
#include "minu_disp.h"
#include "minu_conf.h"
#include "minu_types.h"
#include "minu_viewer.h"
#include MINU_MEM_CUSTOM_INCLUDE

#include <stdint.h>

typedef struct minu_variable_t minu_variable_t;
struct minu_variable_t
{
    minu_item_t super;
    void *var;
    minu_item_cb var_cb;
    void (*varToString)(void *var, char *str);
};

static state_t variable_onEntry(minu_item_t *me, minu_item_para_t *para)
{
    state_t ret = STATUS_IGNORED;
    minu_variable_t *item = (minu_variable_t *)me;

    /* item of variable has callbakc fucntion, */
    /* which means we need handle the item until user quit. otherwise just
     * showing the variable */
    if (item->var_cb)
    {
        ret = STATUS_TRAN;
        item->var_cb(item->var, para->event);
    }

    return ret;
}

static void variable_onHandling(minu_item_t *me, minu_event_id_t e)
{
    minu_variable_t *item = (minu_variable_t *)me;

    if (item->var_cb)
        item->var_cb(item->var, e);
}

static void variable_draw_appendage(minu_item_t *me,
                                    void *menu,
                                    minu_pos_t *target)
{
    char var_str[MINU_ITEM_NAME_SIZE];
    minu_variable_t *item = (minu_variable_t *)me;
    const minu_base_t *menu_attr = minu_base_getAttr(menu);
    const minu_layout_t *layout = minu_getLayout(menu);

    item->varToString(item->var, var_str);
    uint16_t str_w = minu_disp_getStrWidth(var_str);

    /* recalculate the variable x position in the screen */
    target->x = menu_attr->w - str_w - layout->bar_width - layout->border_gap;

    minu_disp_drawStr(target->x, target->y, var_str);
}

minu_item_t *minu_item_variable_new(char *name,
                                    void *var,
                                    void (*varToString)(void *var, char *str),
                                    minu_item_cb var_cb)
{
    const static minu_item_ops_t ops = {
        .onEntry = &variable_onEntry,
        .onHandling = &variable_onHandling,
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
