
#ifndef __MINU_TYPE_H_
#define __MINU_TYPE_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    STATUS_IGNORED,
    STATUS_REFRESH,
    STATUS_TRAN,
} state_t;

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

typedef struct minu_t *minu_handle_t;
typedef struct minu_viewer_t *minu_viewer_handle_t;


#ifdef __cplusplus
}
#endif

#endif /* __MINU_TYPE_H_ */
