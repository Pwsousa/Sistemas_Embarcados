#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "play_audio.c"

int main (){
    setup();
    while(true){
      play_song(BUZZER_A);
    }
}