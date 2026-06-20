#ifndef __BSP_SYSTEM_H__
#define __BSP_SYSTEM_H__

/*系统底层*/
#include "main.h"
#include "adc.h"
//#include "can.h"
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

#include "usart.h"
#include "stdio.h"
#include "stdarg.h"
#include "string.h"

#include <math.h>

/*用户底层*/
#include "system.h"
#include "scheduler.h"
#include "OLED_Hard.h"
#include "oled_app.h"
#include "uart_app.h"
#include "adc_app.h"
#include "key_app.h"
#include "tim_app.h"

extern uint8_t uart_rx_dma_buffer[128];

extern uint16_t adc_dma_buffer[1][30];
extern float adc_value;

extern uint32_t Frequency;   

extern float m;

extern float dcdc_duty;
#endif
