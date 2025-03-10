#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "audio/play_audio.h"

int main (){
    setup();
    while(true){
      play_song(BUZZER_A);
    }
}