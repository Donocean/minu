#ifndef __MINU_CONF_H_
#define __MINU_CONF_H_

#ifdef __cplusplus
extern "C" {
#endif

/*========================* SCREEN TYPE SETTINGS *=========================*/
/* chose your color gamut by uncomment the macro below */
/* #define MINU_DISPLAY_USE_RGB */
#define MINU_DISPLAY_USE_MONO

/*========================* ITEM NAME SETTINGS *=========================*/
/* save space in ram but can't modify item's name */
// #define MINU_ITEM_USING_CONST_NAME

#ifndef MINU_ITEM_USING_CONST_NAME
/* item's name max length */
#define MINU_ITEM_NAME_SIZE 32
#endif

/*========================* ITEM NAME SETTINGS *=========================*/
/* must be pow of 2 */
#define MINU_EVENT_QUEUE_SIZE 32

/*========================* MEMORY SETTINGS *=========================*/
/* #define MEIU_MEM_CUSTOM */

#ifdef MEIU_MEM_CUSTOM
/* user need to complete the macro below if MENU_MEM_CUSTOM was enabled */
#define MINU_MEM_CUSTOM_INCLUDE <stdlib.h>
#define MINU_MEM_CUSTOM_ALLOC   malloc
#define MINU_MEM_CUSTOM_FREE    free

#else
#define MINU_MEM_CUSTOM_INCLUDE "minu_mem.h"
#define MINU_MEM_CUSTOM_ALLOC   minu_mem_malloc
#define MINU_MEM_CUSTOM_FREE    minu_mem_free

#define MINU_MEM_TOTAL_HEAP_SIZE            ((size_t)(2 * 1024))
#define MINU_MEM_ALIGNMENT                  4
#define MINU_MEM_APPLICATION_ALLOCATED_HEAP 0

#if MINU_MEM_ALIGNMENT == 8
#define MINU_MEM_ALIGNMENT_MASK (0x0007)
#endif

#if MINU_MEM_ALIGNMENT == 4
#define MINU_MEM_ALIGNMENT_MASK (0x0003)
#endif

#if MINU_MEM_ALIGNMENT == 2
#define MINU_MEM_ALIGNMENT_MASK (0x0001)
#endif

#if MINU_MEM_ALIGNMENT == 1
#define MINU_MEM_ALIGNMENT_MASK (0x0000)
#endif
#endif

/*========================* TICK SETTINGS *=========================*/

#ifdef MENU_TICK_CUSTOM
#define MENU_TICK_CUSTOM_SYS_TIME_EXPR milis()
#endif

/*========================*  SETTINGS *=========================*/

#ifdef __cplusplus
}
#endif
#endif /* __MINU_CONF_H_ */
