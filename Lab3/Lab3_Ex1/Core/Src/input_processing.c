#include "main.h"
#include "input_reading.h"
#include "led_display.h"
enum ButtonState { BUTTON_RELEASED , BUTTON_PRESSED ,BUTTON_PRESSED_MORE_THAN_1_SECOND } ;
enum ButtonState buttonState = BUTTON_RELEASED ;
enum Mode { MODE1, MODE2, MODE3, MODE4};
enum Mode mode = MODE1;
int TIMER_CYCLE = 10;

int timer0_counter = 0;
int timer0_flag = 1;

int timer1_counter = 0;
int timer1_flag = 1;
void setTimer1(int duration){
	timer1_counter = duration/TIMER_CYCLE;
	timer1_flag = 0;
}

void timer_run1(){
	if(timer1_counter > 0){
		timer1_counter --;
		if(timer1_counter == 0){
			timer1_flag = 1;
		}
	}
}

void setTimer0(int duration){
	timer0_counter = duration/TIMER_CYCLE;
	timer0_flag = 0;
}

void timer_run0(){
	if(timer0_counter > 0){
		timer0_counter --;
		if(timer0_counter == 0){
			timer0_flag = 1;
		}
	}
}

void func_mode1(){
	if(timer0_flag){
		setTimer0(300);
		display_7Seg2(1);
		if(state_mode1 == 0){
			if(timeTotal > 0){
				display_7Seg1(timeTotal);
				if(timeTotal - timeAmber > 0){
					displaySingleLed(0);
					timeTotal--;
				}
				else{
					displaySingleLed(1);
					timeTotal--;
				}
			}
			else{
				timeTotal = timeRed;
				state_mode1 = 1;
			}
		}
		else{
			if(timeTotal > 0){
				display_7Seg1(timeTotal);
				if(timeTotal - timeAmber > 0){
					displaySingleLed(2);
					timeTotal--;
				}
				else{
					displaySingleLed(3);
					timeTotal--;
				}
			}
			else{
				timeTotal = timeRed;
				state_mode1 = 0;
			}
		}
	}
}

void func_mode2(){
	display_7Seg2(2);
	if(timer0_flag){
		setTimer0(300);
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_2);
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5);
	}
	if(flagPressButton3 == 0){
		if(is_button_pressed(1)){
			if(press[1]){
				timeRed++;
				temp1++;
				if(timeRed > 99) timeRed = 0;
				press[1] = 0;
			}
		}
		else{
			press[1] = 1;
		}
		display_7Seg1(timeRed);
		if(is_button_pressed(2)){
			if(press[2]){
				time_buffer[0] = timeRed;
				timeTotal = timeRed;
				reset_12LED();
				flagPressButton3 = 1;
				press[2] = 0;
			}
		}
		else{
			press[2] = 1;
		}
	}
	else{
		if(timer1_flag){
			setTimer1(300);
			if(temp1 > 0){
				display_7Seg1(temp1);
				temp1 --;
			}
			else{
				temp1 = timeRed;
			}
		}
	}
}

void func_mode3(){
	display_7Seg2(3);
	if(timer0_flag){
		setTimer0(300);
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_1);
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_4);
	}
	if(flagPressButton3 == 0){
		if(is_button_pressed(1)){
			if(press[1]){
				timeAmber++;
				temp2++;
				if(timeAmber > 99) timeAmber = 0;
				press[1] = 0;
			}
		}
		else{
			press[1] = 1;
		}
		display_7Seg1(timeAmber);
		if(is_button_pressed(2)){
			if(press[2]){
				time_buffer[1] = timeAmber;
				timeTotal = timeRed;
				reset_12LED();
				flagPressButton3 = 1;
				press[2] = 0;
			}
		}
		else{
			press[2] = 1;
		}
	}
	else{
		if(timer1_flag){
			setTimer1(300);
			if(temp2 > 0){
				display_7Seg1(temp2);
				temp2 --;
			}
			else{
				temp2 = timeAmber;
			}
		}
	}
}

void func_mode4(){
	display_7Seg2(4);
	if(timer0_flag){
		setTimer0(300);
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);
	}
	if(flagPressButton3 == 0){
		if(is_button_pressed(1)){
			if(press[1]){
				timeGreen++;
				temp3++;
				if(timeGreen > 99) timeGreen = 0;
				press[1] = 0;
			}
		}
		else{
			press[1] = 1;
		}
		display_7Seg1(timeGreen);
		if(is_button_pressed(2)){
			if(press[2]){
				time_buffer[2] = timeGreen;
				timeTotal = timeRed;
				temp3 = timeGreen;
				reset_12LED();
				flagPressButton3 = 1;
				press[2] = 0;
			}
		}
		else{
			press[2] = 1;
		}
	}
	else{
		if(timer1_flag){
			setTimer1(300);
			if(temp3 > 0){
				display_7Seg1(temp3);
				temp3 --;
			}
			else{
				temp3 = timeGreen;
			}
		}
	}
}
void fsm_for_input_processing ( void ){
	switch ( mode ){
		case MODE1 :
			if( is_button_pressed (0) ){
				if(press[0] == 1){
					mode = MODE2;
					reset_12LED();
					press[0] = 0;
				}
			}
			else{
				press[0] = 1;
			}
			func_mode1();
			break ;
		case MODE2 :
			if( is_button_pressed (0) ){
				if(press[0] == 1){
					mode = MODE3;
					reset_12LED();
					if(flagPressButton3 == 0){
						timeRed = time_buffer[0];
					} else flagPressButton3 = 0;
					temp1 = timeRed;
					press[0] = 0;
				}
			}
			else{
				press[0] = 1;
			}
			func_mode2();
			break ;
		case MODE3 :
			if( is_button_pressed (0) ){
				if(press[0] == 1){
					mode = MODE4;
					reset_12LED();
					if(flagPressButton3 == 0){
						timeRed = time_buffer[0];
					} else flagPressButton3 = 0;
					temp2 = timeAmber;
					press[0] = 0;
				}
			}
			else{
				press[0] = 1;
			}
			func_mode3();
			break ;
		case MODE4:
			if( is_button_pressed (0) ){
				if(press[0] == 1){
					mode = MODE1;
					reset_12LED();
					if(flagPressButton3 == 0){
						timeRed = time_buffer[0];
					} else flagPressButton3 = 0;
					temp3 = timeGreen;
					press[0] = 0;
				}
			}
			else{
				press[0] = 1;
			}
			func_mode4();
			break ;
	}
}

