#include "system.h"

void system_init(void){
//	#if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
//	SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));
//	#endif

  OLED_Init();
	HAL_ADC_Start_DMA(&hadc1,(uint32_t*)adc_dma_buffer[0],30);
	
	TIM4_DCDC_StartPWM();
	//TIM4_DCAC_StartPWM();
	//TIM1_StartPWM();
	
	scheduler_init();
}

