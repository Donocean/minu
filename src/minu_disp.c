#include "minu_disp.h"
#include "minu_port.h"
#include <assert.h>
#include MINU_MEM_CUSTOM_INCLUDE

static minu_ops_t *g_ops;

void minu_disp_set(void *disp)
{
    if (g_ops)
        MINU_MEM_CUSTOM_FREE(g_ops);

    g_ops = (minu_ops_t *)disp;
}

void minu_disp_fillRect(int16_t x, int16_t y, uint16_t w, uint16_t h)
{
    assert(g_ops != NULL);
    g_ops->fillRect(x, y, w, h);
}

void minu_disp_drawRect(int16_t x, int16_t y, uint16_t w, uint16_t h)
{
    assert(g_ops != NULL);
    g_ops->drawRect(x, y, w, h);
}

void minu_disp_fillRectInDiff(int16_t x, int16_t y, int16_t w, int16_t h)
{
    assert(g_ops != NULL);
    g_ops->fillRectInDiff(x, y, w, h);
}

void minu_disp_drawStr(int16_t x, int16_t y, const char *str)
{
    assert(g_ops != NULL);
    g_ops->drawStr(x, y, str);
}

void minu_disp_drawIcon(int16_t x, int16_t y, uint16_t w, uint16_t h, void *icon)
{
    assert(g_ops != NULL);
    g_ops->drawIcon(x, y, w, h, icon);
}

int8_t minu_disp_getFontHeight(void)
{
    assert(g_ops != NULL);
    return g_ops->getFontHeight();
}

uint16_t minu_disp_getStrWidth(const char *str)
{
    assert(g_ops != NULL);
    return g_ops->getStrWidth(str);
}

void minu_disp_setFont(void *font)
{
    assert(g_ops != NULL);
    g_ops->setFont(font);
}

// Set text datum position, @ref enum minu_font_datum_t
void minu_disp_setFontDatum(minu_font_datum_t datum)
{
    assert(g_ops != NULL);
    g_ops->setFontDatum(datum);
}

void minu_disp_beginDrawing(void)
{
    assert(g_ops != NULL);
    if (g_ops->beginDrawing)
        g_ops->beginDrawing();
}

/* This function is used to send the buffer to the screen. Don't need to set it
 * if you don't use it */
void minu_disp_endDrawing(void)
{
    assert(g_ops != NULL);
    g_ops->endDrawing();
}

void minu_disp_setWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
    assert(g_ops != NULL);
    return g_ops->setWindow(x, y, w, h);
}

void minu_disp_drawPixel(uint16_t x, uint16_t y)
{
    assert(g_ops != NULL);
    return g_ops->drawPixel(x, y);
}

void minu_disp_drawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{
    assert(g_ops != NULL);
    return g_ops->drawLine(x1, y1, x2, y2);
}
