#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <ctime>
#include <utility>
#include <iomanip>
#include <mutex>

namespace tmp
{
	tm localtime(const std::time_t& time)
	{
		std::tm tm_snapshot;
	#if (defined(WIN32) || defined(_WIN32) || defined(__WIN32__))
		localtime_s(&tm_snapshot, &time);
	#else
		localtime_r(&time, &tm_snapshot);
	#endif
		return tm_snapshot;
	}

	std::string put_time(const std::tm* date_time, const char* c_time_format)
	{
	#if (defined(WIN32) || defined(_WIN32) || defined(__WIN32__))
		std::ostringstream oss;
		oss << std::put_time(const_cast<std::tm*>(date_time), c_time_format);
		return oss.str();
	#else
		const size_t size = 1024;
  		char buffer[size];
  		auto success = std::strftime(buffer, size, c_time_format, date_time);
		if (0 == success)
    		return c_time_format; 
  		return buffer;
	#endif
	}

	std::time_t systemtime_now()
	{
  		auto system_now = std::chrono::system_clock::now();
  		return std::chrono::system_clock::to_time_t(system_now);
	}

}
enum LogLevel
{
	TraceLevel,
	DebugLevel,
	InfoLevel,
	WarnLevel,
	ErrorLevel,
	FatalLevel
};

std::vector<std::string> LevelStr{"[Trace]\t", "[Debug]\t", "[Info]\t", "[Warn]\t", "[Error]\t", "[Fatal]\t"};


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
		log(TraceLevel, LevelStr[0], msg, args...);
	}
	
	template <typename... Args>
	static void Debug(std::string msg, Args... args)
	{
		log(DebugLevel, LevelStr[1], msg, args...);
	}
	
	template <typename... Args>
	static void Info(std::string msg, Args... args)
	{
		log(InfoLevel, LevelStr[2], msg, args...);
	}
	
	template <typename... Args>
	static void Warn(std::string msg, Args... args)
	{
		log(WarnLevel, LevelStr[3], msg, args...);
	}
	
	template <typename... Args>
	static void Error(std::string msg, Args... args)
	{
		log(ErrorLevel, LevelStr[4], msg, args...);
	}
	
	template <typename... Args>
	static void Fatal(std::string msg, Args... args)
	{
		log(FatalLevel, LevelStr[5], msg, args...);
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
	static std::mutex _mutex;
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
		std::tm t = tmp::localtime(tmp::systemtime_now());
		if(level <= lvl)
		{	
			std::lock_guard lock(_mutex);
			std::cout << tmp::put_time(&t, "%F %T") << "\t";
			std::cout << level_str << "\t";
			print(msg, args...);
			std::cout << std::endl;
		}

		if(file.is_open())
		{
			
			file << tmp::put_time(&t, "%F %T") << "\t";
			file << level_str;
			write(msg, args...);
			file << "\n";
		}
	}
	
};

LogLevel Logger::level = InfoLevel;
std::string Logger::filePath;
std::fstream Logger::file;
std::mutex Logger::_mutex;

#endif
