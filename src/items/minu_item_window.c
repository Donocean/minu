#include "minu_item_window.h"
#include "minu.h"
#include "minu_base.h"
#include "minu_item.h"
#include "minu_disp.h"
#include "minu_conf.h"
#include MINU_MEM_CUSTOM_INCLUDE

typedef struct minu_window_t minu_window_t;
struct minu_window_t
{
    minu_item_t super;
    void (*draw_cb)(void *para);
    minu_item_cb user_cb;
    void *user_data;
};

static state_t window_onEntry(minu_item_t *me, minu_item_para_t *para)
{
    minu_window_t *item = (minu_window_t *)me;

    if (item->user_cb)
        item->user_cb(item->user_data, para->event);

    return STATUS_TRAN;
}

static void window_onHandling(minu_item_t *me, minu_event_id_t e)
{
    minu_window_t *item = (minu_window_t *)me;

    if (item->user_cb)
        item->user_cb(item->user_data, e);
}

static void window_onUpdate(minu_item_t *me)
{
    minu_window_t *item = (minu_window_t *)me;

    if (item->draw_cb)
        item->draw_cb(item->user_data);
}

static void window_draw_appendage(minu_item_t *me,
                                  void *menu,
                                  minu_pos_t *target)
{
    char *tag = "[win]";
    minu_attr_t menu_attr = minu_base_getAttr(menu);
    const minu_layout_t *layout = minu_getLayout(menu);

    uint16_t str_w = minu_disp_getStrWidth(tag);

    /* recalculate the variable x position in the screen */
    target->x = menu_attr.w - str_w - layout->bar_width - layout->border_gap;

    minu_disp_drawStr(target->x, target->y, tag);
}

minu_item_t *minu_item_window_new(char *name,
                                  void (*draw_cb)(void *para),
                                  minu_item_cb user_cb,
                                  void *user_data)
{
    const static minu_item_ops_t ops = {
        .onEntry = &window_onEntry,
        .onUpdate = &window_onUpdate,
        .onHandling = &window_onHandling,
        .drawAppendage = &window_draw_appendage,
    };

    minu_window_t *new_item = MINU_MEM_CUSTOM_ALLOC(sizeof(minu_window_t));
    if (!new_item)
        return NULL;

    new_item->super.ops = &ops;
    new_item->draw_cb = draw_cb;
    new_item->user_cb = user_cb;
    new_item->user_data = user_data;
    minu_item_setName(&new_item->super, name);

    return &new_item->super;
}
