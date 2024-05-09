#ifndef __MINU_MONITOR_H_
#define __MINU_MONITOR_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "minu.h"
#include "minu_conf.h"
#include "minu_disp.h"

typedef enum
{
    MINU_EVENT_NONE,
    MINU_EVENT_UP,
    MINU_EVENT_DOWN,
    MINU_EVENT_LEFT,
    MINU_EVENT_RIGHT,
    MINU_EVENT_ENTER,
    MINU_EVENT_BACK,
    MINU_EVENT_MAX,
} minu_event_id_t;

typedef struct
{
    uint8_t in;
    uint8_t out;
    uint8_t buf[MINU_EVENT_QUEUE_SIZE]; /* @ref minu_event_id_t */
} minu_event_t;

typedef struct
{
    minu_t      *act_menu;
    minu_disp_t  disp;
    minu_event_t evt;
} minu_monitor_t;

void minu_monitor_focusOn(minu_monitor_t *const me, minu_t *menu);
void minu_monitor_event_post_to(minu_monitor_t *const me, uint8_t val);

void minu_monitor_update(minu_monitor_t *const me);

#ifdef __cplusplus
}
#endif

#endif /* __MINU_MONITOR_H_ */
