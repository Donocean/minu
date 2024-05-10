#include "minu_disp.h"
#include <stddef.h>

#ifdef MINU_DISPLAY_USE_RGB

void minu_disp_fillScreen(uint32_t color)
{
    port_fillScreen(color);
}

void minu_disp_setFontColor(uint32_t color)
{
    port_setFontColor(color);
}

void minu_disp_fillRect(int16_t x, int16_t y, uint16_t w, uint16_t h, uint32_t color)
{
    port_fillRect(x, y, w, h, color);
}

#else

void minu_disp_fillRectInDiff(int16_t x, int16_t y, int16_t w, int16_t h)
{
    port_fillRectInDiff(x, y, w, h);
}

#endif

void minu_disp_drawStr(int16_t x, int16_t y, const char *str)
{
    port_drawStr(x, y, str);
}

void minu_disp_drawIcon(int16_t x, int16_t y, uint16_t w, uint16_t h, void *icon)
{
    port_drawIcon(x, y, w, h, icon);
}

int8_t minu_disp_getFontHeight(void)
{
    return port_getFontHeight();
}

uint16_t minu_disp_getStrWidth(char *str)
{
    return port_getStrWidth(str);
}

void minu_disp_setFont(void *font)
{
    port_setFont(font);
}

// Set text datum position, @ref enum minu_font_datum_t
void minu_disp_setFontDatum(minu_font_datum_t datum)
{
    port_setFontDatum(datum);
}

/* This function is used to send the buffer to the screen. Don't need to set it if you don't use it */
void minu_disp_flush(void)
{
    port_flush();
}
