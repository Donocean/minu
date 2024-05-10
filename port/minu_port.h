#ifndef __MINU_PORT_H_
#define __MINU_PORT_H_

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

void     port_fillRectInDiff(int16_t x, int16_t y, int16_t w, int16_t h);
void     port_drawIcon(int16_t x, int16_t y, uint16_t w, uint16_t h, void *icon);
int8_t   port_getFontHeight(void);
uint16_t port_getStrWidth(char *str);
void     port_setFont(void *font);
void     port_setFontDatum(minu_font_datum_t datum);
void     port_drawStr(int16_t x, int16_t y, const char *str);
void     port_flush(void);

/* Note: Please call these function after the specific UI library has been initialized! */
void minu_port_new_disp_u8g2(void *u8g2_obj);
void minu_port_new_disp_tft_espi(void *tft_obj);

#ifdef __cplusplus
}
#endif

#endif /* __MINU_PORT_H_ */
