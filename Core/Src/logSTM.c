/*
 * log.c
 *
 *  Created on: 2 авг. 2023 г.
 *      Author: user
 */


#include <logSTM.h>
#include "stm32g0xx_hal.h"
extern UART_HandleTypeDef huart2;
#define PRINT_BUFFER_SIZE 100

/** Custom printf function in order to use HAL_UART_Transmit()
 * @param *fmt String to print
 * @param argp Parameters list
 */
void HAL_printf_valist(const char *fmt, va_list argp) {
  char string[PRINT_BUFFER_SIZE];

  if (vsprintf(string, fmt, argp) > 0) {
    HAL_UART_Transmit(&huart2, (uint8_t*)string, strlen(string), HAL_MAX_DELAY); // send message via UART
  } else {
	HAL_UART_Transmit(&huart2, (uint8_t*)"E - Print\n", 14, HAL_MAX_DELAY);
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
