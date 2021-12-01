/*
 * task.c
 *
 *  Created on: Nov 30, 2021
 *      Author: Asus
 */


#include "main.h"
#include "task.h"

void task1() {
	HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
}
void task2() {
	HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_1);
}
void task3() {
	HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_2);
}
void task4() {
	HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);
}
void task5() {
	HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_4);
}
