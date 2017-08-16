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
// ----------------------------------------------------------------------------
//
// Standalone STM32F4 led blink sample (trace via NONE).
//
// In debug configurations, demonstrate how to print a greeting message
// on the trace device. In release configurations the message is
// simply discarded.
//
// Then demonstrates how to blink a led with 1 Hz, using a
// continuous loop and SysTick delays.
//
// Trace support is enabled by adding the TRACE macro definition.
// By default the trace messages are forwarded to the NONE output,
// but can be rerouted to any device or completely suppressed, by
// changing the definitions required in system/src/diag/trace_impl.c
// (currently OS_USE_TRACE_ITM, OS_USE_TRACE_SEMIHOSTING_DEBUG/_STDOUT).
//


// ----- LED definitions ------------------------------------------------------
#warning "Assume a STM32F4-Discovery board, PC12-PC15, active high."





// ----- main() ---------------------------------------------------------------

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"




void vLCDTask( void *pvParameters )
{
	GUI_Init();
	GUI_DispString("Hello world!");

	for( ;; )
	{
		GUI_SetColor(GUI_RED);
		GUI_FillCircle(320/2,240/2,100);
		GUI_SetColor(GUI_BLUE);
		GUI_FillCircle(320/2,240/2,100);
		//vTaskDelay(100);
	}
}



int
main(int argc, char* argv[])
{

	SystemInit();
	SystemCoreClockUpdate();
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC, ENABLE);
	Tft_Init();


  xTaskCreate( vLCDTask,  "LED0", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL );
//  xTaskCreate( vLCDTask1, "LED1", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL );
//  xTaskCreate( vLCDTask2, "LED2", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL );
//  xTaskCreate( vLCDTask3, "LED3", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL );

  vTaskStartScheduler();


 while(1)
 {

 }
}





#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
