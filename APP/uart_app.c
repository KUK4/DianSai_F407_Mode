#include "uart_app.h"



void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	printf("dma data:%s\r\n",uart_rx_dma_buffer);
}


//void uart_proc(void)
//{
//}

