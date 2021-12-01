/*
 * scheduler.h
 *
 *  Created on: Nov 30, 2021
 *      Author: Asus
 */

#include "main.h"

#ifndef INC_SCHEDULER_H_
#define INC_SCHEDULER_H_

typedef struct{
//	Pointer to task
	void (*pTask)(void);
//	Delay (ticks) until the function will be run
	uint32_t Delay;
//	Interval (ticks) between subsequent runs
	uint32_t Period;
//	Incremented when task is due to execute
	uint32_t RunMe;
//	Id task
	uint8_t TaskId;
} sTask;

typedef struct{
//	Pointer to task
	void (*pTask)(void);
//	Id task
	uint8_t TaskId;
} qTask;

enum Error{
	FREE_OF_ERROR,
	ERROR_SCH_TOO_MANY_TASKS,
	ERROR_SCH_WAITING_FOR_SLAVE_TO_ACK,
	ERROR_SCH_WAITING_FOR_START_COMMAND_FROM_MASTER,
	ERROR_SCH_ONE_OR_MORE_SLAVES_DID_NOT_START,
	ERROR_SCH_LOST_SLAVE,
	ERROR_SCH_CAN_BUS_ERROR,
	ERROR_I2C_WRITE_BYTE_AT24C64,
	ERROR_SCH_CANNOT_DELETE_TASK
};

#define SCH_MAX_TASKS 5
#define MAX_QUEUE 10
#define NO_TASK_ID 0
#define _SUCCESS_ 1
#define _ERROR_ 0
#define TIMER_CYCLE 10

sTask SCH_tasks_G[SCH_MAX_TASKS];
qTask Run_Tasks_G[MAX_QUEUE];

uint32_t Error_code_G;


void SCH_Init(void);
void SCH_Update(void);
void SCH_Dispatch_Tasks(void);
uint32_t SCH_Add_Task(void (*pfunction)(void), uint32_t DELAY, uint32_t PERIOD);
uint8_t SCH_Delete_Task(uint32_t taskID);
uint32_t push(sTask task);
uint8_t pop();
void SCH_Shift_End_Tasks(uint32_t Index, int typeArr);
void SCH_Shift_Start_Tasks(uint32_t Index, int typeArr);

//================= optional ======================

void Timer_Init();
void Watchdog_Init();
// Report system status
void SCH_Report_Status();
// The scheduler enters idle mode at this point
void SCH_Go_To_Sleep();


#endif /* INC_SCHEDULER_H_ */
