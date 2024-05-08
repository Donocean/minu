#include <stddef.h>
#include <stdio.h>
#include "minu.h"
#include "minu_vector.h"

int main(int argc, char *argv[])
{
    minu_t *menu = minu_creat();

    for (int i = 0; i < menu->item_num; i++)
    {
        printf("item[%d] = %p, item[%d].name = %s\n", i, &VECTOR_AT(menu->items, i), i, VECTOR_AT(menu->items, i).name);
    }

    return 0;
}
