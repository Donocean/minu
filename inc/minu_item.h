#ifndef __MINU_ITEM_H_
#define __MINU_ITEM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <string.h>
#include "minu_conf.h"
#include "minu_base.h"

typedef struct minu_t *minu_handle_t;
typedef void (*minu_item_cb)(void *para);

typedef enum
{
    MINU_ITEM_TYPE_SUBMENU,
    MINU_ITEM_TYPE_VARIABLE,
    MINU_ITEM_TYPE_CHECKBOX,
    MINU_ITEM_TYPE_POPWINDOW,

} minu_item_type_t;

typedef struct
{
    uint16_t width;
    uint16_t height;
    void *icon;
} minu_icon_t;

typedef struct _menu_item
{
    minu_base_t super;

#ifdef MINU_ITEM_USING_CONST_NAME
    const char *name;
#else
    char name[MINU_ITEM_NAME_SIZE];
#endif

    minu_item_cb cb;
    void *user_data;
    minu_handle_t sub_menu;
} minu_item_t;

void minu_item_setName(minu_item_t *const me, char *name);
void minu_item_set(minu_item_t *const me,
                   char *name,
                   minu_item_cb cb,
                   void *user_data);

#ifdef __cplusplus
}
#endif

#endif /* __MINU_ITEM_H_ */
