#ifndef __MINU_ITEM_H_
#define __MINU_ITEM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "minu_conf.h"
#include "minu_base.h"
#include "minu_types.h"

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
    minu_handle_t *act_menu;
    uint8_t event;
} minu_item_para_t;

typedef struct
{
    state_t (*onEntry)(minu_item_t *me, minu_item_para_t *para);
    void (*onHandling)(minu_item_t *me, minu_event_id_t e);
    void (*onUpdate)(minu_item_t *me);
    void (*drawAppendage)(minu_item_t *me, void *menu, minu_pos_t *target);
} minu_item_ops_t;

struct minu_item_t
{
    minu_base_t super;

#ifdef MINU_ITEM_USING_CONST_NAME
    const char *name;
#else
    char name[MINU_ITEM_NAME_SIZE];
#endif

    const minu_item_ops_t *ops;
};

void minu_item_setName(minu_item_t *const me, const char *name);
void minu_item_onUpdate(minu_item_t *me);
state_t minu_item_onEntry(minu_item_t *me, minu_item_para_t *para);
void minu_item_onHandling(minu_item_t *me, minu_event_id_t e);
void minu_item_drawAppendage(minu_item_t *me, void *menu, minu_pos_t *target);

#ifdef __cplusplus
}
#endif

#endif /* __MINU_ITEM_H_ */
