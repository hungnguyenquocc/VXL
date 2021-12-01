/*
 * display.c
 *
 *  Created on: Nov 30, 2021
 *      Author: Asus
 */


#include "main.h"
#include "scheduler.h"
#include "display.h"


UART_HandleTypeDef huart2;

void display_task(int idx) {

	uint32_t time = HAL_GetTick();
	uint32_t tmp = time;
	int index = 0;
	while(tmp > 0) {
		index++;
		tmp /= 10;
	}
	uint8_t msg[index];
	sprintf(msg, "%ld", time);

	uint8_t task[15] = "ms - Task -\n\r";
	task[10] = Run_Tasks_G[idx].TaskId + '0';
	HAL_UART_Transmit(&huart2, msg, sizeof(msg), 100);
	HAL_UART_Transmit(&huart2, task, sizeof(task), 100);

}
