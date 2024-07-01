#ifndef __MINU_VIEWER_H_
#define __MINU_VIEWER_H_

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
    MINU_EVENT_QUIT,
    MINU_EVENT_REFRESH,
    MINU_EVENT_DELETE,
    MINU_EVENT_MAX,
} minu_event_id_t;

typedef struct minu_viewer_t *minu_viewer_handle_t;

minu_viewer_handle_t minu_viewer_create(minu_t *menu);

void minu_viewer_update(minu_viewer_handle_t me);
void minu_viewer_event_post_to(minu_viewer_handle_t me, minu_event_id_t e);

#ifdef __cplusplus
}
#endif

#endif /* __MINU_VIEWER_H_ */
