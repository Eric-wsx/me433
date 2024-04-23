#include "pico/stdlib.h"
#include "hardware/i2c.h"

// Device default I2C address
#define MCP23008_ADDR 0x20

// MCP23008 chip register addresses
#define MCP23008_IODIR 0x00  // I/O direction register
#define MCP23008_GPIO  0x09  // GPIO register for reading pin states
#define MCP23008_OLAT  0x0A  // Output latch register

// Raspberry Pi Pico I2C SDA and SCL pins
#define I2C_SDA_PIN 12
#define I2C_SCL_PIN 13

// Raspberry Pi Pico default LED pin
#define LED_PIN PICO_DEFAULT_LED_PIN

// Function prototypes
void initialize_mcp23008();
void set_led(bool state);
bool read_button();

int main() {
    // Initialize standard input output
    stdio_init_all();
    // Initialize Pico on-board LED
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    // Initialize I2C communication
    i2c_init(i2c_default, 100000); // Set to 100 kHz
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);

    // Initialize MCP23008
    initialize_mcp23008();

    while (true) {
        // Control on-board LED to blink
        gpio_put(LED_PIN, true);
        sleep_ms(250);
        gpio_put(LED_PIN, false);
        sleep_ms(250);

        // Control GP7 LED based on GP0 button state
        set_led(!read_button());
        sleep_ms(250);
    }
}

void initialize_mcp23008() {
    uint8_t buf[] = {MCP23008_IODIR, 0x7F}; // Set IODIR register, GP0 as input, others as output
    i2c_write_blocking(i2c_default, MCP23008_ADDR, buf, 2, false);
}

void set_led(bool state) {
    uint8_t buf[] = {MCP23008_OLAT, state << 7}; // Control LED by setting the highest bit of OLAT register
    i2c_write_blocking(i2c_default, MCP23008_ADDR, buf, 2, false);
}

bool read_button() {
    uint8_t reg = MCP23008_GPIO;
    uint8_t value;
    i2c_write_blocking(i2c_default, MCP23008_ADDR, &reg, 1, true); // Hold bus control
    i2c_read_blocking(i2c_default, MCP23008_ADDR, &value, 1, false); // Release bus control
    return (value & 0x01) != 0; // Return the state of GP0
}
