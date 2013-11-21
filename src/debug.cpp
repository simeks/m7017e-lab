#include "common.h"

#include "debug.h"

#include <stdarg.h>

void debug::Printf(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);

	char msg[2048];
	vsnprintf(msg, sizeof(msg), fmt, args);
	msg[2047] = '\0';

	printf("%s", msg);

#ifdef _WIN32
	OutputDebugString(msg);
#endif

	va_end(args);
}
