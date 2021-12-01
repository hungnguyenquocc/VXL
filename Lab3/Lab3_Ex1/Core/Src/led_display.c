#include "main.h"
void display_7Seg1(uint8_t value){
	uint8_t unit = value %10;
	uint8_t tens =  value / 10;
	for(int i = 0; i < 4; i++){
		if(tens & (1 << i))
			HAL_GPIO_WritePin(GPIOA,(1 << i),GPIO_PIN_SET);
		else
			HAL_GPIO_WritePin(GPIOA,(1 << i),GPIO_PIN_RESET);
	}
	for(int i = 4; i < 8; i++){
			if(unit & (1 << (i - 4)))
				HAL_GPIO_WritePin(GPIOA,(1 << i),GPIO_PIN_SET);
			else
				HAL_GPIO_WritePin(GPIOA,(1 << i),GPIO_PIN_RESET);
	}
}

void display_7Seg2(uint8_t value){
	uint8_t unit = value %10;
	uint8_t tens =  value / 10;
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, (tens >> 0)&(0x01) );
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, (tens >> 1)&(0x01) );
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, (tens >> 2)&(0x01) );
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, (tens >> 3)&(0x01) );

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, (unit >> 0)&(0x01) );
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_13, (unit >> 1)&(0x01) );
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_14, (unit >> 2)&(0x01) );
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, (unit >> 3)&(0x01) );
}

void displaySingleLed(int status){
	switch (status) {
	case 0: // RED - GREEN - row
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);

		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
		break;
	case 1: // RED - YELLOW - row
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);

		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
		break;
	case 2: // RED - GREEN - row
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET);

		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
		break;
	case 3: // RED - YELLOW- col
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET);

		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
		break;
	default:
		break;
	}
}

void reset_12LED(){
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5, 0);
}
