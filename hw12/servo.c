#include "pico/stdlib.h"
#include "hardware/pwm.h" 

#define LED_PIN PICO_DEFAULT_LED_PIN
#define PWM_PIN 15

uint16_t PWM_init();
void move_angle(uint16_t wrap);

int main() {
    stdio_init_all();
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    
    uint16_t wrap = PWM_init();

    while (1) {
        move_angle(wrap);
    }
}

uint16_t PWM_init(){    
    // 125M / 50 / 50 = 50 Hz
    gpio_set_function(PWM_PIN, GPIO_FUNC_PWM); // Set the PWM pin to function as PWM
    uint slice_num = pwm_gpio_to_slice_num(PWM_PIN); // Get PWM slice number
    float div = 50.0f; // Divider must be between 1 and 255
    pwm_set_clkdiv(slice_num, div); // Set the clock divider
    uint16_t wrap = 50000; // Set the wrap value, must be less than 65535
    pwm_set_wrap(slice_num, wrap); // Set the PWM wrap value
    pwm_set_enabled(slice_num, true); // Enable the PWM
    return wrap;
}

void move_angle(uint16_t wrap){
    int degree0 = wrap * 0.025; // Set duty cycle to 2.5% (1250)
    int degree180 = wrap * 0.125; // Set duty cycle to 12.5% (6250)

    // Move from 0 to 180 degrees
    for (int i = degree0; i < degree180; i += 2) {
        pwm_set_gpio_level(PWM_PIN, i);
        sleep_ms(1);
    }

    // Move from 180 to 0 degrees
    for (int i = degree180; i > degree0; i -= 2) {
        pwm_set_gpio_level(PWM_PIN, i); 
        sleep_ms(1);
    }
}