#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"

// Constants
#define LED_PIN 0
#define BUTTON_PIN 1
#define ADC_INPUT_PIN 26

// Initialize GPIO for LED and button
void init_gpio() {
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, 1); // Turn on the LED
}

// Wait for USB to be connected
void wait_for_usb_connection() {
    while (!stdio_usb_connected()) {
        sleep_ms(100);
    }
}

// Initialize ADC for reading
void init_adc() {
    adc_init();
    adc_gpio_init(ADC_INPUT_PIN);
    adc_select_input(0);
}

// Read voltage and print it
void read_and_print_voltage(int frequency) {
    for (int i = 0; i < frequency; i++) {
        uint16_t voltage = adc_read();
        sleep_ms(10);
        printf("Voltage detected: %fV\r\n", (voltage * 3.3) / 4090);
    }
}

int main() {
    stdio_init_all();
    wait_for_usb_connection();
    init_gpio();

    // Wait for button press to turn off LED
    while (gpio_get(BUTTON_PIN) == 0);
    gpio_put(LED_PIN, 0); // Turn off the LED

    printf("Start!\n");
    init_adc();

    int frequency;
    while (1) {
        scanf("%d", &frequency);
        read_and_print_voltage(frequency);
    }

    return 0;
}
