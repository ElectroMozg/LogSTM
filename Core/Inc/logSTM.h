/*
 * log.h
 *
 *  Created on: 2 авг. 2023 г.
 *      Author: user
 */

#ifndef INC_LOGSTM_H_
#define INC_LOGSTM_H_

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

void HAL_printf_valist(const char *fmt, va_list argp);

/** Custom printf function, only translate to va_list arg HAL_UART.
 * @param *fmt String to print
 * @param ... Data
 */
void HAL_printf(const char *fmt, ...);

/** Generic LOG procedure
 * @param Log level
 * @param *fmt String to print
 * @param argp Parameters list
 */
void logSTM(uint8_t level, const char *fmt, va_list argp);
/** LOG procedure - Info
 * @param *fmt String to print
 * @param ... Parameters list
 */
void logI(const char* fmt, ...);

/** LOG procedure - Error
 * @param *fmt String to print
 * @param .. Parameters list
 */
void logE(const char* fmt, ...);


#endif /* INC_LOGSTM_H_ */
