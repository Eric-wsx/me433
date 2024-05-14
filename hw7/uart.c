#include <stdio.h>
#include "ssd1306.h"
#include "font.h"
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/uart.h"
#include "hardware/irq.h"

#define UART_ID uart0
#define BAUD_RATE 115200
#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY    UART_PARITY_NONE

#define UART_TX_PIN 0
#define UART_RX_PIN 1

static int chars_rxed = 0;
static char message_stored[100];
static int message_index = 0;

void drawchar(uint x, uint y, char letter) {
    for (int col = 0; col < 5; col++) {
        char cha = ASCII[letter - 32][col];
        for (int row = 0; row < 8; row++) {
            char bit = (cha >> row) & 0b1;
            ssd1306_drawPixel(x + col, y + row, bit);
        }
    }
}

void drawstring(uint x, uint y, const char *message) {
    int i = 0;
    while (message[i]) {
        drawchar(x + i * 5, y * 8, message[i]);
        i++;
    }
    ssd1306_update();
}

void on_uart_rx() {
    while (uart_is_readable(UART_ID)) {
        uint8_t ch = uart_getc(UART_ID);
        if (ch == '\r' || ch == '\n') {
            ssd1306_clear();
            drawstring(0, 0, message_stored);
            ssd1306_update();
            message_index = 0;
        } else {
            if (message_index < sizeof(message_stored) - 1) {
                message_stored[message_index++] = ch;
                message_stored[message_index] = '\0';
            }
        }

        if (uart_is_writable(UART_ID)) {
            uart_putc(UART_ID, ch);
        }
        chars_rxed++;
    }
}

void setup_uart() {
    uart_init(UART_ID, 2400);
    int __unused actual = uart_set_baudrate(UART_ID, BAUD_RATE);
    uart_set_hw_flow(UART_ID, false, false);
    uart_set_format(UART_ID, DATA_BITS, STOP_BITS, PARITY);
    uart_set_fifo_enabled(UART_ID, false);
    int UART_IRQ = UART_ID == uart0 ? UART0_IRQ : UART1_IRQ;
    irq_set_exclusive_handler(UART_IRQ, on_uart_rx);
    irq_set_enabled(UART_IRQ, true);
    uart_set_irq_enables(UART_ID, true, false);
}

void setup_gpio() {
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
}

void setup_i2c() {
    i2c_init(i2c_default, 100 * 1000);
    gpio_init(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_init(PICO_DEFAULT_I2C_SCL_PIN);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
}

int main() {
    stdio_init_all();
    setup_gpio();
    setup_i2c();
    setup_uart();
    ssd1306_setup();

    while (1) {
        gpio_put(PICO_DEFAULT_LED_PIN, 1);
        sleep_ms(250);
        gpio_put(PICO_DEFAULT_LED_PIN, 0);
        sleep_ms(250);
    }
}