#include "DebugLog.h"
#include <iostream>
#include <stdarg.h>


FILE* outFile;
bool consoleDump;
bool fileDump;

namespace Debug
{

	void OpenDebugStream(bool append, bool console, bool file)
	{
		if (append)
			fopen_s(&outFile, "debug.log", "a+");
		else
			fopen_s(&outFile, "debug.log", "w");

		consoleDump = console;
		fileDump = file;
	}


	void DebugOutput(const char *format, ...)
	{
		va_list ap;
		va_start(ap, format);
		if (consoleDump)
			vfprintf(stdout, format, ap);

		if (fileDump)
		{
			vfprintf(outFile, format, ap);
			fflush(outFile);
		}
		va_end(ap);
	}

	void CloseDebugStream()
	{
		fclose(outFile);
	}
}