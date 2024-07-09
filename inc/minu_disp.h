#ifndef __MINU_OPS_H_
#define __MINU_OPS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "minu_conf.h"

/* Text plotting alignment(reference datum point) */
typedef enum
{
    TL_DATUM, // Top left(default)
    ML_DATUM, // Middle left
    BL_DATUM, // Bottom left
} minu_font_datum_t;

#ifdef MINU_DISPLAY_USE_RGB
/* RGB interface */
void minu_disp_fillScreen(uint32_t color);
void minu_disp_setFontColor(uint32_t color);
void minu_disp_fillRect(int16_t x,
                        int16_t y,
                        uint16_t w,
                        uint16_t h,
                        uint32_t color);

#else

/* MONO interface */
void minu_disp_fillRect(int16_t x, int16_t y, uint16_t w, uint16_t h);

#endif

void minu_disp_fillRectInDiff(int16_t x, int16_t y, int16_t w, int16_t h);
void minu_disp_drawStr(int16_t x, int16_t y, const char *str);
void minu_disp_drawHLine(int16_t x, int16_t y, uint16_t len);
void minu_disp_drawVLine(int16_t x, int16_t y, uint16_t len);
void minu_disp_drawIcon(int16_t x,
                        int16_t y,
                        uint16_t w,
                        uint16_t h,
                        void *icon);
int8_t minu_disp_getFontHeight(void);
uint16_t minu_disp_getStrWidth(char *str);
void minu_disp_setFont(void *font);
void minu_disp_setFontDatum(minu_font_datum_t datum);
void minu_disp_flush(void);
void minu_disp_setWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h);

#ifdef __cplusplus
}
#endif

#endif /* __MINU_OPS_H_ */
