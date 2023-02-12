#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <utility>

enum LogLevel
{
	TraceLevel,
	DebugLevel,
	InfoLevel,
	WarnLevel,
	ErrorLevel,
	FatalLevel
};

std::vector<const char*> LevelStr{"[Trace]\t", "[Debug]\t", "[Info]\t", "[Warn]\t", "[Erroe]\t", "[Fatal]\t"};

template<typename... Args>
constexpr void print(Args&&... args) noexcept
{
   ((std::cout << std::forward<Args>(args) << " "), ...);
}

class Logger
{
	
public:
	static void SetLevel(LogLevel newLevel)
	{
		level = newLevel;
	}
	
	template <typename... Args>
	static void Trace(const char* msg, Args... args)
	{
		log(WarnLevel, LevelStr[0], msg, args...);
	}
	
	template <typename... Args>
	static void Debug(std::string msg, Args... args)
	{
		log(WarnLevel, LevelStr[1], msg, args...);
	}
	
	template <typename... Args>
	static void Info(std::string msg, Args... args)
	{
		log(WarnLevel, LevelStr[2], msg, args...);
	}
	
	template <typename... Args>
	static void Warn(const char* msg, Args... args)
	{
		log(WarnLevel, LevelStr[3], msg, args...);
	}
	
	template <typename... Args>
	static void Error(std::string msg, Args... args)
	{
		log(WarnLevel, LevelStr[4], msg, args...);
	}
	
	template <typename... Args>
	static void Fatal(std::string msg, Args... args)
	{
		log(WarnLevel, LevelStr[5], msg, args...);
	}
	
	static void SetFileOutput()
	{
		if(file)
		{
			file -> close();
			file = nullptr;
		}
		
		filePath = "log.txt";
		// file -> open(filePath, std::ios::app);
		std::fstream tmp(filePath, std::ios::app);
		file = &tmp;
		if(!file)
		{
			perror("Error opening file");
			std::cout << std::endl;
		}
	}
	static void SetFileOutput(const char* newPath)
	{
		if(!file -> is_open())
		{
			file -> close();
		}

		filePath = newPath;
		file -> open(filePath, std::ios::app);

		if(!file -> is_open())
		{
			perror("Error opening file");
			std::cout << std::endl;
		}

	}

	static void CloseFile()
	{
		file -> close();
		file = nullptr;
	}


private:
	static LogLevel level;
	template<typename... Args>
	static void log(LogLevel lvl, const char* level_str, const char* msg, Args... args)
	{
		std::time_t cur_time = std::time(0);
		std::tm* p = std::localtime(&cur_time);
		char tbuffer[100];
		strftime(tbuffer, 100, "%c", p);

		if(level < lvl)
		{
			std::cout << tbuffer << "\t";
			std::cout << level_str << "\t";
			print(msg, args...);
			std::cout << std::endl;
		}

		if(file -> is_open())
		{
			// 
			// fprintf(file, "%s", level_str);
			// fprintf(file, "%s", msg);
			// fprintf(file, "\n");
		}
	}
	static const char* filePath;
	static std::fstream* file;
	
	

};

LogLevel Logger::level = InfoLevel;
const char* Logger::filePath = 0;
std::fstream* Logger::file = nullptr;

#endif
