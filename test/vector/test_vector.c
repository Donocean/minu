#include "minu_item.h"
#include "minu_vector.h" /* Code Under Test (CUT) */
#include "et.h"          /* ET: Embedded Test */
#include <stdio.h>
#include <stdlib.h>

minu_vector_itme_t vec;

void setup(void)
{
    minu_vector_init(&vec);
}

void teardown(void)
{
}

/* test group -------------------------------------------------*/
TEST_GROUP("Basic")
{
    TEST("CUT test (passing)")
    {
        minu_item_t *new_item0 = malloc(sizeof(minu_item_t));
        minu_item_t *new_item1 = malloc(sizeof(minu_item_t));
        minu_item_t *new_item2 = malloc(sizeof(minu_item_t));
        minu_item_t *new_item3 = malloc(sizeof(minu_item_t));
        minu_item_t *new_item4 = malloc(sizeof(minu_item_t));
        minu_item_t *new_item5 = malloc(sizeof(minu_item_t));
        minu_item_t *new_item6 = malloc(sizeof(minu_item_t));
        printf("\n     item: %p\n", new_item0);
        printf("vec.items: %p\n", vec.items);
        minu_vector_push_back(&vec, new_item0);
        minu_vector_push_back(&vec, new_item1);
        minu_vector_push_back(&vec, new_item2);
        minu_vector_push_back(&vec, new_item3);
        minu_vector_push_back(&vec, new_item4);
        minu_vector_push_back(&vec, new_item5);
        minu_vector_push_back(&vec, new_item6);

        VERIFY(vec.items[0] == new_item0);
        VERIFY(vec.items[1] == new_item1);
        VERIFY(vec.items[2] == new_item2);
        VERIFY(vec.items[3] == new_item3);
        VERIFY(vec.size == 7);
    }

    TEST("struct test")
    {
        struct test
        {
            int a;
            int b;
        };

        struct test *t = malloc(sizeof(struct test));
        struct test *pt = &t->a;

        VERIFY(t == pt);
    }

} /* TEST_GROUP() */

/* dependencies for the CUT -----------------------------------*/
