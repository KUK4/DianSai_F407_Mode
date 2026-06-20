#include "tim_app.h"

#define PI 3.14159265358979323846f


/* 用户自定义参数 -------------------------------------------------------- */
#define ARR_VALUE       2099            // 与 CubeMX 设置的 Counter Period 一致
#define SINE_RES_MAX  4000          // 最大容量，根据需求设定 80khz/SINE_RES_MAX  =fre_min(最小频率)

/* 全局变量 -------------------------------------------------------------- */
static uint16_t sine_res = 1600;    // 当前实际点数
uint32_t Frequency = 50;       // 频率，单位 Hz
static float sin_table[SINE_RES_MAX];       // 占空比表，范围 0~1
float m=1.000f;

static uint32_t idx_tim4 = 0;
static uint32_t idx_tim1 = 0;


float dcdc_duty = 0.50f;          // DCDC 占空比，范围 0~1，默认 50%
#define TIM4_MODE_DCAC   0 //dcac模式
#define TIM4_MODE_DCDC   1 //dcdc模式
static uint8_t tim4_mode = TIM4_MODE_DCAC;  // 默认保持原 DCAC 模式，也可初始化为 DCDC

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

void TIM4_DCDC_StartPWM(void)
{
		// 设置为 DCDC 模式
		tim4_mode = TIM4_MODE_DCDC;

		// 只启动通道1，不需要互补，也不需要正弦表
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);

    // 使能更新中断，用于实时刷新占空比
    __HAL_TIM_ENABLE_IT(&htim4, TIM_IT_UPDATE);
}
	/* TIM4 启动函数（生成正弦波） ------------------------------------------- */
	
void TIM4_DCAC_StartPWM(void)
{
	//设置模式为TIM4_MODE_DCAC
		tim4_mode = TIM4_MODE_DCAC;
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
			if (tim4_mode == TIM4_MODE_DCDC)
        {
            // DCDC 模式：直接将限幅后的占空比写入通道1
						if(dcdc_duty<=0.10f)dcdc_duty = 0.10f;
						if(dcdc_duty>=0.85f)dcdc_duty = 0.85f;
            TIM4->CCR3 = (uint32_t)(dcdc_duty * ARR_VALUE);
            // 通道2可关闭或设为0，避免干扰（如果之前打开过，可在此清零）
        }
			else{
				if (m > 1.0f) m = 1.0f;
				// 从表中取出占空比
				float dutyA_raw = sin_table[idx_tim4];
				float dutyA = 0.5f + 0.5f * m * (2*dutyA_raw - 1);          // 0.5+0.5sin，范围 0~1
				
				float dutyB = 1.0f - dutyA;

				// 写入比较寄存器
				TIM4->CCR1 = (uint32_t)(dutyA * ARR_VALUE);
				TIM4->CCR2 = (uint32_t)(dutyB * ARR_VALUE);

				// 指针下移
				idx_tim4 = (idx_tim4 + 2) % sine_res;
			}
		}
		if (htim->Instance == TIM1)
		{
				uint32_t phase_a = idx_tim1;
				uint32_t phase_b = (idx_tim1 + sine_res / 3) % sine_res;
				uint32_t phase_c = (idx_tim1 + 2 * sine_res / 3) % sine_res;
				TIM1->CCR1 = (uint32_t)(sin_table[phase_a] * ARR_VALUE);
				TIM1->CCR2 = (uint32_t)(sin_table[phase_b] * ARR_VALUE);
				TIM1->CCR3 = (uint32_t)(sin_table[phase_c] * ARR_VALUE);


				idx_tim1 = (idx_tim1 + 1) % sine_res;
		}
}
