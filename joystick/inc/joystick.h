#ifndef JOYSTICH_H
#define JOYSTICH_H

/******************************| Includes |*******************************/
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"

/******************************| Definicoes |*******************************/
#define BUTTON_PIN_A 5
#define BUTTON_PIN_B 6

#define PIN_LED_A   13
#define PIN_LED_B   12
#define PIN_LED_C   11

#define JOYSTICK_VRY 26
#define JOYSTICK_VRX 27
#define BUTTON_JOYSTICK 22

#define ADC_CHANNEL_0   0
#define ADC_CHANNEL_1   1

/****************************| Cabecalho das funcoes |*********************/
void joystick_init(void);
void joystick_read_x(uint16_t *vrx_value);
void joystick_read_y(uint16_t *vry_value);
bool joystick_read_button(void);

#endif //JOYSTICH_H