#include "main.h"
#include "input_reading.h"
#include "input_processing.h"

void HAL_TIM_PeriodElapsedCallback ( TIM_HandleTypeDef * htim )
{
	if(htim -> Instance == TIM2 ){
		button_reading ();
		timer_run0();
		timer_run1();
	}
}

