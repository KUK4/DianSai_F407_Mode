#include "key_app.h"


uint8_t key_value;
uint8_t key_value_last;
uint8_t key_down;
uint8_t key_up;

uint8_t key_read(void){
    if(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_0) == 0){
        return 1;
    }else if(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_1) == 0){
        return 2;
    }else if(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_2) == 0){
        return 3;
    }else if(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_3) == 0){
        return 4;
    }else if(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_4) == 0){
        return 5;
    }else if(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_5) == 0){
        return 6;
    }else if(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_6) == 0){
        return 7;
    }else if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13) == 0){
        return 8;
    }
    return 0;
}

void key_proc(void){
    key_value = key_read();
    key_down = (key_value) & (key_value ^ key_value_last);
    key_up = ~(key_value) & (key_value ^ key_value_last);
    key_value_last = key_value;
    switch (key_down)
    {   
    case 1:
        break;
    case 2:
        break;
    case 3:
        break;
    case 4: 
        break;
    case 5:
        break;
    case 6:
        break;
    case 7:
        break;
    case 8: 
        break;
	}
}
