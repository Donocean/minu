#ifndef __MINU_PORT_H_
#define __MINU_PORT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "minu_conf.h"

/* Monochrome interface */
typedef struct
{
    void (*fillRect)(int16_t x, int16_t y, uint16_t w, uint16_t h);
    void (*drawRect)(int16_t x, int16_t y, uint16_t w, uint16_t h);

    void (*fillRectInDiff)(int16_t x, int16_t y, int16_t w, int16_t h);
    void (*drawIcon)(int16_t x, int16_t y, uint16_t w, uint16_t h, void *icon);
    int8_t (*getFontHeight)(void);
    uint16_t (*getStrWidth)(const char *str);
    void (*setFont)(void *font);
    void (*setFontDatum)(uint8_t datum);
    void (*drawPixel)(int16_t x, int16_t y);

    void (*drawLine)(int16_t x1, int16_t y1, int16_t x2, int16_t y2);
    void (*drawStr)(int16_t x, int16_t y, const char *str);
    void (*setWindow)(int16_t x, int16_t y, uint16_t w, uint16_t h);
    void (*beginDrawing)(void); /* optional */
    void (*endDrawing)(void);
} minu_ops_t;

/* Note: Please call these function after the specific UI library has been
 * initialized! */
void minu_port_new_disp_u8g2(void *u8g2_obj);
void minu_port_new_disp_raylib(void *raylib_obj);
void minu_port_new_disp_tft_espi(void *tft_obj);

#ifdef __cplusplus
}
#endif

#endif /* __MINU_PORT_H_ */
