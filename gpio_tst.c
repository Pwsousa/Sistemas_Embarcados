#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

#define BUTTON_PIN_A 5
#define BUTTON_PIN_B 6

#define PIN_LED_A   13
#define PIN_LED_B   12
#define PIN_LED_C   11

#define BUZZER_PIN_A  21
#define BUZZER_PIN_B  10

int main(){
    stdio_init_all();

    gpio_init(PIN_LED_A);
    gpio_init(PIN_LED_B);
    gpio_init(PIN_LED_C);

    gpio_init(BUTTON_PIN_A);
    gpio_init(BUTTON_PIN_B);

    gpio_init(BUZZER_PIN_A);
    gpio_init(BUZZER_PIN_B);


    gpio_set_dir(PIN_LED_A, GPIO_OUT);
    gpio_set_dir(PIN_LED_B, GPIO_OUT);
    gpio_set_dir(PIN_LED_C, GPIO_OUT);

    gpio_set_dir(BUTTON_PIN_A, GPIO_IN);
    gpio_set_dir(BUTTON_PIN_B, GPIO_IN);

    gpio_set_dir(BUZZER_PIN_A, GPIO_OUT);
    gpio_set_dir(BUZZER_PIN_B, GPIO_OUT);

    gpio_pull_up(BUTTON_PIN_A);
    gpio_pull_up(BUTTON_PIN_B);

    while (true){
        if(!gpio_get(BUTTON_PIN_A)){
            gpio_put(PIN_LED_A,1);

        }
    }
    
}