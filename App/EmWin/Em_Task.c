/*
 * Em_Task.c
 *
 *  Created on: Aug 16, 2017
 *      Author: Mazarei
 */
#include <App_Config.h>
#include <FreeRTOS.h>
#include <task.h>
#include <GUI.h>
#include <Em_Task.h>



void EmWin_Task(void *Param);

void EmWin_Init(void)
{
		xTaskCreate(EmWin_Task, "EmWin", EmWin_TASK_Stak/sizeof(int), NULL, EmWin_PRIORITY, NULL);
}



void EmWin_Task(void *Param)
{
	GUI_Init();
	GUI_DispString("Hello world!");
	while(1)
	{

		GUI_Exec();		  /* Do the background work ... Update windows etc.) */
		GUI_X_ExecIdle(); /* Nothing left to do for the moment ... Idle processing */
	}
}


