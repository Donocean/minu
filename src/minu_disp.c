#include "minu_disp.h"
#include "minu_port.h"
#include MINU_MEM_CUSTOM_INCLUDE
#include <stddef.h>

static minu_ops_t *g_ops;

void minu_disp_set(void *disp)
{
    if (g_ops)
        MINU_MEM_CUSTOM_FREE(g_ops);

    g_ops = (minu_ops_t *)disp;
}

#ifdef MINU_DISPLAY_USE_RGB

void minu_disp_fillScreen(uint32_t color)
{
    g_ops->fillScreen(color);
}

void minu_disp_setFontColor(uint32_t color)
{
    g_ops->setFontColor(color);
}

void minu_disp_fillRect(int16_t x, int16_t y, uint16_t w, uint16_t h, uint32_t color)
{
    g_ops->fillRect(x, y, w, h, color);
}

#else

void minu_disp_fillRectInDiff(int16_t x, int16_t y, int16_t w, int16_t h)
{
    g_ops->fillRectInDiff(x, y, w, h);
}

#endif

void minu_disp_drawStr(int16_t x, int16_t y, const char *str)
{
    g_ops->drawStr(x, y, str);
}

void minu_disp_drawIcon(int16_t x, int16_t y, uint16_t w, uint16_t h, void *icon)
{
    g_ops->drawIcon(x, y, w, h, icon);
}

int8_t minu_disp_getFontHeight(void)
{
    return g_ops->getFontHeight();
}

uint16_t minu_disp_getStrWidth(char *str)
{
    return g_ops->getStrWidth(str);
}

void minu_disp_setFont(void *font)
{
    g_ops->setFont(font);
}

// Set text datum position, @ref enum minu_font_datum_t
void minu_disp_setFontDatum(minu_font_datum_t datum)
{
    g_ops->setFontDatum(datum);
}

/* This function is used to send the buffer to the screen. Don't need to set it if you don't use it */
void minu_disp_flush(void)
{
    g_ops->flush();
}
