#include "key_app.h"


uint8_t key_value=0;
uint8_t key_old=0;
uint8_t key_down=0;
uint8_t key_up=0;

uint8_t key_read(void){
    if(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_1) == 0){
        return 1;
    }else if(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_0) == 0){
        return 2;
    }else if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13) == 0){
        return 3;
    }else if(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_6) == 0){
        return 4;
    }else if(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_5) == 0){
        return 5;
    }else if(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_4) == 0){
        return 6;
    }else if(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_3) == 0){
        return 7;
    }else if(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_2) == 0){
        return 8;
    }
    return 0;
}

void key_proc(void){
    key_value = key_read();
    key_down = (key_value) & (key_old ^ key_value);
    key_up = ~(key_value) & (key_old ^ key_value);
    key_old = key_value;
		if(key_value == 1){
			GPIOC->BSRR = (1 << 5); 
		}
		if(key_value == 2){
			GPIOC->BSRR = (1 << (5+16)); 
		}
    switch (key_down)
    {   
			case 1:
        Frequency += 1;
        if(Frequency >=100){
            Frequency = 100;
        }
        GenSinTable(80000 / Frequency);
			break;
			case 2:
        Frequency -= 1;
        if(Frequency <= 20){
            Frequency = 20;
        }
        GenSinTable(80000 / Frequency);
			break;
			case 3:
				m += 0.01;
      break;
			case 4: 
				m -= 0.01;
      break;
			case 5:
				dcdc_duty += 0.01;
      break;
			case 6:
				dcdc_duty -= 0.01;
      break;
			case 7:
        break;
			case 8: 
        break;
	}
}
