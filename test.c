#include <stddef.h>
#include <stdio.h>
#include "minu.h"
#include "minu_vector.h"

int main(int argc, char *argv[])
{
    minu_t *menu = minu_creat(MINU_TYPE_SELECTOR_VERTICAL, 0, 0, 120, 64);

    minu_addItem(menu, "tom", NULL, NULL);
    minu_addItem(menu, "bob", NULL, NULL);
    minu_addItem(menu, "jerry", NULL, NULL);

    for (int i = 0; i < menu->items.size; i++)
    {
        printf("item[%d] = %p, item[%d].name = %s\n", i, &VECTOR_AT(menu->items, i), i, VECTOR_AT(menu->items, i).name);
    }

    return 0;
}
