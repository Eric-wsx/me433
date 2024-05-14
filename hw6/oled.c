#include <stdio.h>
#include "ssd1306.h"
#include "font.h"
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"

// Define general settings for initializations and operations
#define BUFFER_SIZE 50
#define ADC_RANGE 4096

// Define character or message boundaries for the screen
#define COL 128
#define ROW 32
#define CHAR_COL 5 // Width of each character
#define CHAR_ROW 8 // Height of each character

// Define pin functionalities on Pico
#define LED_PIN PICO_DEFAULT_LED_PIN
#define ADC_PIN 26 
#define PICO_I2C_SDA_PIN 12
#define PICO_I2C_SCL_PIN 13

// Function declarations
static void draw_all_pixels(void);
static void draw_char(uint x, uint y, char letter);
static void draw_string(uint x, uint y, const char *message);

int main() {
    stdio_init_all();

    // Initialize the LED on Pico
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    // Initialize SDA and SCL pins on Pico for I2C
    gpio_init(PICO_I2C_SDA_PIN);
    gpio_init(PICO_I2C_SCL_PIN);
    gpio_set_function(PICO_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_I2C_SCL_PIN, GPIO_FUNC_I2C);

    // Initialize ADC
    adc_init();             // Initialize the ADC module
    adc_gpio_init(ADC_PIN); // Set ADC0 pin to be ADC input instead of GPIO
    adc_select_input(0);    // Select to read from ADC0

    // Initialize I2C
    i2c_init(i2c_default, 100 * 1000);

    // Initialize SSD1306
    ssd1306_setup();

    char message[BUFFER_SIZE];

    while (1) {
        // Blink the LED (GP25)
        gpio_put(LED_PIN, 1);
        sleep_ms(250);
        gpio_put(LED_PIN, 0);

        // Start timing the ADC read operation
        int start = to_us_since_boot(get_absolute_time());

        // Read the ADC value
        uint adc_val = adc_read();
        
        // Print the ADC value to the screen
        sprintf(message, "ADC voltage: %.3fV", ((double)adc_val * 3.3) / ADC_RANGE);
        draw_string(0, 0, message);
        ssd1306_update();

        // End timing the ADC read operation
        int end = to_us_since_boot(get_absolute_time());

        // Calculate and print the frames per second (FPS) to the screen
        double duration = (end - start) / 1000.0;
        sprintf(message, "FPS: %.3ff/s", 1000.0 / duration);
        draw_string(0, 2, message);
        ssd1306_update();

        // Poll every 250ms, adjusting for the time taken by the loop
        sleep_ms(250 - duration);
    }

    return 0;
}

// Function to draw all pixels on the screen (for testing purposes)
void draw_all_pixels(void) {
    for (char i = 0; i < COL; i++) {
        for (char j = 0; j < ROW; j++) {
            ssd1306_drawPixel(i, j, 1);
        }
    }
}

// Function to draw a single character at a given position
void draw_char(uint x, uint y, char letter) {
    for (int col = 0; col < CHAR_COL; col++) {
        char character = ASCII[letter - 32][col];
        for (int row = 0; row < CHAR_ROW; row++) {
            char bit = (character >> row) & 0b1;
            ssd1306_drawPixel(x + col, y + row, bit);
        }
    }
}

// Function to draw a string starting at a given position
void draw_string(uint x, uint y, const char *message) {
    int index = 0;
    while (message[index]) {
        draw_char(x + index * (CHAR_COL + 1), y * (CHAR_ROW + 1), message[index]);
        index++;
    }
}
