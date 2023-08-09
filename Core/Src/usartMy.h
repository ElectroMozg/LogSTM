#ifndef SRC_USARTMY_H_
#define SRC_USARTMY_H_

#include <stdint.h>

void uart_init(void);
void uart_transmit_buffer(uint8_t* data, uint32_t size);
void uart_rx_check(void);

#endif /* SRC_USARTMY_H_ */
