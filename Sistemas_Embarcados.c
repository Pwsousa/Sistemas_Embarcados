#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "hardware/gpio.h"

#define PIN_LED_A 13
#define PIN_LED_B 12

int led_value_A = 0;
int led_value_B = 0;

bool repeating_timer_callback(struct repeating_timer *t) {
    led_value_A = 1 - led_value_A;
    gpio_put(PIN_LED_A, led_value_A);
    return true;
};

bool repeating_timer_call(struct repeating_timer *g){
    led_value_B = 1 - led_value_B;
    gpio_put(PIN_LED_B, led_value_B);
    return true;
};

void setup();

int main(){
    setup();
    struct repeating_timer timer;
    struct repeating_timer timer2;


    add_repeating_timer_ms(250, repeating_timer_callback, NULL, &timer);
    sleep_ms(100);
    add_repeating_timer_ms(1000, repeating_timer_call, NULL, &timer2);

    while(true){
        tight_loop_contents();
    }
    
}

void setup(){
    stdio_init_all();
    gpio_init(PIN_LED_A);
    gpio_init(PIN_LED_B);
    gpio_set_dir(PIN_LED_A, GPIO_OUT);
    gpio_set_dir(PIN_LED_B, GPIO_OUT);
}