#include <stdint.h>

#include "minu_mem.h"
#include "minu_conf.h"

#ifndef MEIU_MEM_CUSTOM

/*
   内存管理方案的空闲块链表是以内存块起始地址大小排序,
   内存地址小的在前，地址大的在后. 因为heap_4 的内存合并算法，
   在释放内存的时候，假如相邻的两个空闲内存块在地址上是连续的
   那么就可以合并为一个内存块
*/

#define ENTER_CRITICAL()
#define EXIT_CRITICAL()

#define MINIMUM_BLOCK_SIZE ((size_t)(heapSTRUCT_SIZE << 1))
#define BITS_PER_BYTE      ((size_t)8)

#if (MINU_MEM_APPLICATION_ALLOCATED_HEAP == 1)
extern uint8_t heap[MINU_MEM_TOTAL_HEAP_SIZE];
#else
static uint8_t heap[MINU_MEM_TOTAL_HEAP_SIZE];
#endif // MINU_MEM_APPLICATION_ALLOCATED_HEAP

typedef struct A_BLOCK_LINK
{
    /* point to the next free memory block address */
    struct A_BLOCK_LINK *nextFreeBlock;
    /* free memory block size */
    size_t blockSize;
} blockLink_t;

/*-----------------------------------------------------------*/

static void heapInit(void);
static void insertBlockIntoFreeList(blockLink_t *blockToInsert);

/*-----------------------------------------------------------*/

/* aligned struct size.
 * for instance:
 *      - structure size = 12 byte
 *      - aligned size = 16
 *      heapSTRUCT_SIZE = (12 + 15) & ~0x0f = 16
 */
static const uint16_t heapSTRUCT_SIZE =
    ((sizeof(blockLink_t) + (MINU_MEM_ALIGNMENT - 1)) &
     ~MINU_MEM_ALIGNMENT_MASK);
/* when a block is used, the struct member xBlockSize
 * of the using block highest bit mark as 1, otherwise 0 */
static const size_t blockAllocatedBit =
    ((size_t)1) << ((sizeof(size_t) * BITS_PER_BYTE) - 1);
/* record how much memory is unused, but doesn't include memory fragmentation */
static size_t remainingBytes;
/* record the minimum remaining bytes in the total time.
 * just for monitoring worst-case scenario using this algorithm */
static size_t minimumRemainingBytes;

static blockLink_t head, *ptail;

void *minu_mem_malloc(size_t wantedSize)
{
    void *ret = NULL;

    ENTER_CRITICAL();
    {
        if (ptail == NULL)
            heapInit();

        if ((wantedSize & blockAllocatedBit) == 0)
        {
            if (wantedSize > 0)
            {
                /* allocate additional struct size for memory management */
                wantedSize += heapSTRUCT_SIZE;

                /* align the allocation size */
                if ((wantedSize & MINU_MEM_ALIGNMENT_MASK) != 0x00)
                {
                    wantedSize += (MINU_MEM_ALIGNMENT -
                                   (wantedSize & MINU_MEM_ALIGNMENT_MASK));
                }
            }

            if ((wantedSize > 0) && (wantedSize <= remainingBytes))
            {
                blockLink_t *block, *previousBlock;

                previousBlock = &head;
                block = head.nextFreeBlock;
                while ((block->blockSize < wantedSize) &&
                       (block->nextFreeBlock != NULL))
                {
                    previousBlock = block;
                    block = block->nextFreeBlock;
                }

                if (block != ptail)
                {
                    ret = (void *)(((uint8_t *)previousBlock->nextFreeBlock) +
                                   heapSTRUCT_SIZE);

                    /* remove the fitting block from the free list */
                    previousBlock->nextFreeBlock = block->nextFreeBlock;

                    /* the fitting block can be splited */
                    if ((block->blockSize - wantedSize) > MINIMUM_BLOCK_SIZE)
                    {
                        blockLink_t *newBlockLink =
                            (void *)(((uint8_t *)block) + wantedSize);

                        /* split fitting memory block to smaller one */
                        newBlockLink->blockSize = block->blockSize - wantedSize;
                        block->blockSize = wantedSize;

                        /* insert the splited free block into the free block
                         * list */
                        insertBlockIntoFreeList(newBlockLink);
                    }

                    remainingBytes -= block->blockSize;
                    if (remainingBytes < minimumRemainingBytes)
                        minimumRemainingBytes = remainingBytes;

                    /* mark as used */
                    block->blockSize |= blockAllocatedBit;
                    block->nextFreeBlock = NULL;
                }
            }
        }
    }
    EXIT_CRITICAL();

    return ret;
}

void minu_mem_free(void *pv)
{
    blockLink_t *plink;
    uint8_t *p = (uint8_t *)pv;

    if (pv != NULL)
    {
        p -= heapSTRUCT_SIZE;
        plink = (void *)p;

        /* if block is used */
        if ((plink->blockSize & blockAllocatedBit) != 0)
        {
            if (plink->nextFreeBlock == NULL)
            {
                /* mark block as free */
                plink->blockSize &= ~blockAllocatedBit;

                ENTER_CRITICAL();
                {
                    remainingBytes += plink->blockSize;
                    insertBlockIntoFreeList(((blockLink_t *)plink));
                }
                EXIT_CRITICAL();
            }
        }
    }
}

size_t minu_mem_getFreeHeapSize(void)
{
    return remainingBytes;
}

size_t minu_getMinimumEverFreeHeapSize(void)
{
    return minimumRemainingBytes;
}

static void heapInit(void)
{
    size_t address;
    blockLink_t *firstFreeBlock;
    uint8_t *alignedHeap;
    size_t totalHeapSize = MINU_MEM_TOTAL_HEAP_SIZE;

    /* 获取堆栈起始地址 */
    address = (size_t)heap;

    /* 如果起始地址没有地址对齐，则进行内存对齐操作 */
    if ((address & MINU_MEM_ALIGNMENT_MASK) != 0)
    {
        address += (MINU_MEM_ALIGNMENT - 1);
        address &= ~((size_t)MINU_MEM_ALIGNMENT_MASK);

        /* 减去因对齐而浪费掉的字节 */
        totalHeapSize -= address - (size_t)heap;
    }

    /* 保存对齐后的地址 */
    alignedHeap = (uint8_t *)address;

    /* initialize list header and tail */
    head.nextFreeBlock = (void *)alignedHeap;
    head.blockSize = (size_t)0;

    /*ptail的值为内存尾部向前偏移一个 BlockLink_t 结构体大小，偏移出来的这个
      BlockLink_t 就是 ptail
      尾部只是一个标记，当遍历空闲链表到这里的时候，表示已经没有可用的内存块了，
      所以 ptail 的 pxNextFreeBlock 成员变量为
      NULL，与heap_2方案不同的是链表的尾部节点不是静态的，而是放在了内存的最后。*/
    address = ((size_t)alignedHeap) + totalHeapSize;
    address -= heapSTRUCT_SIZE;
    address &= ~((size_t)MINU_MEM_ALIGNMENT_MASK);
    ptail = (void *)address;
    ptail->blockSize = 0;
    ptail->nextFreeBlock = NULL;

    /* initialize the first free block */
    firstFreeBlock = (void *)alignedHeap;
    firstFreeBlock->blockSize = address - (size_t)firstFreeBlock;
    firstFreeBlock->nextFreeBlock = ptail;

    /* initialize counter value */
    minimumRemainingBytes = firstFreeBlock->blockSize;
    remainingBytes = firstFreeBlock->blockSize;
}

/* 插入空闲内存块链表的时候会通过合并算法将可以合并成大内存块的相邻内存块进行合并
 */
static void insertBlockIntoFreeList(blockLink_t *blockToInsert)
{
    uint8_t *p;
    blockLink_t *iterator;

    /* find the free memory block beside the block to be inserted */
    for (iterator = &head; iterator->nextFreeBlock < blockToInsert;
         iterator = iterator->nextFreeBlock)
    {
        /* do nothing here */
    }

    /* 如果前一个内存块的尾部地址恰好是 blockToInsert 的头部地址，
       那代表这两个内存块是连续的，可以合并那么就把blockToInsert
       合并到该内存块中 */
    p = (uint8_t *)iterator;
    if ((p + iterator->blockSize) == (uint8_t *)blockToInsert)
    {
        /*将 blockToInsert 合并入 iterator 中。 iterator
         * 的大小就是本身大小再加上 blockToInsert 的大小 */
        iterator->blockSize += blockToInsert->blockSize;
        blockToInsert = iterator;
    }

    /* 同理，再判断 blockToInsert
       是否和后面的空闲内存相邻，如果blockToInsert的尾部地址
        是下一个内存块的头部地址，那么也是说明这连个内存块是连续的，可以合并 */
    p = (uint8_t *)blockToInsert;
    if ((p + blockToInsert->blockSize) == (uint8_t *)iterator->nextFreeBlock)
    {
        /* 还要判断 blockToInsert 的下一个内存块是不是尾部节点ptail，
        为什么呢？因为尾部节点就是放在系统管理的内存块最后的地址上，而head不是放在堆里的，
        所以这里要判断一下。如果不是 ptail，并且还连续的，
        那么就将后面的内存合入blockToInsert， 并用 blockToInsert
        代替该内存在链表中的位置， blockToInsert
        的大小就是本身大小再加上下一个内存块的大小 */
        if (iterator->nextFreeBlock != ptail)
        {
            /* Form one big block from the two blocks. */
            blockToInsert->blockSize += iterator->nextFreeBlock->blockSize;
            blockToInsert->nextFreeBlock =
                iterator->nextFreeBlock->nextFreeBlock;
        }
        else
        {
            /* 如果 blockToInsert 的下一个内存块是 ptail，那就不能合并，
                 将内存块节点的成员变量 pxNextFreeBlock 指向 ptail。 */
            blockToInsert->nextFreeBlock = ptail;
        }
    }
    else
    {
        /* 如果 blockToInsert 与后面的内存块不相邻，那么只能插入链表了 */
        blockToInsert->nextFreeBlock = iterator->nextFreeBlock;
    }

    /* 判断下前面是否已经合并了，如果合并了，就不用再更新链表了，
         否则就更新一下与前一个内存块的链表连接关系。 */
    if (iterator != blockToInsert)
        iterator->nextFreeBlock = blockToInsert;
}

#endif
