#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include <math.h>  // Include to use sin function

#define PI 3.14159265358979323846
#define WAVEFORM_SIZE 100
#define MAX_VOLTAGE 3.3

static volatile float SineWaveform[WAVEFORM_SIZE];
static volatile float TriWaveform[WAVEFORM_SIZE];

void init_spi() {
    spi_init(spi_default, 1000000); // Initialize SPI at 1 MHz
    gpio_set_function(PICO_DEFAULT_SPI_RX_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_TX_PIN, GPIO_FUNC_SPI);

    // Initialize chip select pin
    gpio_init(PICO_DEFAULT_SPI_CSN_PIN);
    gpio_set_dir(PICO_DEFAULT_SPI_CSN_PIN, GPIO_OUT);
    gpio_put(PICO_DEFAULT_SPI_CSN_PIN, 1); // Active low, initialize to high
}

void cs_select() {
    gpio_put(PICO_DEFAULT_SPI_CSN_PIN, 0);
    sleep_us(100); // Small delay to ensure setup is respected
}

void cs_deselect() {
    gpio_put(PICO_DEFAULT_SPI_CSN_PIN, 1);
    sleep_us(100); // Small delay to ensure hold time is respected
}

void generate_sine_waveform() {
    float amplitude = MAX_VOLTAGE / 2.0;
    float offset = amplitude;
    for (int i = 0; i < WAVEFORM_SIZE; i++) {
        float theta = (float)i / WAVEFORM_SIZE * 4.0 * PI; // 2 full cycles
        SineWaveform[i] = amplitude * sin(theta) + offset;
    }
}

void generate_triangle_waveform() {
    int halfSize = WAVEFORM_SIZE / 2;
    float rise = MAX_VOLTAGE / halfSize;
    for (int i = 0; i < halfSize; i++) {
        TriWaveform[i] = rise * i;
        TriWaveform[halfSize + i] = MAX_VOLTAGE - rise * i;
    }
}

void write_waveform_data(bool channel, uint16_t data) {
    uint8_t buf[2] = {data >> 8, data & 0xFF};  // Prepare 16-bit data
    buf[0] |= (channel << 7) | 0x70; // Setup DAC config bits for channel A or B
    cs_select();
    spi_write_blocking(spi_default, buf, 2);
    cs_deselect();
}

void output_waveforms() {
    for (int i = 0; i < WAVEFORM_SIZE; i++) {
        uint16_t sineData = (uint16_t)(SineWaveform[i] * 1023 / MAX_VOLTAGE);
        uint16_t triData = (uint16_t)(TriWaveform[i] * 1023 / MAX_VOLTAGE);
        write_waveform_data(0, sineData); // Channel A for Sine
        write_waveform_data(1, triData);  // Channel B for Triangle
        sleep_ms(5);
    }
}

int main() {
    stdio_init_all();
    init_spi();
    generate_sine_waveform();
    generate_triangle_waveform();

    while (1) {
        output_waveforms();
    }

    return 0;
}
