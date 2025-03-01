#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "hardware/adc.h"

#define BUTTON_PIN_A 5
#define BUTTON_PIN_B 6

#define PIN_LED_A   13
#define PIN_LED_B   12
#define PIN_LED_C   11

#define BUZZER_PIN_A  21
#define BUZZER_PIN_B  10

#define JOYSTICK_VRY 26
#define JOYSTICK_VRX 27
#define BUTTON_JOYSTICK 22

const int ADC_CHANNEL_0 = 0;
const int ADC_CHANNEL_1 = 1; 

uint16_t led_A_level, led_B_level = 100;
uint16_t buzzer_A_level = 100;
uint slice_buzzer_A;
uint slice_led_A, slice_led_B; 
const float DIVIDER_PWM = 16.0;          
const uint16_t PERIOD = 4096;  


void setup();
void setup_joystick();
void setup_pwm_led(uint led, uint *slice, uint16_t level);
void joystick_read_axis(uint16_t *vrx_value, uint16_t *vry_value);

int main(){
    uint16_t vrx_value, vry_value, sw_value;

    setup();
    printf("Joystick-PWM");

    while (1){
        joystick_read_axis(&vrx_value, &vry_value);

        pwm_set_gpio_level(PIN_LED_A, vrx_value);
        pwm_set_gpio_level(PIN_LED_B, vry_value);
        pwm_set_gpio_level(BUZZER_PIN_B, vrx_value);

        sleep_ms(100);
        if(!gpio_get(BUTTON_JOYSTICK)){
            gpio_put(PIN_LED_C,1);
        }
        else if (!gpio_get(BUTTON_PIN_A)){
            gpio_put(PIN_LED_C,0);
        }
    }

}

void setup_joystick(){
    adc_init();
    adc_gpio_init(JOYSTICK_VRX);
    adc_gpio_init(JOYSTICK_VRY);

    gpio_init(BUTTON_JOYSTICK);
    gpio_set_dir(BUTTON_JOYSTICK, GPIO_IN);
    gpio_pull_up(BUTTON_JOYSTICK);
}

void setup_pwm_led(uint led, uint *slice, uint16_t level){
    gpio_set_function(led,GPIO_FUNC_PWM);
    *slice = pwm_gpio_to_slice_num(led);
    pwm_set_clkdiv(*slice, DIVIDER_PWM);
    pwm_set_gpio_level(led, level);
    pwm_set_enabled(*slice, true);
}

void setup_pwm_buzzer(uint buzzer, uint *slice, uint16_t level){
    gpio_set_function(buzzer, GPIO_FUNC_PWM);
    *slice = pwm_gpio_to_slice_num(buzzer);
    pwm_set_clkdiv(*slice, DIVIDER_PWM);
    pwm_set_gpio_level(buzzer, level);
    pwm_set_enabled(*slice, true);
}

void setup(){
    stdio_init_all();
    setup_joystick();

    gpio_init(PIN_LED_C);
    gpio_set_dir(PIN_LED_C, GPIO_OUT);

    setup_pwm_led(PIN_LED_A, &slice_led_A, led_A_level);
    setup_pwm_led(PIN_LED_B, &slice_led_B, led_B_level);

    gpio_init(BUTTON_PIN_A);
    gpio_set_dir(BUTTON_PIN_A, GPIO_IN);
    gpio_pull_up(BUTTON_PIN_A);

    setup_pwm_buzzer(BUZZER_PIN_B, &slice_buzzer_A, buzzer_A_level);
}

void joystick_read_axis(uint16_t *vrx_value, uint16_t *vry_value){

    adc_select_input(ADC_CHANNEL_0);
    sleep_us(2);
    *vrx_value = adc_read();

    adc_select_input(ADC_CHANNEL_1);
    sleep_us(2);
    *vry_value = adc_read();
}