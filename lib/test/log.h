#ifndef LOG_H_
#define LOG_H_

#include <stdarg.h>

int log_init(bool print_pid);

void log_destroy(void);

void log_print(const char *__restrict fmt, ...) 
__attribute__((format(printf,1,2)));

#endif /* LOG_H_ */