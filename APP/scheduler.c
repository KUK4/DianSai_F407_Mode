#include "scheduler.h"

uint8_t task_num;

typedef struct {
	void (*task_func)(void);
	uint32_t run_time;
	uint32_t last_time;    
} task_t;

static task_t scheduler_task[] = {
	{oled_proc,100,0},
	{key_proc,10,0}
	//{uart_proc,100,0},
};


void scheduler_init(void)
{
	task_num = sizeof(scheduler_task) / sizeof(task_t);
}

void scheduler_run(void)
{
	for(uint8_t i = 0;i < task_num;i++)
	{	
		uint32_t now_time = uwTick;
		if(now_time>=scheduler_task[i].run_time+scheduler_task[i].last_time)
		{
			scheduler_task[i].last_time = now_time;
			scheduler_task[i].task_func();
		}
	}
}

