#ifndef __MINU_OPS_H_
#define __MINU_OPS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "minu_port_ops.h"

typedef struct minu_ops_t *minu_disp_t;

#ifdef MINU_DISPLAY_USE_RGB

void minu_disp_fillScreen(const minu_disp_t me, uint32_t color);
void minu_disp_setFontColor(const minu_disp_t me, uint32_t color);
void minu_disp_fillRect(const minu_disp_t me, int16_t x, int16_t y, uint16_t w, uint16_t h, uint32_t color);

#else

void minu_disp_fillRect(const minu_disp_t me, int16_t x, int16_t y, int16_t w, int16_t h);

#endif

void     minu_disp_drawStr(const minu_disp_t me, int16_t x, int16_t y, const char *str);
void     minu_disp_drawIcon(const minu_disp_t me, int16_t x, int16_t y, uint16_t w, uint16_t h, void *icon);
int8_t   minu_disp_getFontHeight(const minu_disp_t me);
uint16_t minu_disp_getStrWidth(const minu_disp_t me, char *str);
void     minu_disp_setFont(const minu_disp_t me, void *font);
void     minu_disp_setFontDatum(const minu_disp_t me, minu_font_datum_t datum);
void     minu_disp_flush(const minu_disp_t me);

#ifdef __cplusplus
}
#endif

#endif /* __MINU_OPS_H_ */
