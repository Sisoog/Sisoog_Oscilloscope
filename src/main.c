//
// This file is part of the GNU ARM Eclipse distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

// ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include "diag/Trace.h"
//#include "stm32f4xx_hal.h"


#include "FreeRTOS.h"
#include "task.h"
#include <string.h>
#include "semphr.h"
#include "stm32f4xx_fsmc.h"
#include "stm32f4xx_fmc.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "Display.h"
#include "GUI.h"
#include <Em_Task.h>





// ----- main() ---------------------------------------------------------------

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"




void vLCDTask( void *pvParameters )
{

	GUI_DispString("Hello world!");

	for( ;; )
	{
//		GUI_SetColor(GUI_RED);
//		GUI_FillCircle(320/2,240/2,100);
//		vTaskDelay(100);
//		GUI_SetColor(GUI_BLUE);
//		GUI_FillCircle(320/2,240/2,100);
		vTaskDelay(100);
	}
}



int
main(int argc, char* argv[])
{

	SystemInit();
	SystemCoreClockUpdate();
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC, ENABLE);

	Tft_Init();

	EmWin_Init();
	xTaskCreate( vLCDTask,  "LCD", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL );

	vTaskStartScheduler();


 while(1)
 {

 }
}





#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
