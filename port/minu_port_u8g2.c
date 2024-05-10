#include "minu_port.h"
#include "u8g2.h"
#include <assert.h>
#include <stdint.h>
#include "minu_conf.h"
#include MINU_MEM_CUSTOM_INCLUDE
#include "minu_monitor.h"

#ifdef MINU_DISPLAY_USE_RGB
#undef MINU_DISPLAY_USE_RGB
#define MINU_DISPLAY_USE_MONO
#endif

static u8g2_t *port_u8g2;

/* Note: call this function after the u8g2 has been initialized! */
void minu_port_new_disp_u8g2(void *u8g2_obj)
{
    port_u8g2 = u8g2_obj;

    port_setFont((void *)u8g2_font_HelvetiPixel_tr);
    port_setFontDatum(TL_DATUM);
}

void port_fillRectInDiff(int16_t x, int16_t y, int16_t w, int16_t h)
{
    u8g2_SetDrawColor(port_u8g2, 2);
    u8g2_DrawRBox(port_u8g2, x, y, w, h, 2);
}

void port_drawIcon(int16_t x, int16_t y, uint16_t w, uint16_t h, void *icon)
{
    u8g2_DrawXBM(port_u8g2, x, y, w, h, icon);
}

int8_t port_getFontHeight(void)
{
    int8_t h = u8g2_GetAscent(port_u8g2) - u8g2_GetDescent(port_u8g2) + 2;
    return h;
}

uint16_t port_getStrWidth(char *str)
{
    return (uint16_t)u8g2_GetStrWidth(port_u8g2, str);
}

void port_setFont(void *font)
{
    u8g2_SetFont(port_u8g2, font);
}

// Set text datum position, @ref enum minu_font_datum_t
void port_setFontDatum(minu_font_datum_t datum)
{
    /* for u8g2 we only use the top left Datum */
    (void)datum;
    u8g2_SetFontPosTop(port_u8g2);
}

void port_drawStr(int16_t x, int16_t y, const char *str)
{
    u8g2_DrawStr(port_u8g2, x, y, str);
}

void port_flush(void)
{
    u8g2_SendBuffer(port_u8g2);
    u8g2_ClearBuffer(port_u8g2);
}
