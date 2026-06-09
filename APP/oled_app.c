#include "oled_app.h"

#define OLED_STRING_BUFFER_SIZE  16  

void OLED_Printf(uint8_t Line, uint8_t Column, char *format, ...)
{
    char String[OLED_STRING_BUFFER_SIZE];
    va_list arg;
    va_start(arg, format);
    vsprintf(String, format, arg);
    va_end(arg);
    OLED_ShowString(Line, Column, String);
}

void oled_proc(void){
	OLED_Printf(1,1,"ADC1:  ");
}

