#ifndef __U8G2_PORT_H_
#define __U8G2_PORT_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "u8g2.h"
#include "minu_base.h"
#include <stdint.h>

// SCREEN_WIDTH must equal window width
#define SCREEN_WIDTH 128

typedef struct
{
    minu_base_t super;
    uint16_t *buf;
    uint16_t size;
    uint16_t min;
    uint16_t max;
    uint16_t in;
} wave_window_t;

u8g2_t *ui_u8g2_new(void);

void minu_wave_init(wave_window_t *me,
                    uint8_t x,
                    uint8_t y,
                    uint8_t w,
                    uint8_t h,
                    uint16_t *buf,
                    uint16_t size);

void minu_wave_drawFullWave(wave_window_t *me);
void minu_wave_drawDynamicWave(wave_window_t *me, uint16_t data);
void minu_wave_drawBackground(wave_window_t *me, uint8_t dot_width);

#ifdef __cplusplus
}
#endif

#endif //__U8G2_PORT_H_
