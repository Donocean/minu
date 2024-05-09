#include "minu_disp.h"
#include <assert.h>
#include <stddef.h>

#ifdef MINU_DISPLAY_USE_RGB

void minu_disp_fillScreen(const minu_disp_t me, uint32_t color)
{
    assert(me != NULL);
    me->fillScreen(me, color);
}

void minu_disp_setFontColor(const minu_disp_t me, uint32_t color)
{
    assert(me != NULL);
    me->setFontColor(me, color);
}

void minu_disp_fillRect(const minu_disp_t me, int16_t x, int16_t y, uint16_t w, uint16_t h, uint32_t color)
{
    assert(me != NULL);
    me->fillRect(me, x, y, w, h, color);
}

#else

void minu_disp_fillRect(const minu_disp_t me, int16_t x, int16_t y, int16_t w, int16_t h)
{
    assert(me != NULL);
    me->fillRect(me, x, y, w, h);
}

#endif

void minu_disp_drawStr(const minu_disp_t me, int16_t x, int16_t y, const char *str)
{
    assert(me != NULL);
    me->drawStr(me, x, y, str);
}

void minu_disp_drawIcon(const minu_disp_t me, int16_t x, int16_t y, uint16_t w, uint16_t h, void *icon)
{
    assert(me != NULL);
    me->drawIcon(me, x, y, w, h, icon);
}

int8_t minu_disp_getFontHeight(const minu_disp_t me)
{
    assert(me != NULL);
    return me->getFontHeight(me);
}

uint16_t minu_disp_getStrWidth(const minu_disp_t me, char *str)
{
    assert(me != NULL);
    return me->getStrWidth(me, str);
}

void minu_disp_setFont(const minu_disp_t me, void *font)
{
    assert(me != NULL);
    me->setFont(me, font);
}

// Set text datum position, @ref enum minu_font_datum_t
void minu_disp_setFontDatum(const minu_disp_t me, minu_font_datum_t datum)
{
    assert(me != NULL);
    me->setFontDatum(me, datum);
}

/* This function is used to send the buffer to the screen. Don't need to set it if you don't use it */
void minu_disp_flush(const minu_disp_t me)
{
    assert(me != NULL);
    if (me->flush)
        me->flush(me);
}
