#include "minu_mem.h"
#include "minu_conf.h"
#include <stddef.h>
#include <stdint.h>

#ifndef MENU_MEM_CUSTOM

#define ENTER_CRITICAL()
#define EXIT_CRITICAL()

/* A few bytes might be lost to byte aligning the heap start address. */
#define ADJUSTED_HEAP_SIZE (MINU_MEM_TOTAL_HEAP_SIZE - MINU_MEM_ALIGNMENT)

static void heapInit(void);

#if (MINU_MEM_APPLICATION_ALLOCATED_HEAP == 1)
extern uint8_t minu_heap[MINU_MEM_TOTAL_HEAP_SIZE];
#else
static uint8_t minu_heap[MINU_MEM_TOTAL_HEAP_SIZE];
#endif

typedef struct A_BLOCK_LINK
{
    struct A_BLOCK_LINK *nextFreeBlock; /* pointer to the next free memory block */
    size_t               blockSize;     /* free memory size */
} blockLink_t;

/* aligned struct size.
 * for instance: structure size = 12 byte: aligned size = 16: 12 + 15=23, 27 & ~0x15=8
 */
static const uint16_t heapSTRUCT_SIZE = ((sizeof(blockLink_t) + (MINU_MEM_ALIGNMENT - 1)) & ~MINU_MEM_ALIGNMENT_MASK);
static size_t         remainingBytes  = ADJUSTED_HEAP_SIZE;
static blockLink_t    head, tail;

#define heapMINIMUM_BLOCK_SIZE ((size_t)(heapSTRUCT_SIZE * 2))

/* Sort every blocks from small to big */
#define prvInsertBlockIntoFreeList(blockToInsert)                                                                  \
    do                                                                                                             \
    {                                                                                                              \
        blockLink_t *iterator;                                                                                     \
        size_t       blockSize;                                                                                    \
        blockSize = blockToInsert->blockSize;                                                                      \
        for (iterator = &head; iterator->nextFreeBlock->blockSize < blockSize; iterator = iterator->nextFreeBlock) \
        {                                                                                                          \
        }                                                                                                          \
        blockToInsert->nextFreeBlock = iterator->nextFreeBlock;                                                    \
        iterator->nextFreeBlock      = blockToInsert;                                                              \
    } while (0)

void *minu_mem_malloc(size_t wantedSize)
{
    void          *ret            = NULL;
    static uint8_t is_initialized = 0;

    ENTER_CRITICAL();
    {
        if (is_initialized == 0)
        {
            heapInit();
            is_initialized = 1;
        }

        if (wantedSize > 0)
        {
            /* allocate additional struct size for memory management */
            wantedSize += heapSTRUCT_SIZE;

            /* align the allocation size */
            if ((wantedSize & MINU_MEM_ALIGNMENT_MASK) != 0)
            {
                wantedSize += (MINU_MEM_ALIGNMENT - (wantedSize & MINU_MEM_ALIGNMENT_MASK));
            }
        }

        if ((wantedSize > 0) && (wantedSize < ADJUSTED_HEAP_SIZE))
        {
            blockLink_t *block, *previousBlock;

            previousBlock = &head;
            block         = head.nextFreeBlock;
            while ((block->blockSize < wantedSize) && (block->nextFreeBlock != NULL))
            {
                previousBlock = block;
                block         = block->nextFreeBlock;
            }

            /* if find the free block */
            if (block != &tail)
            {
                ret = (void *)(((uint8_t *)previousBlock->nextFreeBlock) + heapSTRUCT_SIZE);

                /* remove the fitting block from the free list */
                previousBlock->nextFreeBlock = block->nextFreeBlock;

                /* if the fitting block can be splited */
                if ((block->blockSize - wantedSize) > heapMINIMUM_BLOCK_SIZE)
                {
                    blockLink_t *newBlock;

                    /* split current memory block to smaller one */
                    newBlock            = (void *)(((uint8_t *)block) + wantedSize);
                    newBlock->blockSize = block->blockSize - wantedSize;
                    block->blockSize    = wantedSize;

                    /* add the splited free block into the free block list */
                    prvInsertBlockIntoFreeList((newBlock));
                }
                remainingBytes -= block->blockSize;
            }
        }
    }
    EXIT_CRITICAL();

    return ret;
}

void minu_mem_free(void *pv)
{
    uint8_t     *p = (uint8_t *)pv;
    blockLink_t *pxLink;

    if (pv != NULL)
    {
        p      -= heapSTRUCT_SIZE;
        pxLink  = (void *)p;

        ENTER_CRITICAL();
        {
            /* add the free block into the free block list */
            prvInsertBlockIntoFreeList(((blockLink_t *)pxLink));
            remainingBytes += pxLink->blockSize;
        }
        EXIT_CRITICAL();
    }
}

size_t minu_mem_getFreeHeapSize(void)
{
    return remainingBytes;
}

static void heapInit(void)
{
    blockLink_t *firstFreeBlock;
    uint8_t     *alignedHeap;

    /* make sure the address is aligned */
    alignedHeap = (uint8_t *)(((size_t)&minu_heap[MINU_MEM_ALIGNMENT]) & (~((size_t)MINU_MEM_ALIGNMENT_MASK)));
    /* initialize list header and tail */
    head.nextFreeBlock = (void *)alignedHeap;
    head.blockSize     = (size_t)0;
    tail.blockSize     = ADJUSTED_HEAP_SIZE;
    tail.nextFreeBlock = NULL;

    /* initialize the first free block */
    firstFreeBlock                = (void *)alignedHeap;
    firstFreeBlock->blockSize     = ADJUSTED_HEAP_SIZE;
    firstFreeBlock->nextFreeBlock = &tail;
}

#endif /* MENU_MEM_CUSTOM */
