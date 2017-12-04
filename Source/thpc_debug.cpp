#include "thpc_debug.h"

void thpc_Debug_PrintToDebugger(const char *text, ...)
{
	char textBuffer[1024], *p = textBuffer;
	va_list arg;
	int n;
	va_start(arg, text);
	n = _vsnprintf_s(textBuffer, sizeof(textBuffer) - 3, _TRUNCATE, text, arg);
	va_end(arg);

	p += (n < 0) ? sizeof(textBuffer) - 3 : n;

	while (p > textBuffer && isspace(p[-1]))
		*--p = '\0';

	*p++ = '\r';
	*p++ = '\n';
	*p = '\0';

	OutputDebugString(textBuffer);
}
