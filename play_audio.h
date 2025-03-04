#include <stdio.h>
#include "pico/stdio.h"
#include "hardware/pwm.h"
#include "hardware/gpio.h"
#include "notes.h"

#define BUZZER_A 21
#define BUZZER_B 10

const float CLOCK_PWM = 16.0;
const uint16_t MAX_DIV_BUZZER = 16;
const uint16_t MIN_DIV_BUZZER = 2;


typedef enum {
    PLAY_NOTE,
    DURATION_NOTE,
    PLAY_REST,
    DURATION_REST,
} state_song;

void play_note(uint pin, uint16_t wrap);
void play_rest(uint pin);
extern void play_song(int pin);
extern void setup();

