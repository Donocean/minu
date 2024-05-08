#ifndef __MINU_VECTOR_H_
#define __MINU_VECTOR_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "minu_item.h"

typedef struct
{
    uint16_t     size;
    uint16_t     capacity;
    minu_item_t *items;
} minu_vector_itme_;

#define VECTOR_AT(v, i)  ((v).items[i])
#define VECTOR_SIZE(v)   ((v).size)

void minu_vector_init(minu_vector_itme_ *const me);
void minu_vector_push_back(minu_vector_itme_ *const me, minu_item_t *item);
void minu_vector_del(minu_vector_itme_ *const me);

#ifdef __cplusplus
}
#endif

#endif /* __MINU_VECTOR_H_ */
