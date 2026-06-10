#include "tim_app.h"

#define PI 3.14159265358979323846f


/* 用户自定义参数 -------------------------------------------------------- */
#define ARR_VALUE       2099            // 与 CubeMX 设置的 Counter Period 一致
#define SINE_RES_MAX  4000          // 最大容量，根据需求设定

/* 全局变量 -------------------------------------------------------------- */
static uint16_t sine_res = 1600;    // 当前实际点数
uint32_t Frequency = 50;       // 频率，单位 Hz
static float sin_table[SINE_RES_MAX];       // 占空比表，范围 0~1



/* 正弦表生成函数 -------------------------------------------------------- */
void GenSinTable(uint32_t points)
{
		if (points > SINE_RES_MAX)
		{
			points = SINE_RES_MAX;      // 防止越界
		}
		
    for (uint16_t i = 0; i < points; i++)
    {
        sin_table[i] = 0.5f + 0.5f * sinf(2.0f * PI * i / points);
    }
    sine_res = points;              // 更新全局点数
}


/* TIM4 启动函数（生成正弦波） ------------------------------------------- */
void TIM4_StartPWM(void)
{
    // 1. 生成正弦表
    GenSinTable(sine_res);

    // 2. 启动两路 PWM
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);

    // 3. 使能更新中断（用于刷新占空比）
    __HAL_TIM_ENABLE_IT(&htim4, TIM_IT_UPDATE);
}


// 正弦表、启动函数、中断回调（之前给你的那份）
void TIM1_StartPWM(void)
{
    GenSinTable(sine_res);

    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
    HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
    HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_3);

    __HAL_TIM_MOE_ENABLE(&htim1);
    HAL_TIM_Base_Start_IT(&htim1);   // 开启更新中断
}

// 更新中断回调更新 CCR1/2/3（之前那份 SPWM 代码）

/* 定时器更新中断回调函数 ------------------------------------------------ */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM4)
    {
        static uint32_t idx = 0;

        // 从表中取出占空比
        float dutyA = sin_table[idx];          // 0.5+0.5sin，范围 0~1
        float dutyB = 1.0f - dutyA;

        // 写入比较寄存器
        uint32_t ccr1 = (uint32_t)(dutyA * ARR_VALUE);
        uint32_t ccr2 = (uint32_t)(dutyB * ARR_VALUE);
        __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, ccr1);
        __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, ccr2);

        // 指针下移
        idx = (idx + 1) % sine_res;
    }
    if (htim->Instance == TIM1)
    {
        static uint32_t idx = 0;

        uint32_t phase_a = idx;
        uint32_t phase_b = (idx + sine_res / 3) % sine_res;
        uint32_t phase_c = (idx + 2 * sine_res / 3) % sine_res;

        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, (uint32_t)(sin_table[phase_a] * ARR_VALUE));
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, (uint32_t)(sin_table[phase_b] * ARR_VALUE));
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, (uint32_t)(sin_table[phase_c] * ARR_VALUE));

        idx = (idx + 1) % sine_res;
    }
}
