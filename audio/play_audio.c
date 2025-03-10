#include "play_audio.h"

uint16_t wrap_div_buzzer = 8;


void setup(){
    uint slice;
    stdio_init_all();

    gpio_set_function(BUZZER_A, GPIO_FUNC_PWM);
    slice = pwm_gpio_to_slice_num(BUZZER_A);
    pwm_set_clkdiv(slice, CLOCK_PWM);
}

void play_note(uint pin, uint16_t wrap){

    
    int slice = pwm_gpio_to_slice_num(pin);
    pwm_set_wrap(slice,wrap);
    pwm_set_gpio_level(pin, wrap / wrap_div_buzzer);
    pwm_set_enabled(slice, true);
}

void play_rest(uint pin){
    int slice = pwm_gpio_to_slice_num(pin);
    pwm_set_enabled(slice,false);
}

void play_song(int pin){
    static uint i = 0;
    static uint cont = 0;
    static state_song song = PLAY_NOTE;

    int duration = 800 / durations[i];

    switch (song) {
    case PLAY_NOTE:
        play_note(pin, melody[i]);
        song = DURATION_NOTE;
        cont = 0;
        break;
    
    case DURATION_NOTE: 
        sleep_ms(1);
        if (cont ++ > duration) song = PLAY_REST;
        break;

    case PLAY_REST:
        play_rest(pin);
        song = DURATION_REST;
        cont = 0;
        break;
    case DURATION_REST:
        sleep_ms(1);
        if (cont++ > duration){
            song = PLAY_NOTE;
            i++;
            if (i > SONG_LENGTH) i = 0;
        }
        break;
    default:
        i, cont = 0;
        song = PLAY_NOTE;
        break;
    }

}


