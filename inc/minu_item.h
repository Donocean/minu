#ifndef __MINU_ITEM_H_
#define __MINU_ITEM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "minu_base.h"
#include "minu_conf.h"

typedef struct minu_item_t minu_item_t;
typedef void (*minu_item_cb)(void *para, uint16_t e);

typedef struct
{
    void *icon;
    uint16_t w;
    uint16_t h;
} minu_icon_t;

typedef struct
{
    void *act_menu;
    uint8_t event;
} minu_item_para_t;

typedef enum
{
    MINU_ITEM_STATUS_IGNORE,
    MINU_ITEM_STATUS_REFRESH,
    MINU_ITEM_STATUS_TRANSFER,
} minu_item_status_t;

typedef struct
{
    minu_item_status_t (*onUpdate)(minu_item_t *me, minu_item_para_t *para);
    void (*drawAppendage)(minu_item_t *me, minu_pos_t *target);
} minu_item_ops_t;

struct minu_item_t
{
    minu_base_t super;

#ifdef MINU_ITEM_USING_CONST_NAME
    const char *name;
#else
    char name[MINU_ITEM_NAME_SIZE];
#endif

    minu_item_ops_t *ops;
};

void minu_item_setName(minu_item_t *const me, char *name);
minu_item_status_t minu_item_onUpdate(minu_item_t *me, minu_item_para_t *para);
void minu_item_drawAppendage(minu_item_t *me, minu_pos_t *target);

#ifdef __cplusplus
}
#endif

#endif /* __MINU_ITEM_H_ */
