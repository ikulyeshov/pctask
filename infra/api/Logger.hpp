/*
 * Logger.cpp
 *
 *  Created on: Apr 14, 2016
 *      Author: ihor.kulyeshov
 */

#ifndef API_LOGGER_CPP_
#define API_LOGGER_CPP_

#include <stdio.h>
#include <stdarg.h>


#define TRACE_LEVEL_DEBUG			3
#define TRACE_LEVEL_INFO			2
#define TRACE_LEVEL_ERROR			1
#define TRACE_LEVEL_NO_TRACE		0


#ifndef LOGGER_MODULE
#define LOGGER_MODULE "global"
#endif

#ifndef TRACE_LEVEL
#define TRACE_LEVEL TRACE_LEVEL_DEBUG
#endif

#define ps_log_error(fmt, ...) ps_log(TRACE_LEVEL_ERROR, "-E- " fmt, ##__VA_ARGS__)
#define ps_log_info(fmt, ...)  ps_log(TRACE_LEVEL_INFO,  "-I- " fmt, ##__VA_ARGS__)
#define ps_log_debug(fmt, ...) ps_log(TRACE_LEVEL_DEBUG, "-D- " fmt, ##__VA_ARGS__)

#define ps_log(level, fmt, ...) \
		{\
			ps_printf(level, LOGGER_MODULE ": " fmt, ##__VA_ARGS__);\
			ps_printf(level, "\n");\
		}

static inline void ps_printf(unsigned level, const char *fmt, ...)
{
	if (level > TRACE_LEVEL) return;

    va_list argptr;
    va_start(argptr, fmt);
    vprintf(fmt, argptr);
    va_end(argptr);
}


#endif /* API_LOGGER_CPP_ */
