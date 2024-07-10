#ifndef __MINU_ITEM_CHECKBOX_H_
#define __MINU_ITEM_CHECKBOX_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "minu_item.h"

#include <stdbool.h>

minu_item_t *minu_item_checkbox_new(char *name, bool *var_bool);

#ifdef __cplusplus
}
#endif

#endif /* __MINU_ITEM_CHECKBOX_H_ */
