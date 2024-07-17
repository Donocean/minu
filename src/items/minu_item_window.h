#ifndef __MINU_ITEM_WINDOW_H_
#define __MINU_ITEM_WINDOW_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "minu_item.h"

minu_item_t *minu_item_window_new(char *name,
                                  void (*draw_cb)(void *para),
                                  minu_item_cb user_cb,
                                  void *user_data);

#ifdef __cplusplus
}
#endif

#endif /* __MINU_ITEM_WINDOW_H_ */
