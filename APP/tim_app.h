#ifndef __TIM_APP_H
#define __TIM_APP_H

#include "bsp_system.h"

void GenSinTable(uint32_t points);
void TIM1_StartPWM(void);
void TIM4_StartPWM(void);

#endif
