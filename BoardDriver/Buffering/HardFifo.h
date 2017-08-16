/*
 * HardFifo.h
 *
 *  Created on: ?? ÂÈÇä ???? åž.Ô.
 *      Author: mazarei
 */

#ifndef BUFFERING_HARDFIFO_H_
#define BUFFERING_HARDFIFO_H_

#include <stdint.h>


class HardFifo {
public:
	HardFifo(uint16_t Buffer_Size=64);
	virtual ~HardFifo();

	//bool incert(uint8_t data);
	void incert(uint8_t data);
	bool Read(uint8_t &data);
	void flush(void);
	uint16_t Counter(void);
	bool IS_Overflow(void);

protected:
	uint8_t *Buffer;
	uint16_t rx_wr_index;
	uint16_t rx_rd_index;
	uint16_t rx_counter;
	bool rx_buffer_overflow;
	uint16_t RX_BUFFER_SIZE;
};

#endif /* BUFFERING_HARDFIFO_H_ */
