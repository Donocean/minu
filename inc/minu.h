#ifndef __MINU_H_
#define __MINU_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <string.h>
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

typedef struct _menu minu_t;
struct _menu
{
    minu_base_t super;

    minu_vector_itme items;
    minu_t          *cotainer_menu;

    uint8_t item_index;
    uint8_t item_num; /* total number of the items */
    uint8_t menuType; /* @ref enum minu_type_t */
    uint8_t is_usingAnim : 1;
};

#define MENU_GET_ITEM_SUPER(x) &((x).super)

minu_t *minu_creat(void);
void    minu_delete(minu_t *me);

void minu_goNext(minu_t *me);
void minu_goPrevious(minu_t *me);
void minu_goIn(minu_t **act_menu);
void minu_goOut(minu_t **act_menu);

#ifdef __cplusplus
}
#endif

#endif /* __MINU_H_ */
