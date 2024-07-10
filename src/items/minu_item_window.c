#include "minu_item_window.h"
#include "minu_item.h"
#include "minu_conf.h"
#include MINU_MEM_CUSTOM_INCLUDE

typedef struct minu_window_t minu_window_t;
struct minu_window_t
{
    minu_item_t super;
    
};

minu_item_t *minu_item_window_new(void)
{
    minu_window_t *new_item = MINU_MEM_CUSTOM_ALLOC(sizeof(minu_window_t));

    if (!new_item)
        return NULL;

    return &new_item->super;
}


