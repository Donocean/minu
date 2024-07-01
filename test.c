#include <stddef.h>
#include <stdio.h>
#include "minu.h"
#include "minu_vector.h"

int main(int argc, char *argv[])
{
    minu_t *menu =
        minu_creat("test", MINU_TYPE_SELECTOR_VERTICAL, 0, 0, 120, 64);

    minu_addItem(menu, "tom", NULL, NULL);
    minu_addItem(menu, "bob", NULL, NULL);
    minu_addItem(menu, "jerry", NULL, NULL);
    minu_addItem(menu, "alan", NULL, NULL);
    minu_addItem(menu, "john", NULL, NULL);

    for (int i = 0; i < VECTOR_SIZE(menu->items); i++)
    {
        printf("item[%d] = %p, item[%d].name = %s y = %d\n",
               i,
               &VECTOR_AT(menu->items, i),
               i,
               VECTOR_AT(menu->items, i).name,
               VECTOR_AT((menu->items), i).super.y);
    }

    menu->item_index = 1;
    minu_deleteItem(menu);
    printf("\n\n");
    for (int i = 0; i < VECTOR_SIZE(menu->items); i++)
    {
        printf("item[%d] = %p, item[%d].name = %s y = %d\n",
               i,
               &VECTOR_AT(menu->items, i),
               i,
               VECTOR_AT(menu->items, i).name,
               VECTOR_AT((menu->items), i).super.y);
    }

    return 0;
}
