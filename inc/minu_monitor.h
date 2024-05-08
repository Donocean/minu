#ifndef __MINU_MONITOR_H_
#define __MINU_MONITOR_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "minu.h"
#include "minu_ops.h"
#include "minu_conf.h"

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

typedef enum
{
    TL_DATUM, // Top left (default)
    TC_DATUM, // Top centre
    TR_DATUM, // Top right
    ML_DATUM, // Middle left
    CL_DATUM, // Centre left, same as above
    MC_DATUM, // Middle centre
    CC_DATUM, // Centre centre, same as above
    MR_DATUM, // Middle right
    CR_DATUM, // Centre right, same as above
    BL_DATUM, // Bottom left
    BC_DATUM, // Bottom centre
    BR_DATUM, // Bottom right
} minu_font_datum_t;

typedef struct
{
    uint8_t in;
    uint8_t out;
    uint8_t buf[MINU_EVENT_QUEUE_SIZE]; /* @ref minu_event_id_t */
} minu_event_t;

typedef struct
{
    minu_t      *act_menu;
    minu_ops_t   ops;
    minu_event_t evt;
    uint8_t      font_datum; /* the text plotting alignment(reference datum point) @ref minu_event_id_t */
} minu_monitor_t;

void minu_monitor_update(minu_monitor_t *const me);
void minu_monitor_init(minu_monitor_t *const me, minu_t *menu, minu_ops_t *ops);
void minu_monitor_event_post_to(minu_monitor_t *const me, uint8_t val);

static inline void minu_handle_set_font_datum_alignment(minu_monitor_t *const me, minu_font_datum_t datum)
{
    me->font_datum = datum;
}

#ifdef __cplusplus
}
#endif

#endif /* __MINU_MONITOR_H_ */
