#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "hardware/gpio.h"

#define PIN_LED_A 13
#define PIN_LED_B 12
#define PIN_BUTTON_A 5

int led_value_A = 0;
int led_value_B = 0;

bool repeating_timer_callback(struct repeating_timer *t) {
    led_value_A = 1 - led_value_A;
    gpio_put(PIN_LED_A, led_value_A);
    printf("LED TOGGLED");
    return true;
};

bool repeating_timer_call(struct repeating_timer *g){
    led_value_B = 1 - led_value_B;
    gpio_put(PIN_LED_B, led_value_B);
    return true;
};

int64_t alarm_callback(alarm_id_t id, void *user_data){
    printf("Alarm Fired");
    return 0;
}

void buttom_callback(uint gpio, uint32_t events){
    gpio_put(PIN_LED_A, 0);
    gpio_put(PIN_LED_B, 1);
    sleep_ms(1000);
    gpio_put(PIN_LED_B,0);
    printf("Interrupts ocurred at pin %d with event %d", gpio , events);
    
}

void setup();

int main(){
    setup();

    gpio_set_irq_enabled_with_callback(PIN_BUTTON_A, 0x04, 1, buttom_callback);

    struct repeating_timer timer;
    struct repeating_timer timer2;
    add_repeating_timer_ms(500, repeating_timer_callback, NULL, &timer);
    sleep_ms(100);
    //add_repeating_timer_ms(1000, repeating_timer_call, NULL, &timer2);
    //sleep_ms(100);
    add_alarm_in_ms(2000, alarm_callback, NULL, false);
    while(true){
        
        tight_loop_contents();
        
    }
    
}

void setup(){
    stdio_init_all();
    gpio_init(PIN_LED_A);
    gpio_init(PIN_LED_B);
    gpio_init(PIN_BUTTON_A);
    gpio_set_dir(PIN_LED_A, GPIO_OUT);
    gpio_set_dir(PIN_LED_B, GPIO_OUT);
    gpio_set_dir(PIN_BUTTON_A, GPIO_IN);
    gpio_pull_up(PIN_BUTTON_A);
}