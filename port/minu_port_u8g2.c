#include "minu_port_ops.h"
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

static static void port_u8g2_flush(minu_ops_t *ops);
static static void port_u8g2_drawStr(minu_ops_t *ops, int16_t x, int16_t y, const char *str);
static void port_u8g2_setFontDatum(minu_ops_t *ops, minu_font_datum_t datum);
static void port_u8g2_setFont(minu_ops_t *ops, void *font);
static uint16_t port_u8g2_getStrWidth(minu_ops_t *ops, char *str);
static int8_t port_u8g2_getFontHeight(minu_ops_t *ops);
static void port_u8g2_drawIcon(minu_ops_t *ops, int16_t x, int16_t y, uint16_t w, uint16_t h, void *icon);
static void port_u8g2_fillRect(minu_ops_t *ops, int16_t x, int16_t y, int16_t w, int16_t h);

typedef struct
{
    minu_ops_t base;
    u8g2_t    *u8g2;
} u8g2_disp_t;

/* Note: call this function after the u8g2 has been initialized! */
void minu_port_new_disp_u8g2(u8g2_t *u8g2, minu_monitor_t *moni)
{
    u8g2_disp_t *disp = NULL;

    disp = MINU_MEM_CUSTOM_ALLOC(sizeof(u8g2_disp_t));
    assert(disp != NULL);

    disp->u8g2               = u8g2;
    disp->base.flush         = port_u8g2_flush;
    disp->base.drawStr       = port_u8g2_drawStr;
    disp->base.setFont       = port_u8g2_setFont;
    disp->base.drawIcon      = port_u8g2_drawIcon;
    disp->base.setFontDatum  = port_u8g2_setFontDatum;
    disp->base.getStrWidht   = port_u8g2_getStrWidth;
    disp->base.getFontHeight = port_u8g2_getFontHeight;
    disp->base.fillRect      = port_u8g2_fillRect;

    moni->disp = &disp->base;
}

static void port_u8g2_fillRect(minu_ops_t *ops, int16_t x, int16_t y, int16_t w, int16_t h)
{
    u8g2_disp_t *disp = (u8g2_disp_t *)ops;
    u8g2_SetDrawColor(disp->u8g2, 2);
    u8g2_DrawBox(disp->u8g2, x, y, w, h);
}

static void port_u8g2_drawIcon(minu_ops_t *ops, int16_t x, int16_t y, uint16_t w, uint16_t h, void *icon)
{
    u8g2_disp_t *disp = (u8g2_disp_t *)ops;
    u8g2_DrawXBM(disp->u8g2, x, y, w, h, icon);
}

static int8_t port_u8g2_getFontHeight(minu_ops_t *ops)
{
    u8g2_disp_t *disp = (u8g2_disp_t *)ops;
    int8_t       h    = u8g2_GetAscent(disp->u8g2) - u8g2_GetDescent(disp->u8g2) + 2;
    return h;
}

static uint16_t port_u8g2_getStrWidth(minu_ops_t *ops, char *str)
{
    u8g2_disp_t *disp = (u8g2_disp_t *)ops;
    return (uint16_t)u8g2_GetStrWidth(disp->u8g2, str);
}

static void port_u8g2_setFont(minu_ops_t *ops, void *font)
{
    u8g2_disp_t *disp = (u8g2_disp_t *)ops;
    u8g2_SetFont(disp->u8g2, font);
}

// Set text datum position, @ref enum minu_font_datum_t
static void port_u8g2_setFontDatum(minu_ops_t *ops, minu_font_datum_t datum)
{
    u8g2_disp_t *disp = (u8g2_disp_t *)ops;

    /* for u8g2 we only use the top left Datum */
    (void)datum;
    u8g2_SetFontPosTop(disp->u8g2);
}

static static void port_u8g2_drawStr(minu_ops_t *ops, int16_t x, int16_t y, const char *str)
{
    u8g2_disp_t *disp = (u8g2_disp_t *)ops;
    u8g2_DrawStr(disp->u8g2, x, y, str);
}

static static void port_u8g2_flush(minu_ops_t *ops)
{
    u8g2_disp_t *disp = (u8g2_disp_t *)ops;
    u8g2_SendBuffer(disp->u8g2);
}
