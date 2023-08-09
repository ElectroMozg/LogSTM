#ifndef INC_LOGSTM_H_
#define INC_LOGSTM_H_

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

/**logSTM generic LOG massage
 * @param uint8_t type - mark type log massage
 * @param *fmt String to print
 * @param argp Parameters list
 */
void logSTM(uint8_t type, const char *fmt, va_list argp);

/** logI generic LOG  "Info" massage
 * @param *fmt String to print
 * @param ... Parameters list
 */
void logI(const char* fmt, ...);

/** logE generic LOG  "Error" massage
 * @param *fmt String to print
 * @param .. Parameters list
 */
void logE(const char* fmt, ...);


#endif /* INC_LOGSTM_H_ */
