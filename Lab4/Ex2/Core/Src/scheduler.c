/*
 * scheduler.c
 *
 *  Created on: Nov 30, 2021
 *      Author: Asus
 */
#include "timer.h"
#include "scheduler.h"
#include "display.h"

enum typeTask {
	STASK, QTASK
};

static uint8_t count, count_run, TASKID;
static uint32_t RUNME;

void SCH_Init(){
	int i = 0;
	while(i < SCH_MAX_TASKS && i < MAX_QUEUE) {
		SCH_Delete_Task(i);
		pop();
		i++;
	}
	// Reset the global error variable
	// − SCH_Delete_Task () will generate an error code ,
	// ( because the task array is empty)

	count = 0;
	count_run = 0;
	RUNME = 0;
	TASKID = 0;

	Error_code_G = FREE_OF_ERROR;
	Timer_Init();
	Watchdog_Init();
}
void SCH_Update(){
	if(SCH_tasks_G[0].pTask) {
		if(SCH_tasks_G[0].Delay > 0) {
			SCH_tasks_G[0].Delay--;
		}
		if(SCH_tasks_G[0].Delay == 0) {
			SCH_tasks_G[0].RunMe++;
		}
	}
}
void SCH_Dispatch_Tasks(){
	if(SCH_tasks_G[0].pTask && SCH_tasks_G[0].Delay == 0 && SCH_tasks_G[0].RunMe > 0) {
		push(SCH_tasks_G[0]);
		SCH_tasks_G[0].RunMe--;
		if(SCH_tasks_G[0].Period > 0) {
			SCH_tasks_G[0].Delay = SCH_tasks_G[0].Period;
			sTask tmp = SCH_tasks_G[0];
			RUNME = tmp.RunMe;
			SCH_Delete_Task(0);
			SCH_Add_Task(tmp.pTask, tmp.Delay, tmp.Period);
		}
		else {
			SCH_Delete_Task(0);
		}
	}
	if(Run_Tasks_G[0].pTask) {
		display_task(0);
		(*Run_Tasks_G[0].pTask)();
		pop();
	}
	// Report system status
	SCH_Report_Status();
	// The scheduler enters idle mode at this point
	SCH_Go_To_Sleep();
}
uint32_t SCH_Add_Task(void (*pfunction)(), uint32_t DELAY, uint32_t PERIOD){
	if(count == SCH_MAX_TASKS) {
		Error_code_G = ERROR_SCH_TOO_MANY_TASKS;
		return SCH_MAX_TASKS;
	}
	int Index = 0;
	int Slot = -1;
	uint32_t Delay_RT = 0;
	uint32_t tmp = 0;
	while(Index < count){
		if(Slot == -1) {
			tmp = Delay_RT;
			Delay_RT += SCH_tasks_G[Index].Delay;
		}
		if(Delay_RT > DELAY) {
			Slot = Index;
			break;
		}
		Index++;
	}
	count++;
	if(Slot == -1) {
		SCH_tasks_G[Index].pTask = pfunction;
		SCH_tasks_G[Index].Delay = DELAY - Delay_RT;
		SCH_tasks_G[Index].Period = PERIOD;
		SCH_tasks_G[Index].RunMe = RUNME;
		SCH_tasks_G[Index].TaskId = (TASKID != 0)? TASKID : count;
	}
	else {
		SCH_Shift_End_Tasks(Slot, STASK);
//		add task
		SCH_tasks_G[Slot].pTask = pfunction;
		SCH_tasks_G[Slot].Delay = DELAY - tmp;
		SCH_tasks_G[Slot].Period = PERIOD;
		SCH_tasks_G[Slot].RunMe = RUNME;
		SCH_tasks_G[Slot].TaskId = (TASKID != 0)? TASKID : count;
//		tinh toan lai delay cua slot+1
		SCH_tasks_G[Slot + 1].Delay -= SCH_tasks_G[Slot].Delay;
	}
	RUNME = 0;
	TASKID = 0;

	return Index;
}
uint8_t SCH_Delete_Task(uint32_t taskID){
	unsigned char Code_State;
	if(!SCH_tasks_G[taskID].pTask){
		//No task at this location
		//Set the global error variable
		Error_code_G = ERROR_SCH_CANNOT_DELETE_TASK;
		Code_State = _ERROR_;
	}
	else{
		Code_State = _SUCCESS_;
		if(SCH_tasks_G[taskID].Period > 0) {
			TASKID = SCH_tasks_G[taskID].TaskId;
		}
		SCH_Shift_Start_Tasks(taskID, STASK);
		SCH_tasks_G[count-1].pTask = 0x0000;
		SCH_tasks_G[count-1].Delay = 0;
		SCH_tasks_G[count-1].Period = 0;
		SCH_tasks_G[count-1].RunMe = 0;
		SCH_tasks_G[count-1].TaskId = 0;
		count--;
	}
	return Code_State;
}


uint32_t push(sTask task) {
	if(count_run == MAX_QUEUE) return 0;
	Run_Tasks_G[count_run].pTask = task.pTask;
	Run_Tasks_G[count_run].TaskId = task.TaskId;
	count_run++;
	return 1;
}
uint8_t pop() {
	if(count_run == 0) return 0;
	SCH_Shift_Start_Tasks(0, QTASK);
	Run_Tasks_G[count_run - 1].pTask = 0x0000;
	Run_Tasks_G[count_run - 1].TaskId = 0;
	count_run--;
	return 1;
}



void Timer_Init(){

}
void Watchdog_Init(){

}
void SCH_Report_Status(){
#ifdef SCH_REPORT_ERRORS

#define Error_Port GPIOA
#endif
}
void SCH_Go_To_Sleep(){

}

void SCH_Shift_End_Tasks(uint32_t Index, int typeArr) {
	if(typeArr == STASK) {
		for(int i = count - 1; i > Index; i--) {
			SCH_tasks_G[i] = SCH_tasks_G[i-1];
		}
	}
	else if(typeArr == QTASK) {
		for(int i = count_run - 1; i > Index; i--) {
			Run_Tasks_G[i] = Run_Tasks_G[i-1];
		}
	}
}
void SCH_Shift_Start_Tasks(uint32_t Index, int typeArr) {
	if(typeArr == STASK) {
		for(int i = Index; i < count-1; i++) {
			SCH_tasks_G[i] = SCH_tasks_G[i+1];
		}
	}
	else if(typeArr == QTASK) {
		for(int i = Index; i < count_run-1; i++) {
			Run_Tasks_G[i] = Run_Tasks_G[i+1];
		}
	}
}

