#include <thread>
#include <functional>
#include "logger.h"

void log(int num)
{
	Logger::Error("error: ", num);
}

int main()
{
	Logger::EnableFileOutput();
	Logger::SetLevel(TraceLevel);
	Logger::Trace("Trace log", 3466);
	Logger::Warn("warning info " , "implicit cast");
	std::thread threads[10];
	for(int i = 0; i < 10; ++i)
	{
		threads[i] = std::thread(std::ref(log), i);
	}
	for(int i = 0; i < 10; ++i)
	{
		threads[i].join();
	}
	Logger::CloseFile();
}
