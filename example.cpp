#include "logger.h"

int main()
{
	Logger::SetFileOutput();
	Logger::SetLevel(TraceLevel);
	Logger::Trace("Trace log");
	Logger::Warn("warning info " , "implicit cast");
	
}
