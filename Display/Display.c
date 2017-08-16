/*
 * Display.c
 *
 *  Created on: Aug 12, 2017
 *      Author: Mazarei
 */
#include <registers.h>
#include <Display.h>
#include <stm32f4xx.h>

#define LCD_REG      	(((volatile unsigned short *) 0x60000000))
#define LCD_DATA      	(((volatile unsigned short *) 0x60020000))
// Initialization command tables for different LCD controllers
#define TFTLCD_DELAY 0xFF
volatile LCDType_t	LType = Unkwon;


static const uint16_t SSD1289_regValues[] =
{
		SSD1289_OSCSTART	, 0x0001,
		SSD1289_PWRCTRL1	, 0xA8A4,
		SSD1289_PWRCTRL2	, 0x0000,
		SSD1289_PWRCTRL3	, 0x080C,
		SSD1289_PWRCTRL4	, 0x2B00,
		SSD1289_PWRCTRL5	, 0x00B7,
		SSD1289_OUTCTRL		, 0x2B3F,
		SSD1289_ACCTRL		, 0x0600,
		SSD1289_SLEEP		, 0x0000,
		SSD1289_ENTRY		, 0x6070,
		SSD1289_CMP1		, 0x0000,
		SSD1289_CMP2		, 0x0000,
		SSD1289_HPORCH		, 0xEF1C,
		SSD1289_VPORCH		, 0x0003,
		SSD1289_DSPCTRL		, 0x0233,
		SSD1289_FCYCCTRL	, 0x0000,
		SSD1289_GSTART		, 0x0000,
		SSD1289_VSCROLL1	, 0x0000,
		SSD1289_VSCROLL2	, 0x0000,
		SSD1289_W1START		, 0x0000,
		SSD1289_W1END		, 0x013F,
		SSD1289_W2START		, 0x0000,
		SSD1289_W2END		, 0x0000,
		SSD1289_HADDR		, 0xEF00,
		SSD1289_VSTART		, 0x0000,
		SSD1289_VEND		, 0x013F,
		SSD1289_GAMMA1		, 0x0707,
		SSD1289_GAMMA2		, 0x0204,
		SSD1289_GAMMA3		, 0x0204,
		SSD1289_GAMMA4		, 0x0502,
		SSD1289_GAMMA5		, 0x0507,
		SSD1289_GAMMA6		, 0x0204,
		SSD1289_GAMMA7		, 0x0204,
		SSD1289_GAMMA8		, 0x0502,
		SSD1289_GAMMA9		, 0x0302,
		SSD1289_GAMMA10		, 0x0302,
		SSD1289_WRMASK1		, 0x0000,
		SSD1289_WRMASK2		, 0x0000,
		SSD1289_FFREQ		, 0x8000,
		SSD1289_YADDR		, 0x0000,
		SSD1289_XADDR		, 0x0000,
};


static const uint16_t ILI932x_regValues[] =
{
  ILI932X_START_OSC        , 0x0001, // Start oscillator
  TFTLCD_DELAY             , 50,     // 50 millisecond delay
  ILI932X_DRIV_OUT_CTRL    , 0x0100,
  ILI932X_DRIV_WAV_CTRL    , 0x0700,
  ILI932X_ENTRY_MOD        , 0x1030,
  ILI932X_RESIZE_CTRL      , 0x0000,
  ILI932X_DISP_CTRL2       , 0x0202,
  ILI932X_DISP_CTRL3       , 0x0000,
  ILI932X_DISP_CTRL4       , 0x0000,
  ILI932X_RGB_DISP_IF_CTRL1, 0x0,
  ILI932X_FRM_MARKER_POS   , 0x0,
  ILI932X_RGB_DISP_IF_CTRL2, 0x0,
  ILI932X_POW_CTRL1        , 0x0000,
  ILI932X_POW_CTRL2        , 0x0007,
  ILI932X_POW_CTRL3        , 0x0000,
  ILI932X_POW_CTRL4        , 0x0000,
  TFTLCD_DELAY             , 200,
  ILI932X_POW_CTRL1        , 0x1690,
  ILI932X_POW_CTRL2        , 0x0227,
  TFTLCD_DELAY             , 50,
  ILI932X_POW_CTRL3        , 0x001A,
  TFTLCD_DELAY             , 50,
  ILI932X_POW_CTRL4        , 0x1800,
  ILI932X_POW_CTRL7        , 0x002A,
  TFTLCD_DELAY             , 50,
  ILI932X_GAMMA_CTRL1      , 0x0000,
  ILI932X_GAMMA_CTRL2      , 0x0000,
  ILI932X_GAMMA_CTRL3      , 0x0000,
  ILI932X_GAMMA_CTRL4      , 0x0206,
  ILI932X_GAMMA_CTRL5      , 0x0808,
  ILI932X_GAMMA_CTRL6      , 0x0007,
  ILI932X_GAMMA_CTRL7      , 0x0201,
  ILI932X_GAMMA_CTRL8      , 0x0000,
  ILI932X_GAMMA_CTRL9      , 0x0000,
  ILI932X_GAMMA_CTRL10     , 0x0000,
  ILI932X_GRAM_HOR_AD      , 0x0000,
  ILI932X_GRAM_VER_AD      , 0x0000,
  ILI932X_HOR_START_AD     , 0x0000,
  ILI932X_HOR_END_AD       , 0x00EF,
  ILI932X_VER_START_AD     , 0X0000,
  ILI932X_VER_END_AD       , 0x013F,
  ILI932X_GATE_SCAN_CTRL1  , 0xA700, // Driver Output Control (R60h)
  ILI932X_GATE_SCAN_CTRL2  , 0x0003, // Driver Output Control (R61h)
  ILI932X_GATE_SCAN_CTRL3  , 0x0000, // Driver Output Control (R62h)
  ILI932X_PANEL_IF_CTRL1   , 0X0010, // Panel Interface Control 1 (R90h)
  ILI932X_PANEL_IF_CTRL2   , 0X0000,
  ILI932X_PANEL_IF_CTRL3   , 0X0003,
  ILI932X_PANEL_IF_CTRL4   , 0X1100,
  ILI932X_PANEL_IF_CTRL5   , 0X0000,
  ILI932X_PANEL_IF_CTRL6   , 0X0000,
  ILI932X_DISP_CTRL1       , 0x0133, // Main screen turn on
};


void SRAM_Init(void)
{
	FSMC_NORSRAMInitTypeDef          FSMC_NORSRAMInitStructure;
	FSMC_NORSRAMTimingInitTypeDef 	 FSMC_NORSRAMTimingInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable GPIOs clock */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE, ENABLE);

  /* Enable FMC clock */
  RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC, ENABLE);

/*-- GPIOs Configuration -----------------------------------------------------*/
/*
	PD0 -> D2
	PD1 -> D3
	PD4 -> OE(RD)
	PD5 -> WE
	PD7 -> NE1
	PD8 -> D13
	PD9 -> D14
	PD10-> D15
	PD11-> RS(A16)
	PD14-> D0
	PD15-> D1

	PE7 -> D4
	PE8 -> D5
	PE9 -> D6
	PE10-> D7
	PE11-> D8
	PE12-> D9
	PE13-> D10
	PE14-> D11
	PE15-> D12

*/

  /* GPIOD configuration */
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource4, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource7, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource10, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource11, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_FSMC);


  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0  | GPIO_Pin_1  | GPIO_Pin_4   | GPIO_Pin_5  | GPIO_Pin_7  |
		  	  	  	  	  	    GPIO_Pin_8  | GPIO_Pin_9  | GPIO_Pin_10  | GPIO_Pin_11 | GPIO_Pin_14 | GPIO_Pin_15 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;

  GPIO_Init(GPIOD, &GPIO_InitStructure);


  /* GPIOE configuration */
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource7 , GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource8 , GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource9 , GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource10 , GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource11 , GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource12 , GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource13 , GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource14 , GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource15 , GPIO_AF_FSMC);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7  | GPIO_Pin_8   | GPIO_Pin_9   | GPIO_Pin_10 |
		  	  	  	  	  	    GPIO_Pin_11 | GPIO_Pin_12  | GPIO_Pin_13  | GPIO_Pin_14 |  GPIO_Pin_15;


  GPIO_Init(GPIOE, &GPIO_InitStructure);


/*-- FMC Configuration ------------------------------------------------------*/
//	FSMC_NORSRAMInitTypeDef FSMC_NORSRAMInitStructure;
//	FSMC_NORSRAMTimingInitTypeDef FSMC_NORSRAMTimingInitStructureRead;
//	FSMC_NORSRAMTimingInitTypeDef FSMC_NORSRAMTimingInitStructureWrite;

//	/* Enable FSMC Clock */
//	RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC, ENABLE);
//
//	/* Define Read timing parameters */
//	FSMC_NORSRAMTimingInitStructureRead.FSMC_AddressSetupTime = 3;
//	FSMC_NORSRAMTimingInitStructureRead.FSMC_AddressHoldTime = 0;
//	FSMC_NORSRAMTimingInitStructureRead.FSMC_DataSetupTime = 15;
//	FSMC_NORSRAMTimingInitStructureRead.FSMC_BusTurnAroundDuration = 0;
//	FSMC_NORSRAMTimingInitStructureRead.FSMC_CLKDivision = 1;
//	FSMC_NORSRAMTimingInitStructureRead.FSMC_DataLatency = 0;
//	FSMC_NORSRAMTimingInitStructureRead.FSMC_AccessMode = FSMC_AccessMode_A;
//
//	/* Define Write Timing parameters */
//	FSMC_NORSRAMTimingInitStructureWrite.FSMC_AddressSetupTime = 3;
//	FSMC_NORSRAMTimingInitStructureWrite.FSMC_AddressHoldTime = 0;
//	FSMC_NORSRAMTimingInitStructureWrite.FSMC_DataSetupTime = 6;
//	FSMC_NORSRAMTimingInitStructureWrite.FSMC_BusTurnAroundDuration = 0;
//	FSMC_NORSRAMTimingInitStructureWrite.FSMC_CLKDivision = 1;
//	FSMC_NORSRAMTimingInitStructureWrite.FSMC_DataLatency = 0;
//	FSMC_NORSRAMTimingInitStructureWrite.FSMC_AccessMode = FSMC_AccessMode_A;
//
//	/* Define protocol type */
//	FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1; //Bank1
//	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable; //No mux
//	FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM; //SRAM type
//	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b; //16 bits wide
//	FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode =
//			FSMC_BurstAccessMode_Disable; //No Burst
//	FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait =
//			FSMC_AsynchronousWait_Disable; // No wait
//	FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity =
//			FSMC_WaitSignalPolarity_Low; //Don'tcare
//	FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable; //No wrap mode
//	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive =
//			FSMC_WaitSignalActive_BeforeWaitState; //Don't care
//	FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
//	FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable; //Don't care
//	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Enable; //Allow distinct Read/Write parameters
//	FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable; //Don't care
//
//	// Set read timing structure
//	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct =
//			&FSMC_NORSRAMTimingInitStructureRead;
//
//	// Set write timing structure
//	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct =
//			&FSMC_NORSRAMTimingInitStructureWrite;
//
//	// Initialize FSMC for read and write
//	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);
//
//	// Enable FSMC
//	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);





	FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;
	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
	FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;
	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
	FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
	FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
	FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Enable;
	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &FSMC_NORSRAMTimingInitStructure;
	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);

	FSMC_NORSRAMTimingInitStructure.FSMC_AddressSetupTime = 1;
	FSMC_NORSRAMTimingInitStructure.FSMC_DataSetupTime = 3;
	FSMC_NORSRAMTimingInitStructure.FSMC_AddressHoldTime = 1;
	FSMC_NORSRAMTimingInitStructure.FSMC_CLKDivision = 1;
	FSMC_NORSRAMTimingInitStructure.FSMC_AccessMode = FSMC_AccessMode_A;// B? This is how he does it in ili9325
	FSMC_NORSRAMTimingInitStructure.FSMC_BusTurnAroundDuration = 0;
	FSMC_NORSRAMTimingInitStructure.FSMC_DataLatency = 0;
	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &FSMC_NORSRAMTimingInitStructure;
	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);

	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);
}

void Tft_initGPIO(void)
{
	GPIO_InitTypeDef init={0};
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);

	// reset
    init.GPIO_Pin=GPIO_Pin_9;                               // reset pin -> PC9
    init.GPIO_Mode=GPIO_Mode_OUT;
    init.GPIO_Speed=GPIO_Speed_100MHz;
    GPIO_Init(GPIOC,&init);

    init.GPIO_Pin=GPIO_Pin_8;                               // IM0 pin -> PA8
    init.GPIO_Mode=GPIO_Mode_OUT;
    init.GPIO_Speed=GPIO_Speed_100MHz;
    GPIO_Init(GPIOA,&init);

    GPIO_ResetBits(GPIOA,GPIO_Pin_8);
}

void Tft_uDelay (const uint32_t usec)
{
  uint32_t count = 0;
  if (usec==0) return;
  const uint32_t utime = (120 * usec / 7);
  do
  {
    if ( ++count > utime )
    {
      return ;
    }
  }
  while (1);
}

void Tft_delayMillis(uint32_t millis)
{
	Tft_uDelay(millis * 1000);
}

// the RS (C/D) pin is high during write
// d[0:7] version
static inline void writeData(uint16_t data)
{
	*LCD_DATA = data;
}

// the RS (C/D) pin is low during write
// d[0:7] version
static inline void writeCommand(uint16_t cmd)
{
    *LCD_REG=(cmd);
}

// d[0:7] version
static uint16_t readData()
{
    uint16_t d = 0;
    d = *LCD_DATA;
    return d;
}

static uint16_t readRegister(uint16_t addr)
{
   writeCommand(addr);
   readData();
   return readData();
}

static inline void writeRegister(uint16_t addr, uint16_t data)
{
    writeCommand(addr);
    Tft_uDelay(10);
    writeData(data);
}

void Tft_reset(void)
{
    GPIO_ResetBits(GPIOC, GPIO_Pin_9);
    Tft_delayMillis(2);
    GPIO_SetBits(GPIOC, GPIO_Pin_9);

  // resync

    writeData(0);
    writeData(0);
    writeData(0);
    writeData(0);
}


void Tft_Init(void)
{
    uint16_t a, d;
    uint8_t i;

    SRAM_Init();
    GPIO_SetBits(GPIOC, GPIO_Pin_9);
    Tft_delayMillis(20);
    Tft_reset();
    Tft_delayMillis(200);


  uint16_t identifier = readRegister(0x0);

  if ((identifier == 0x9325) || (identifier == 0x9328))	/*ili9325 or ili9328 LCD Contorller*/
  {
	  for (i = 0; i < sizeof(ILI932x_regValues) / 4; i++)
	  {
	     a = *(ILI932x_regValues + i*2);
	     d = *(ILI932x_regValues + i*2 + 1);

	     if (a == TFTLCD_DELAY)
	     {
	      	Tft_delayMillis(d);
	     }
	     else
	     {
	        writeRegister(a, d);
	     }
	  }
	  LType = ili932x;
  }
  else if(identifier == 0x0089 || identifier == 0x8989) /*SSD1289 LCD Contorller*/
  {
	  for (i = 0; i < sizeof(SSD1289_regValues) / 4; i++)
	  {
	  	  a = *(SSD1289_regValues + i*2);
	  	  d = *(SSD1289_regValues + i*2 + 1);

	  	  if (a == TFTLCD_DELAY)
	  	  {
	  	   	Tft_delayMillis(d);
	  	  }
	  	  else
	  	  {
	  	     writeRegister(a, d);
	  	  }
	  }
	  writeCommand(0x22);
	  LType = ssd1289;
  }
}


LCDType_t Tft_GetType()
{
	return LType;
}

void Tft_drawPixel(int16_t x, int16_t y, uint16_t color)
{

	writeRegister(ILI932X_GRAM_HOR_AD, x);     // GRAM Address Set (Horizontal Address) (R20h)
	writeRegister(ILI932X_GRAM_VER_AD, y);     // GRAM Address Set (Vertical Address) (R21h)
	writeCommand(ILI932X_RW_GRAM);             // Write Data to GRAM (R22h)
	writeData(color);
}
