#include "minu_item_checkbox.h"
#include "driver/i2c.h"
#include "minu_base.h"
#include "minu_disp.h"
#include "minu_item.h"
#include "minu.h"
#include "minu_conf.h"
#include MINU_MEM_CUSTOM_INCLUDE

#include <stdio.h>
#include "esp_log.h"

typedef struct minu_checkbox_t minu_checkbox_t;
struct minu_checkbox_t
{
    minu_item_t super;
    bool *var_bool;
};

static minu_item_status_t checkbox_onUpdate(minu_item_t *me,
                                            minu_item_para_t *para)
{
    minu_checkbox_t *item = (minu_checkbox_t *)me;
    minu_item_status_t ret = MINU_ITEM_STATUS_IGNORE;

    (void)para;

    if (item->var_bool)
        *item->var_bool = !(*item->var_bool);

    ESP_LOGI("", "b: %d", *item->var_bool);

    return ret;
}

static void checkbox_draw_appendage(minu_item_t *me, minu_pos_t *target)
{
    minu_checkbox_t *item = (minu_checkbox_t *)me;
    const minu_base_t *item_attr = minu_base_getAttr(me);

    minu_disp_drawRect(100, target->y, item_attr->h, item_attr->h);

    if (*item->var_bool)
    {
        int gap = 3;
        minu_disp_fillRect(100 + gap,
                           target->y + gap,
                           item_attr->h - gap * 2,
                           item_attr->h - gap * 2);
    }
}

minu_item_t *minu_item_checkbox_new(char *name, bool *var_bool)
{
    static minu_item_ops_t ops = {.onUpdate = &checkbox_onUpdate,
                                  .drawAppendage = &checkbox_draw_appendage};
    minu_checkbox_t *new_item = MINU_MEM_CUSTOM_ALLOC(sizeof(minu_checkbox_t));

    if (!new_item)
        return NULL;

    new_item->super.ops = &ops;
    new_item->var_bool = var_bool;
    minu_item_setName(&new_item->super, name);

    return &new_item->super;
}
