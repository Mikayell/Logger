#include <thread>
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
	std::vector<std::thread> threads;
	for(int i = 0; i < 10; ++i)
	{
		threads.push_back(std::thread([&]() { log(i); }));
	}
	for(int i = 0; i < 10; ++i)
	{
		threads[i].join();
	}
	Logger::CloseFile();
}
