#include "minu_port.h"
#include "minu_disp.h"
#include "u8g2.h"
#include <assert.h>
#include <stdint.h>
#include "minu_conf.h"

#ifdef MINU_DISPLAY_USE_RGB
#undef MINU_DISPLAY_USE_RGB
#define MINU_DISPLAY_USE_MONO
#endif

static void port_setWindow(int16_t x, int16_t y, uint16_t w, uint16_t h);
extern void minu_disp_set(void *disp);
static void port_flush(void);
static void port_drawStr(int16_t x, int16_t y, const char *str);
static void port_setFontDatum(uint8_t datum);
static void port_setFont(void *font);
static uint16_t port_getStrWidth(char *str);
static int8_t port_getFontHeight(void);
static void port_drawIcon(int16_t x,
                          int16_t y,
                          uint16_t w,
                          uint16_t h,
                          void *icon);
static void port_fillRect(int16_t x, int16_t y, uint16_t w, uint16_t h);
static void port_drawRect(int16_t x, int16_t y, uint16_t w, uint16_t h);
static void port_fillRectInDiff(int16_t x, int16_t y, int16_t w, int16_t h);
static void port_drawHLine(int16_t x, int16_t y, uint16_t len);
static void port_drawVLine(int16_t x, int16_t y, uint16_t len);
static void port_drawPixel(int16_t x, int16_t y);
static void port_drawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2);

static u8g2_t *port_u8g2;
static minu_ops_t disp_ops = {
    .flush = port_flush,
    .drawStr = port_drawStr,
    .setFont = port_setFont,
    .drawIcon = port_drawIcon,
    .setFontDatum = port_setFontDatum,
    .fillRect = port_fillRect,
    .drawRect = port_drawRect,
    .fillRectInDiff = port_fillRectInDiff,
    .getStrWidth = port_getStrWidth,
    .getFontHeight = port_getFontHeight,
    .drawHLine = port_drawHLine,
    .drawVLine = port_drawVLine,
    .setWindow = port_setWindow,
    .drawPixel = port_drawPixel,
    .drawLine = port_drawLine,
};

/* Note: call this function after the u8g2 has been initialized! */
void minu_port_new_disp_u8g2(void *u8g2_obj)
{
    port_u8g2 = u8g2_obj;
    port_setFontDatum(TL_DATUM);
    minu_disp_set(&disp_ops);
}

static void port_drawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{
    u8g2_DrawLine(port_u8g2, x1, y1, x2, y2);
}

static void port_drawPixel(int16_t x, int16_t y)
{
    u8g2_DrawPixel(port_u8g2, x, y);
}

static void port_setWindow(int16_t x, int16_t y, uint16_t w, uint16_t h)
{
    u8g2_SetClipWindow(port_u8g2, x, y, x + w, y + h);
}

static void port_fillRect(int16_t x, int16_t y, uint16_t w, uint16_t h)
{
    u8g2_DrawBox(port_u8g2, x, y, w, h);
}

static void port_drawRect(int16_t x, int16_t y, uint16_t w, uint16_t h)
{
    u8g2_DrawFrame(port_u8g2, x, y, w, h);
}

static void port_fillRectInDiff(int16_t x, int16_t y, int16_t w, int16_t h)
{
    u8g2_SetDrawColor(port_u8g2, 2);
    u8g2_DrawRBox(port_u8g2, x, y, w, h, 2);
    u8g2_SetDrawColor(port_u8g2, 1);
}

static void port_drawIcon(int16_t x,
                          int16_t y,
                          uint16_t w,
                          uint16_t h,
                          void *icon)
{
    u8g2_DrawXBM(port_u8g2, x, y, w, h, icon);
}

static int8_t port_getFontHeight(void)
{
    int8_t h = u8g2_GetAscent(port_u8g2) - u8g2_GetDescent(port_u8g2) + 2;
    return h;
}

static uint16_t port_getStrWidth(char *str)
{
    return (uint16_t)u8g2_GetStrWidth(port_u8g2, str);
}

static void port_setFont(void *font)
{
    u8g2_SetFont(port_u8g2, font);
}

// Set text datum position, @ref enum minu_font_datum_t
static void port_setFontDatum(uint8_t datum)
{
    /* for u8g2 we only use the top left Datum */
    (void)datum;
    u8g2_SetFontPosTop(port_u8g2);
}

static void port_drawStr(int16_t x, int16_t y, const char *str)
{
    u8g2_DrawStr(port_u8g2, x, y, str);
}

static void port_drawHLine(int16_t x, int16_t y, uint16_t len)
{
    u8g2_DrawHLine(port_u8g2, x, y, len);
}

static void port_drawVLine(int16_t x, int16_t y, uint16_t len)
{
    u8g2_DrawVLine(port_u8g2, x, y, len);
}

static void port_flush(void)
{
    u8g2_SendBuffer(port_u8g2);
    u8g2_ClearBuffer(port_u8g2);
}
