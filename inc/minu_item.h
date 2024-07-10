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
typedef struct minu_item_t minu_item_t;
typedef void (*minu_item_cb)(void *para, uint16_t e);
typedef void (*minu_item_process_cb)(minu_item_t *me);

typedef struct
{
    uint16_t width;
    uint16_t height;
    void *icon;
} minu_icon_t;

typedef struct
{
    minu_item_cb item_ops;
    minu_item_process_cb process_ops;
} minu_item_ops_t;

struct minu_item_t
{
    minu_base_t super;

#ifdef MINU_ITEM_USING_CONST_NAME
    const char *name;
#else
    char name[MINU_ITEM_NAME_SIZE];
#endif

    minu_item_iface_t *ops;
    minu_item_cb cb;
    minu_item_appendage_cb appendage_cb;
    union
    {
        void *user_data;
        minu_handle_t sub_menu;
    } u;
};

void minu_item_setName(minu_item_t *const me, char *name);
void minu_item_processAppendage(minu_item_t *me, void *data);
void minu_item_processAppendage(minu_item_t *me, void *data);
void minu_item_set(minu_item_t *const me,
                   char *name,
                   minu_item_cb cb,
                   void *data);

#ifdef __cplusplus
}
#endif

#endif /* __MINU_ITEM_H_ */
