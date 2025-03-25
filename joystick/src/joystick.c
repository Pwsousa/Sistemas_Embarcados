#include "joystick.h"

void joystick_init(void){

    // Inicia o adc
    adc_init();
    adc_gpio_init(JOYSTICK_VRX);
    adc_gpio_init(JOYSTICK_VRY);
    
    // iniciando e configurando o botao 
    gpio_init(BUTTON_JOYSTICK);
    gpio_set_dir(BUTTON_JOYSTICK, GPIO_IN); 
    gpio_pull_up(BUTTON_JOYSTICK);  

}

void joystick_read_x(uint16_t *vrx_value){
    
    adc_select_input(ADC_CHANNEL_0);
    sleep_ms(2);
    *vrx_value = adc_read();

}

void joystick_read_y(uint16_t *vry_value){
    adc_select_input(ADC_CHANNEL_1);
    sleep_ms(2);
    *vry_value = adc_read();
}

bool joystick_read_button(){
    if(!gpio_get(BUTTON_JOYSTICK)){
        return true;
    }else{
        return false;
    }
}

