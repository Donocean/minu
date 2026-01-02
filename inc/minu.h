#ifndef __MINU_H_
#define __MINU_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "minu_item.h"
#include "minu_vector.h"
#include "minu_disp.h"
#include "minu_port.h"
#include "minu_anim.h"
#include "minu_viewer.h"

typedef struct
{
    uint8_t item_vgap;
    uint8_t item_hgap;
    uint8_t border_gap;
    uint8_t bar_width;
} minu_layout_t;

typedef void (*minu_type_cb)(minu_handle_t me,
                             minu_item_t *new_item,
                             minu_size_t *str_size,
                             minu_icon_t *icon);
void minu_type_selector_vertical(minu_handle_t me,
                                 minu_item_t *new_item,
                                 minu_size_t *str_size,
                                 minu_icon_t *icon);
void minu_type_selector_horizontal(minu_handle_t me,
                                   minu_item_t *new_item,
                                   minu_size_t *str_size,
                                   minu_icon_t *icon);

/* minu types */
#define MINU_TYPE_NONE                NULL /* for user customizing */
#define MINU_TYPE_SELECTOR_VERTICAL   &minu_type_selector_vertical
#define MINU_TYPE_SELECTOR_HORIZONTAL &minu_type_selector_horizontal
#define MINU_TYPE_OPTION_VERTICAL
#define MINU_TYPE_OPTION_HORIZONTAL

minu_handle_t minu_create(minu_type_cb type,
                          const char *title,
                          uint16_t x,
                          uint16_t y,
                          uint16_t w,
                          uint16_t h);
void minu_addSubmenuItem(minu_handle_t me, const char *item_name, minu_handle_t submenu);
void minu_addCheckBoxItem(minu_handle_t me, const char *item_name, bool *flag);
void minu_addVariableItem(minu_handle_t me,
                          const char *item_name,
                          void *var,
                          const char *fmt,
                          minu_item_cb var_cb);
void minu_addWindowItem(minu_handle_t me,
                        const char *item_name,
                        void (*draw_cb)(void *para),
                        minu_item_cb user_cb,
                        void *user_data);

void minu_goNext(minu_handle_t me);
void minu_goPrevious(minu_handle_t me);
state_t minu_goIn(minu_handle_t *act_menu, minu_event_id_t e);
bool minu_goOut(minu_handle_t *act_menu);
void minu_deleteItem(minu_handle_t me);

void minu_setLayout(minu_handle_t me, minu_layout_t *layout);
void minu_loopItem_on_off(minu_handle_t me, bool enable);
minu_layout_t *minu_getLayout(minu_handle_t me);
uint16_t minu_getSize(minu_handle_t me);
int16_t minu_getIndex(minu_handle_t me);
minu_pos_t *minu_getOffset(minu_handle_t me);
minu_attr_t minu_getOffTarget(minu_handle_t me);
minu_item_t *minu_getSelectedItem(minu_handle_t me);
minu_vector_itme_t *minu_getItems(minu_handle_t me);
void minu_delete(minu_handle_t me);

#ifdef __cplusplus
}
#endif

#endif /* __MINU_H_ */
