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

/* core */
void minu_disp_beginDrawing(void);
void minu_disp_endDrawing(void);
void minu_disp_setWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h);

/* draw */
void minu_disp_drawRect(int16_t x, int16_t y, uint16_t w, uint16_t h);
void minu_disp_fillRect(int16_t x, int16_t y, uint16_t w, uint16_t h);
void minu_disp_fillRectInDiff(int16_t x, int16_t y, int16_t w, int16_t h);
void minu_disp_drawStr(int16_t x, int16_t y, const char *str);
void minu_disp_drawIcon(int16_t x, int16_t y, uint16_t w, uint16_t h, void *icon);
void minu_disp_drawPixel(uint16_t x, uint16_t y);
void minu_disp_drawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2);

/* font */
void minu_disp_setFont(void *font);
void minu_disp_setFontDatum(minu_font_datum_t datum);
int8_t minu_disp_getFontHeight(void);
uint16_t minu_disp_getStrWidth(const char *str);

#ifdef __cplusplus
}
#endif

#endif /* __MINU_OPS_H_ */
