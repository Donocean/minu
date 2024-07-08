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
    uint8_t item_gap;
    uint8_t border_gap;
    uint8_t bar_width;
} minu_layout_t;

typedef struct minu_t *minu_handle_t;

minu_handle_t minu_creat(uint8_t type,
                         const char *title,
                         uint16_t x,
                         uint16_t y,
                         uint16_t w,
                         uint16_t h);

void minu_addItem(const minu_handle_t me,
                  minu_item_type_t type,
                  char *name,
                  minu_item_cb cb,
                  void *user_data);

void minu_delete(minu_handle_t me);
void minu_setLayout(minu_handle_t me, minu_layout_t *layout);
void minu_loopItem_on_off(const minu_handle_t me, bool enable);

void minu_goNext(minu_handle_t me);
void minu_goPrevious(minu_handle_t me);
void minu_goIn(minu_handle_t *act_menu);
void minu_goOut(minu_handle_t *act_menu);
void minu_deleteItem(minu_handle_t me);

minu_layout_t *minu_getLayout(minu_handle_t me);
uint16_t minu_getSize(minu_handle_t me);
int16_t minu_getIndex(minu_handle_t me);
minu_item_t *minu_getSelectedItem(minu_handle_t me);
minu_vector_itme_t *minu_getItems(minu_handle_t me);

#ifdef __cplusplus
}
#endif

#endif /* __MINU_H_ */
