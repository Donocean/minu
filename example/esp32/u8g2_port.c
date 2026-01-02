#include "u8g2_port.h"
#include "driver/i2c.h"
#include "esp_log.h"
#include "minu_base.h"
#include "minu_disp.h"
#include "u8g2.h"

/* #define OLED_ADDR 0x7A */
#define OLED_ADDR 0x78
#define OLED_CMD  0x00
#define OLED_DATA 0x40

#define I2C_MASTER_SCL_IO 27
#define I2C_MASTER_SDA_IO 26
/*!< I2C master i2c port number, the number of i2c peripheral interfaces
 * available will depend on the chip */
#define I2C_MASTER_NUM 0

#define I2C_MASTER_TX_BUF_DISABLE 0 /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE 0 /*!< I2C master doesn't need buffer */
#define I2C_MASTER_TIMEOUT_MS     1000

u8g2_t u8g2;

static esp_err_t i2c_master_init(void)
{
    int i2c_master_port = I2C_MASTER_NUM;

    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 400000,
    };

    i2c_param_config(i2c_master_port, &conf);

    return i2c_driver_install(i2c_master_port,
                              conf.mode,
                              I2C_MASTER_RX_BUF_DISABLE,
                              I2C_MASTER_TX_BUF_DISABLE,
                              0);
}
static uint8_t u8x8_gpio_and_delay(u8x8_t *u8x8,
                                   uint8_t msg,
                                   uint8_t arg_int,
                                   void *arg_ptr)
{
    switch (msg)
    {
        case U8X8_MSG_GPIO_AND_DELAY_INIT: // called once during init phase of
                                           // u8g2/u8x8
            i2c_master_init();
            break;                         // can be used to setup pins
        case U8X8_MSG_DELAY_NANO:          // delay arg_int * 1 nano second
            break;
        case U8X8_MSG_DELAY_100NANO:       // delay arg_int * 100 nano seconds
            break;
        case U8X8_MSG_DELAY_10MICRO:       // delay arg_int * 10 micro seconds
            break;
        case U8X8_MSG_DELAY_MILLI:         // delay arg_int * 1 milli second
            break;
        case U8X8_MSG_DELAY_I2C: // arg_int is the I2C speed in 100KHz, e.g. 4 =
                                 // 400 KHz
            break; // arg_int=1: delay by 5us, arg_int = 4: delay by 1.25us
        case U8X8_MSG_GPIO_D0: // D0 or SPI clock pin: Output level in arg_int
                               // case U8X8_MSG_GPIO_SPI_CLOCK:
            break;
        case U8X8_MSG_GPIO_D1: // D1 or SPI data pin: Output level in arg_int
                               // case U8X8_MSG_GPIO_SPI_DATA:
            break;
        case U8X8_MSG_GPIO_D2: // D2 pin: Output level in arg_int
            break;
        case U8X8_MSG_GPIO_D3: // D3 pin: Output level in arg_int
            break;
        case U8X8_MSG_GPIO_D4: // D4 pin: Output level in arg_int
            break;
        case U8X8_MSG_GPIO_D5: // D5 pin: Output level in arg_int
            break;
        case U8X8_MSG_GPIO_D6: // D6 pin: Output level in arg_int
            break;
        case U8X8_MSG_GPIO_D7: // D7 pin: Output level in arg_int
            break;
        case U8X8_MSG_GPIO_E:  // E/WR pin: Output level in arg_int
            break;
        case U8X8_MSG_GPIO_CS: // CS (chip select) pin: Output level in arg_int
            break;
        case U8X8_MSG_GPIO_DC: // DC (data/cmd, A0, register select) pin: Output
                               // level in arg_int
            break;
        case U8X8_MSG_GPIO_RESET:     // Reset pin: Output level in arg_int
            break;
        case U8X8_MSG_GPIO_CS1:       // CS1 (chip select) pin: Output level in
                                      // arg_int
            break;
        case U8X8_MSG_GPIO_CS2:       // CS2 (chip select) pin: Output level in
                                      // arg_int
            break;
        case U8X8_MSG_GPIO_I2C_CLOCK: // arg_int=0: Output low at I2C clock pin
            break; // arg_int=1: Input dir with pullup high for I2C clock pin
        case U8X8_MSG_GPIO_I2C_DATA: // arg_int=0: Output low at I2C data pin
            break; // arg_int=1: Input dir with pullup high for I2C data pin
    }
    return 1;
}

static uint8_t u8x8_byte_hw_i2c(u8x8_t *u8x8,
                                uint8_t msg,
                                uint8_t arg_int,
                                void *arg_ptr)
{
    /* u8g2/u8x8 will never send more than 32 bytes between START_TRANSFER and
     * END_TRANSFER */
    static uint8_t buffer[32];
    static uint8_t buf_idx;
    uint8_t *data;

    switch (msg)
    {
        case U8X8_MSG_BYTE_SEND:
            data = (uint8_t *)arg_ptr;
            while (arg_int > 0)
            {
                buffer[buf_idx++] = *data;
                data++;
                arg_int--;
            }
            break;
        case U8X8_MSG_BYTE_INIT:
            /* add your custom code to init i2c subsystem */
            break;
        case U8X8_MSG_BYTE_SET_DC:
            /* ignored for i2c */
            break;
        case U8X8_MSG_BYTE_START_TRANSFER:
            buf_idx = 0;
            break;
        case U8X8_MSG_BYTE_END_TRANSFER:
            i2c_master_write_to_device(I2C_MASTER_NUM,
                                       OLED_ADDR >> 1,
                                       buffer,
                                       buf_idx,
                                       pdMS_TO_TICKS(1000));
            break;
        default:
            return 0;
    }
    return 1;
}

u8g2_t *ui_u8g2_new(void)
{
    u8g2_Setup_ssd1306_i2c_128x64_noname_f(&u8g2,
                                           U8G2_R0,
                                           u8x8_byte_hw_i2c,
                                           u8x8_gpio_and_delay);
    u8g2_InitDisplay(&u8g2);
    u8g2_SetPowerSave(&u8g2, 0);
    return &u8g2;
}

void minu_wave_drawBackground(wave_window_t *me, uint8_t dot_width)
{
    minu_attr_t win_attr = minu_base_getAttr(me);

    // draw frame
    minu_disp_drawRect(win_attr.x, win_attr.y, win_attr.w, win_attr.h);

    // draw horizontal dotted line
    for (uint8_t i = 0; i < win_attr.w / dot_width; i += dot_width)
        minu_disp_drawHLine(i * dot_width, win_attr.h / 2, dot_width);

    // draw vertical dotted line
    for (uint8_t i = 0; i < win_attr.h / dot_width; i += dot_width)
        minu_disp_drawVLine(win_attr.w / 2, i * dot_width, dot_width);
}

void minu_wave_init(wave_window_t *me,
                    uint8_t x,
                    uint8_t y,
                    uint8_t w,
                    uint8_t h,
                    uint16_t *buf,
                    uint16_t size)
{
    minu_base_init(me, x, y, w, h);
    me->buf = buf;
    me->size = size;
    me->in = me->max = me->min = 0;
}

void minu_wave_drawFullWave(wave_window_t *me)
{
    uint16_t x, y, prev_x, prev_y;
    minu_attr_t win_attr = minu_base_getAttr(me);

    /* find the minimum and maximum value in the buf */
    if (!(me->min + me->max))
        for (uint16_t i = 0; i < me->size; i++)
        {
            if (me->buf[i] > me->max)
                me->max = me->buf[i];
            if (me->buf[i] < me->min)
                me->min = me->buf[i];
        }

    for (uint16_t i = 0; i < me->size; i++)
    {
        // Normalized value to fit window w
        x = (float)i / me->size * win_attr.w;

        // Normalized value to fit window h
        y = (float)(me->buf[i] - me->min) / (me->max - me->min) * win_attr.h;

        // the bigger value are, the smaller showing in the screen. so we need
        // to reverse it
        y = win_attr.h - y;

        // draw line when it's not the first point
        if (i)
            minu_disp_drawLine(prev_x, prev_y, x, y);
        else
            minu_disp_drawPixel(x, y);

        prev_x = x;
        prev_y = y;
    }
}

void minu_wave_drawDynamicWave(wave_window_t *me, uint16_t data)
{
    uint16_t x, y, prev_x, prev_y;
    uint16_t pos = me->in++ % me->size;
    minu_attr_t win_attr = minu_base_getAttr(me);

    me->buf[pos] = data;

    if (data > me->max)
        me->max = data;
    if (data < me->min)
        me->min = data;

    for (uint16_t i = 0; i < me->size; i++)
    {
        // find the oldest value's position in the buffer
        uint8_t index = (i + pos + 1) % me->size;

        // Normalized value to fit window w
        x = (float)i / me->size * win_attr.w;

        // Normalized value to fit window h
        y = (float)(me->buf[index] - me->min) / (me->max - me->min) * win_attr.h;

        // the bigger value are, the smaller showing in the screen. so we
        // need to reverse it
        y = win_attr.h - y;

        // draw line when it's not the first point
        if (i)
            minu_disp_drawLine(prev_x, prev_y, x, y);
        else
            minu_disp_drawPixel(x, y);

        prev_x = x;
        prev_y = y;
    }
}
