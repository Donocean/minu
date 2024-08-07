#ifndef __MINU_VECTOR_H_
#define __MINU_VECTOR_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "minu_item.h"

typedef struct
{
    uint16_t size;
    uint16_t capacity;
    minu_item_t **items;
} minu_vector_itme_t;

#define VECTOR_AT(v, i)  ((v).items[i])
#define VECTOR_SIZE(v)   ((v).size)
#define VECTOR_BEGIN(v)  ((v).items[0])
#define VECTOR_END(v, i) ((v).items[(v).size - 1])

#define PVECTOR_AT(v, i)  ((v)->items[i])
#define PVECTOR_SIZE(v)   ((v)->size)
#define PVECTOR_BEGIN(v)  ((v)->items[0])
#define PVECTOR_END(v, i) ((v).items[(v)->size - 1])

void minu_vector_init(minu_vector_itme_t *const me);
void minu_vector_push_back(minu_vector_itme_t *const me, minu_item_t *item);
void minu_vector_erase(minu_vector_itme_t *const me, uint16_t index);
void minu_vector_pop_back(minu_vector_itme_t *const me);
void minu_vector_del(minu_vector_itme_t *const me);
uint8_t minu_vector_iter_next(minu_vector_itme_t  *const me, minu_item_t **result);

#ifdef __cplusplus
}
#endif

#endif /* __MINU_VECTOR_H_ */
