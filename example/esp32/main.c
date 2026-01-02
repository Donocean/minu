#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "minu_disp.h"
#include "minu_item.h"
#include "minu_types.h"
#include "u8g2.h"
#include "u8g2_port.h"
#include "driver/uart.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"

#include "minu.h"
#include "minu_port.h"
#include "minu_viewer.h"

#include "esp_log.h"

QueueHandle_t uart_queue;
static uint8_t data[128];

void minu_timer_cb(TimerHandle_t xTimer)
{
    // 1ms timer callback
    minu_tick_inc(10);
}

void minu_timer_init(void)
{
    TimerHandle_t anim_timer =
        xTimerCreate("minu timer",      // Just a text name, not used by the kernel.
                     pdMS_TO_TICKS(10), // The timer period in ticks.
                     pdTRUE, // The timers will auto-reload themselves when they expire.
                     NULL,   // Assign each timer a unique id equal to its array index.
                     minu_timer_cb // Each timer calls the same callback when it expires.
        );
    xTimerStart(anim_timer, 0);
}

// Read data from UART.
uint16_t uart_read(void)
{
    uint16_t length = 0;
    uart_get_buffered_data_len(UART_NUM_0, (size_t *)&length);
    length = uart_read_bytes(UART_NUM_0, data, length, 100);
    return length;
}

void uart0_init(uint32_t baud_rate)
{
    const uart_port_t uart_num = UART_NUM_0;

    uart_config_t uart_config = {
        .baud_rate = baud_rate,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
    };
    // Configure UART parameters
    ESP_ERROR_CHECK(uart_param_config(uart_num, &uart_config));

    // Set UART pins(TX: IO0, RX: IO3, RTS: IO18, CTS: IO19)
    ESP_ERROR_CHECK(uart_set_pin(uart_num, 0, 3, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
    // Install UART driver using an event queue here
    ESP_ERROR_CHECK(uart_driver_install(uart_num, 1024, 1024, 5, &uart_queue, 0));
}

bool check_test1;
bool check_test2;
int itest = 6;
float ftest = 3.14;
int inc_i = 0;

void var_int_cb(void *para, uint16_t e)
{
    int *var = para;
    switch (e)
    {
        case MINU_EVENT_UP:
            *var += 10;
            break;
        case MINU_EVENT_DOWN:
            *var -= 10;
            break;
    }
}

void draw_test(void *para)
{
    minu_disp_fillRect(10, 10, 128 - 20, 64 - 20);
}

void app_main(void)
{
    uart0_init(115200);

    u8g2_t *u = ui_u8g2_new();
    minu_port_new_disp_u8g2(u);

    minu_disp_setFont((void *)u8g2_font_HelvetiPixel_tr);
    /* minu_disp_setFont((void *)u8g2_font_10x20_tf); // big font */

    minu_handle_t m0 = minu_creat(MINU_TYPE_SELECTOR_VERTICAL, "main", 0, 0, 128, 64);
    minu_addCheckBoxItem(m0, "check1 ", &check_test1);
    minu_addCheckBoxItem(m0, "check2", &check_test2);
    minu_addVariableItem(m0, "int", &itest, "%d", NULL);
    minu_addVariableItem(m0, "float", &ftest, "%.2f", NULL);
    minu_addVariableItem(m0, "int increase", &inc_i, "%d", &var_int_cb);

    minu_addWindowItem(m0, "nice d", draw_test, NULL, NULL);
    minu_addWindowItem(m0, "hello hi", draw_test, NULL, NULL);
    minu_addWindowItem(m0, "hi hello hi", draw_test, NULL, NULL);

    minu_handle_t m1 = minu_creat(MINU_TYPE_SELECTOR_VERTICAL, "submenu", 0, 0, 128, 64);
    minu_addCheckBoxItem(m1, "submenu check0", &check_test1);
    minu_addCheckBoxItem(m1, "submenu check1", &check_test1);
    minu_addCheckBoxItem(m1, "submenu check2", &check_test1);
    minu_addCheckBoxItem(m1, "submenu check3", &check_test1);
    minu_addCheckBoxItem(m1, "submenu check4", &check_test1);
    minu_addSubmenuItem(m0, "submenu", m1);

    minu_viewer_handle_t viewer = minu_viewer_create(m0);

    /* prepare for animation */
    minu_timer_init();
    while (1)
    {
        uint16_t len = uart_read();

        if (len != 0)
        {
            switch (data[0])
            {
                case 'k':
                    minu_viewer_event_post_to(viewer, MINU_EVENT_UP);
                    break;
                case 'j':
                    minu_viewer_event_post_to(viewer, MINU_EVENT_DOWN);
                    break;
                case 'l':
                    minu_viewer_event_post_to(viewer, MINU_EVENT_ENTER);
                    break;
                case 'h':
                    minu_viewer_event_post_to(viewer, MINU_EVENT_QUIT);
                    break;
                case 'd':
                    minu_viewer_event_post_to(viewer, MINU_EVENT_DELETE);
                    break;
                case 'r':
                    esp_restart();
                default:
                    break;
            }
        }
        minu_viewer_update(viewer);

        vTaskDelay(pdMS_TO_TICKS(1));
    }
}
