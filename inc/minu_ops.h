#ifndef __MINU_OPS_H_
#define __MINU_OPS_H_

#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

#include "minu.h"
#include "minu_conf.h"

typedef struct
{
#ifdef MINU_DISPLAY_USE_RGB
    void (*fillScreen)(uint32_t color);
    void (*setFontColor)(uint32_t color);
    void (*fillRect)(int16_t x, int16_t y, int16_t w, int16_t h, uint32_t color);

#else
    void (*fillRect)(int16_t x, int16_t y, int16_t w, int16_t h);
#endif

    int8_t (*getFontHeight)(void);
    int8_t (*getFontWeight)(void);
    void (*setCursor)(int16_t x, int16_t y);
    void (*setFontSize)(uint8_t fontSize);
    /* this function is used to send the buffer to the screen. if you don't have it, don't set it at all. */
    void (*flush)(void);
} minu_ops_t;

#ifdef __cplusplus
}
#endif

#endif /* __MINU_OPS_H_ */
