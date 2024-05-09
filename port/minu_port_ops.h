#ifndef __MINU_PORT_OPS_H_
#define __MINU_PORT_OPS_H_

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

typedef struct minu_ops_t minu_ops_t;
struct minu_ops_t
{
#ifdef MINU_DISPLAY_USE_RGB
    void (*fillScreen)(minu_ops_t *ops, uint32_t color);
    void (*setFontColor)(minu_ops_t *ops, uint32_t color);
    void (*fillRect)(minu_ops_t *ops, int16_t x, int16_t y, uint16_t w, uint16_t h, uint32_t color);

#else
    void (*fillRect)(minu_ops_t *ops, int16_t x, int16_t y, int16_t w, int16_t h);
#endif

    void (*drawStr)(minu_ops_t *ops, int16_t x, int16_t y, const char *str);
    void (*drawIcon)(minu_ops_t *ops, int16_t x, int16_t y, uint16_t w, uint16_t h, void *icon);

    int8_t (*getFontHeight)(minu_ops_t *ops);
    uint16_t (*getStrWidth)(minu_ops_t *ops, char *str);
    void (*setFont)(minu_ops_t *ops, void *font);

    // Set text datum position, @ref enum minu_font_datum_t
    void (*setFontDatum)(minu_ops_t *ops, minu_font_datum_t datum);

    /* This function is used to send the buffer to the screen. Don't need to set it if you don't use it */
    void (*flush)(minu_ops_t *ops);
};

#ifdef __cplusplus
}
#endif

#endif /* __MINU_PORT_OPS_H_ */
