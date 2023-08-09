
#include <logSTM.h>
#include "stm32g0xx_ll_usart.h"
#define PRINT_BUFFER_SIZE 100

static void logSTM_printf_valist(const char *fmt, va_list argp);
static void logSTM_printf(const char *fmt, ...);
static void logSTM_uart_transmit_buffer(uint8_t* data, uint32_t size);


/*-------------------------------------------------*/
//Global Function
/*-------------------------------------------------*/

/**logSTM() generic LOG massage
 * @param uint8_t type - mark type log massage
 * @param *fmt String to print
 * @param argp Parameters list
 */
void logSTM(uint8_t level, const char *fmt, va_list argp)
{
	logSTM_printf("%c - ", level);
	logSTM_printf_valist(fmt, argp);
}

/** logI() generic LOG  "Info" massage
 * @param *fmt String to print
 * @param ... Parameters list
 */
void logI(const char* fmt, ...)
{
	va_list argp;

	va_start(argp, fmt);
	logSTM('I', fmt, argp);
	va_end(argp);
}

/** logE() generic LOG  "Error" massage
 * @param *fmt String to print
 * @param .. Parameters list
 */
void logE(const char* fmt, ...)
{
	va_list argp;

	va_start(argp, fmt);
	logSTM('E', fmt, argp);
	va_end(argp);
}

/*-------------------------------------------------*/
//Local Function
/*-------------------------------------------------*/

static void logSTM_printf_valist(const char *fmt, va_list argp)
{
	char string[PRINT_BUFFER_SIZE];

	if (vsprintf(string, fmt, argp) > 0)
	{
		logSTM_uart_transmit_buffer((uint8_t*)string, strlen(string));
	}
	else
	{
		logSTM_uart_transmit_buffer((uint8_t*)"E - Print\n", 14);
	}
}

static void logSTM_printf(const char *fmt, ...)
{
	va_list argp;

	va_start(argp, fmt);
	logSTM_printf_valist(fmt, argp);
	va_end(argp);
}

static void logSTM_uart_transmit_buffer(uint8_t* data, uint32_t size)
{
	for (uint32_t i = 0; i < size; i++)
	{
		while (!LL_USART_IsActiveFlag_TXE(USART2));
		LL_USART_TransmitData8(USART2, data[i]);
	}
}
