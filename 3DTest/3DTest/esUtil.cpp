#include "esUtil.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <Windows.h>


void esLogMessage(const char *formatStr, ...)
{
	va_list params;
	char buf[2048];

	va_start(params, formatStr);
	vsprintf_s(buf, sizeof(buf), formatStr, params);

	printf("%s", buf);
	OutputDebugString(buf);

	va_end(params);
}