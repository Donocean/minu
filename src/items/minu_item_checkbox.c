#include "minu_item_checkbox.h"
#include "minu.h"
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

static void checkbox_onEntry(minu_item_t *me, minu_item_para_t *para)
{
    minu_checkbox_t *item = (minu_checkbox_t *)me;

    (void)para;

    if (item->var_bool)
        *item->var_bool = !(*item->var_bool);

    ESP_LOGI("", "b: %d", *item->var_bool);
}

static void checkbox_draw_appendage(minu_item_t *me,
                                    void *menu,
                                    minu_pos_t *target)
{
    minu_checkbox_t *item = (minu_checkbox_t *)me;
    const minu_base_t *item_attr = minu_base_getAttr(me);
    const minu_base_t *menu_attr = minu_base_getAttr(menu);
    const minu_layout_t *layout = minu_getLayout(menu);

    target->x =
        menu_attr->w - layout->bar_width - item_attr->h - layout->border_gap;

    minu_disp_drawRect(target->x, target->y, item_attr->h, item_attr->h);

    if (*item->var_bool)
    {
        int gap = 3;
        minu_disp_fillRect(target->x + gap,
                           target->y + gap,
                           item_attr->h - gap * 2,
                           item_attr->h - gap * 2);
    }
}

minu_item_t *minu_item_checkbox_new(char *name, bool *var_bool)
{
    static minu_item_ops_t ops = {
        .onEntry = &checkbox_onEntry,
        .drawAppendage = &checkbox_draw_appendage,
    };
    minu_checkbox_t *new_item = MINU_MEM_CUSTOM_ALLOC(sizeof(minu_checkbox_t));

    if (!new_item)
        return NULL;

    new_item->super.ops = &ops;
    new_item->var_bool = var_bool;
    minu_item_setName(&new_item->super, name);

    return &new_item->super;
}
