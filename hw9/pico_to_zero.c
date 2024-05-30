#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/irq.h"

#define BUFFER_SIZE 50
#define BAUD_RATE 115200
#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY UART_PARITY_NONE
#define UART_ID uart1
#define UART_TX_PIN 8
#define UART_RX_PIN 9

volatile int data_index = 0;
volatile char pi_zero_data[BUFFER_SIZE];

void get_chars()
{
    while (uart_is_readable(UART_ID))
    {
        unsigned char character = uart_getc(UART_ID);
        if (character == '\n')
        {
            pi_zero_data[data_index] = 0; // Null-terminate the string
            data_index = 0;
            printf("Send from Pi Zero: %s\n\r", pi_zero_data);
        }
        else
        {
            pi_zero_data[data_index] = character;
            data_index++;
        }
    }
}

void setup_uart()
{
    uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    uart_set_hw_flow(UART_ID, false, false);
    uart_set_format(UART_ID, DATA_BITS, STOP_BITS, PARITY);
    uart_set_fifo_enabled(UART_ID, false);

    int UART_IRQ = UART1_IRQ;
    irq_set_exclusive_handler(UART_IRQ, get_chars);
    irq_set_enabled(UART_IRQ, true);
    uart_set_irq_enables(UART_ID, true, false);
}

int main()
{
    stdio_init_all();
    while (!stdio_usb_connected())
    {
        sleep_ms(100);
    }

    setup_uart();

    int int_from_computer;
    char input[BUFFER_SIZE];

    while (1)
    {
        if (scanf("%d", &int_from_computer) == 1)
        {
            printf("Received from computer: %d\n\r", int_from_computer);
            snprintf(input, BUFFER_SIZE, "%d\n", int_from_computer);
            uart_puts(UART_ID, input);
        }
    }
}