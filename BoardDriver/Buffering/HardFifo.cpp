/*
 * HardFifo.cpp
 *
 *  Created on: ?? ÂÈÇä ???? åž.Ô.
 *      Author: mazarei
 */

#include "HardFifo.h"
#include <cstring>

HardFifo::HardFifo(uint16_t Buffer_Size)
{
	RX_BUFFER_SIZE = Buffer_Size;
	Buffer = new uint8_t[RX_BUFFER_SIZE];

	rx_wr_index = 0;
	rx_rd_index = 0;
	rx_counter = 0;
	rx_buffer_overflow = false;
}

HardFifo::~HardFifo() {
	if(Buffer)
		delete Buffer;
}

//bool HardFifo::incert(uint8_t data)
//{
//	Buffer[rx_wr_index++]=data;
//
//   if (rx_wr_index == RX_BUFFER_SIZE) rx_wr_index=0;
//   if (++rx_counter == RX_BUFFER_SIZE)
//   {
//	  rx_counter=0;
//	  rx_buffer_overflow=true;
//	  return false;
//   }
//   return true;
//}


void HardFifo::incert(uint8_t data)
{
	Buffer[rx_wr_index++]=data;

   if (rx_wr_index == RX_BUFFER_SIZE) rx_wr_index=0;
   if (++rx_counter == RX_BUFFER_SIZE)
   {
	  rx_counter=0;
	  rx_buffer_overflow=true;
	  return ;
   }
}

bool HardFifo::Read(uint8_t &data)
{
	if(rx_counter==0) return false;
	data=Buffer[rx_rd_index++];

	if (rx_rd_index == RX_BUFFER_SIZE) rx_rd_index=0;
	--rx_counter;

return true;
}


void HardFifo::flush(void)
{
	memset(Buffer,0, RX_BUFFER_SIZE);
	rx_wr_index = 0;
	rx_rd_index = 0;
	rx_counter = 0;
	rx_buffer_overflow = false;
}


uint16_t HardFifo::Counter(void)
{
	return rx_counter;
}

bool HardFifo::IS_Overflow(void)
{
	return rx_buffer_overflow;
}
