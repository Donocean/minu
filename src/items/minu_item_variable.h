#ifndef __MINU_ITEM_VARIABLE_H_
#define __MINU_ITEM_VARIABLE_H_


#ifdef __cplusplus
extern "C" {
#endif

#include "minu_item.h"

minu_item_t *minu_item_variable_new(char *name,
                                    void *var,
                                    void (*varToString)(void *var, char *str),
                                    minu_item_cb var_cb);

#ifdef __cplusplus
}
#endif

#endif /* __MINU_ITEM_VARIABLE_H_ */
