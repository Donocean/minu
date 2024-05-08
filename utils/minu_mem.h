#ifndef __MINU_MEM_H_
#define __MINU_MEM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

void   minu_mem_free(void *pv);
size_t minu_mem_getFreeHeapSize(void);
void  *minu_mem_malloc(size_t wantedSize);

#ifdef __cplusplus
}
#endif

#endif /* __MINU_MEM_H_ */
