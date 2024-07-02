#ifndef __MINU_H_
#define __MINU_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "minu_base.h"
#include "minu_anim.h"
#include "minu_conf.h"
#include "minu_vector.h"

typedef enum
{
    MINU_TYPE_NONE, /* for user customization */
    MINU_TYPE_SELECTOR_VERTICAL,
    MINU_TYPE_SELECTOR_HORIZONTAL,
    MINU_TYPE_OPTION_VERTICAL,
    MINU_TYPE_OPTION_HORIZONTAL,
} minu_type_t;

typedef struct
{
    union
    {
        uint8_t icon_gap;
        uint8_t item_gap;
    };
    uint8_t border_gap;
    uint8_t bar_width;
} minu_layout_t;

typedef struct _menu minu_t;
struct _menu
{
    minu_base_t super;

    minu_vector_itme_t items;
    const char *title;
    minu_t *cotainer_menu;

    /* for graph */
    minu_layout_t layout;
    minu_base_t selector;
    int16_t movingOffset;

    int16_t item_index; /* -1 means there is nothing in the items vector */
    uint8_t menuType     : 3; /* @ref enum minu_type_t */
    uint8_t is_usingAnim : 1;
    uint8_t is_loopItem  : 1;
};

minu_t *minu_creat(const char *title,
                   uint8_t type,
                   uint16_t x,
                   uint16_t y,
                   uint16_t w,
                   uint16_t h);

void minu_addItem(minu_t *const me,
                  char *name,
                  minu_item_cb cb,
                  void *user_data);

void minu_delete(minu_t *me);
void minu_setLayout(minu_t *const me, minu_layout_t *layout);

static inline void minu_loopItem_on_off(minu_t *const me, bool enable)
{
    me->is_loopItem = enable;
}

void minu_goNext(minu_t *me);
void minu_goPrevious(minu_t *me);
void minu_goIn(minu_t **act_menu);
void minu_goOut(minu_t **act_menu);
void minu_deleteItem(minu_t *me);

#ifdef __cplusplus
}
#endif

#endif /* __MINU_H_ */
