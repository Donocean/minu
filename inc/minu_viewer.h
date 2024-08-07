#ifndef __MINU_VIEWER_H_
#define __MINU_VIEWER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "minu_types.h"
#include "minu_anim.h"

minu_viewer_handle_t minu_viewer_create(minu_handle_t menu);
void minu_viewer_update(minu_viewer_handle_t me);
void minu_viewer_event_post_to(minu_viewer_handle_t me, minu_event_id_t e);
void minu_viewer_setSelAnimPath(minu_viewer_handle_t me, minu_easingPath_t pat);
void minu_viewer_setSelDuration(minu_viewer_handle_t me, uint16_t duration);

#ifdef __cplusplus
}
#endif

#endif /* __MINU_VIEWER_H_ */
