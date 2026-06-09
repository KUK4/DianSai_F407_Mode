#include "system.h"

void system_init(void){
  OLED_Init();
	HAL_ADC_Start_DMA(&hadc1,(uint32_t*)adc_dma_buffer[0],30);
	
	//TIM4_StartPWM();
	TIM1_StartPWM();
	
	scheduler_init();
}

