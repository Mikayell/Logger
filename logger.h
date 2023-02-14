#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
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

std::vector<std::string> LevelStr{"[Trace]\t", "[Debug]\t", "[Info]\t", "[Warn]\t", "[Erroe]\t", "[Fatal]\t"};


class Logger
{
	
public:

	static void SetLevel(LogLevel newLevel)
	{
		level = newLevel;
	}
	
	template <typename... Args>
	static void Trace(std::string msg, Args... args)
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
	static void Warn(std::string msg, Args... args)
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
	
	static void EnableFileOutput()
	{
		if(file)
		{
			file.close();
		}
		
		filePath = "log.txt";
		file.open(filePath, std::ios::app);
		if(!file)
		{
			perror("Error opening file");
			std::cout << std::endl;
		}
	}
	static void EnableFileOutput(std::string newPath)
	{
		if(!file.is_open())
		{
			file.close();
		}

		filePath = newPath;
		file.open(filePath, std::ios::app);

		if(!file.is_open())
		{
			perror("Error opening file");
			std::cout << std::endl;
		}

	}

	static void CloseFile()
	{
		file.close();
		
	}

private:
	static LogLevel level;
	static std::string filePath;
	static std::fstream file;

private:

	template<typename... Args>
	static constexpr void write(Args&&... args) noexcept
	{
		((Logger::file << std::forward<Args>(args) << " "), ...);
	}

	template<typename... Args>
	static constexpr void print(Args&&... args) noexcept
	{
   		((std::cout << std::forward<Args>(args) << " "), ...);
	}

	template<typename... Args>
	static void log(LogLevel lvl, std::string level_str, std::string msg, Args... args)
	{

		auto time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		std::string tm = std::string(std::ctime(&time));
		
		if(level < lvl)
		{
			std::cout << tm.substr(0, tm.length() - 1) << "\t";
			std::cout << level_str << "\t";
			print(msg, args...);
			std::cout << std::endl;
		}

		if(file.is_open())
		{
			
			file << tm.substr(0, tm.length() - 1) << "\t";
			file << level_str;
			write(msg, args...);
			file << "\n";
		}
	}
	
};

LogLevel Logger::level = InfoLevel;
std::string Logger::filePath;
std::fstream Logger::file;

#endif
