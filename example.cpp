#include "logger.h"

int main()
{
	Logger::EnableFileOutput();
	Logger::SetLevel(TraceLevel);
	Logger::Trace("Trace log", 3466);
	Logger::Warn("warning info " , "implicit cast");
	
}
