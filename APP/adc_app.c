#include "adc_app.h"


uint16_t adc_dma_buffer[1][30];


float adc_value;

void adc_proc(void){
    uint32_t adc_sum[1] = {0};
    //adc1
    for(int i = 0; i < 30; i+=1){
        adc_sum[0] += adc_dma_buffer[0][i];

    }
    adc_value = (float)adc_sum[0] / 30.0f * 3.3f / 4096.0f;
}
