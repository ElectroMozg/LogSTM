/*
 * log.c
 *
 *  Created on: 2 авг. 2023 г.
 *      Author: user
 */


#include <logSTM.h>
#include "stm32g0xx_ll_usart.h"
#define PRINT_BUFFER_SIZE 100


static void uart_transmit_buffer(uint8_t* data, uint32_t size);

/** Custom printf function in order to use HAL_UART_Transmit()
 * @param *fmt String to print
 * @param argp Parameters list
 */
void HAL_printf_valist(const char *fmt, va_list argp) {
  char string[PRINT_BUFFER_SIZE];

  if (vsprintf(string, fmt, argp) > 0) {
	  uart_transmit_buffer((uint8_t*)string, strlen(string));
  } else {
    uart_transmit_buffer((uint8_t*)"E - Print\n", 14);
  }
}

/** Custom printf function, only translate to va_list arg HAL_UART.
 * @param *fmt String to print
 * @param ... Data
 */
void HAL_printf(const char *fmt, ...) {
  va_list argp;

  va_start(argp, fmt);
  HAL_printf_valist(fmt, argp);
  va_end(argp);
}

/** Generic LOG procedure
 * @param Log level
 * @param *fmt String to print
 * @param argp Parameters list
 */
void logSTM(uint8_t level, const char *fmt, va_list argp) {
	HAL_printf("%c - ", level);
	HAL_printf_valist(fmt, argp);
}

/** LOG procedure - Info
 * @param *fmt String to print
 * @param ... Parameters list
 */
void logI(const char* fmt, ...) {
  va_list argp;

	va_start(argp, fmt);
	logSTM('I', fmt, argp);
	va_end(argp);
}

/** LOG procedure - Error
 * @param *fmt String to print
 * @param .. Parameters list
 */
void logE(const char* fmt, ...) {
  va_list argp;

  va_start(argp, fmt);
  logSTM('E', fmt, argp);
  va_end(argp);
}

static void uart_transmit_buffer(uint8_t* data, uint32_t size)
{
    for (uint32_t i = 0; i < size; i++)
    {
        // Проверка, готов ли UART для передачи данных
        while (!LL_USART_IsActiveFlag_TXE(USART2));

        // Запись данных в регистр передачи UART
        LL_USART_TransmitData8(USART2, data[i]);
    }
}
