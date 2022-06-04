#pragma once
#include <string>
#include "GeneralTimer.h"
#include <SDL.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>

using std::string;
using std::stringstream;

enum class LoggingLevels {
	DEBUG,
	INFO,
	WARNING,
	ERROR,
	COUNT
};

class Logger
{
private:
	static inline bool toConsole = true;
	static inline bool toFile = false;
	static inline LoggingLevels loggingLevel = LoggingLevels::DEBUG;
	static inline std::ofstream file;
public: 
	static inline void setLevel(LoggingLevels level);
	template<typename T, typename... Args> inline static void logDebug(T first, Args... args);
	template<typename T, typename... Args> inline static void logInfo(T first, Args... args);
	template<typename T, typename... Args> inline static void logWarning(T first, Args... args);
	template<typename T, typename... Args> inline static void logError(T first, Args... args);

	static inline void setHandler(bool toConsole, bool toFile);

	static inline bool getToConsole();
	static inline bool getToFile();
	static inline void close();

private:
	template<typename T, typename... Args> inline static void emit(LoggingLevels level, T first, Args... args);
};



void Logger::setLevel(LoggingLevels level)
{
	loggingLevel = level;
}

template<typename T, typename... Args> void Logger::logDebug(T first, Args... args)
{
	emit(LoggingLevels::DEBUG, first, args...);
}
template<typename T, typename... Args> void Logger::logInfo(T first, Args... args)
{
	emit(LoggingLevels::INFO, first, args...);
}
template<typename T, typename... Args> void Logger::logWarning(T first, Args... args)
{
	emit(LoggingLevels::WARNING, first, args...);
}
template<typename T, typename... Args> void Logger::logError(T first, Args... args)
{
	emit(LoggingLevels::ERROR, first, args...);
}

void Logger::setHandler(bool toConsole, bool toFile)
{
	Logger::toConsole = toConsole;
	Logger::toFile = toFile;
}

template<typename T> string str_concat(T v)
{
	stringstream ss;
	ss << v << " ";
	return ss.str();
}

template<typename T, typename... Args>string str_concat(T first, Args... args)
{
	stringstream ss;
	ss << first << " ";
	return ss.str() + str_concat(args...);
}

template<typename T, typename... Args> void Logger::emit(LoggingLevels level, T first, Args... args)
{
	std::string levelName;
	switch (level) {
	case LoggingLevels::INFO:
		levelName = "INFO";
		break;
	case LoggingLevels::DEBUG:
		levelName = "DEBUG";
		break;
	case LoggingLevels::WARNING:
		levelName = "WARNING";
		break;
	case LoggingLevels::ERROR:
		levelName = "ERROR";
		break;
	}

	auto message = str_concat(first, args...);

	std::stringstream fullMessage;
	fullMessage << "[" + levelName + "] " << std::fixed << std::setprecision(3)
		<< ((double)SDL_GetTicks()) / 1000.0 << "s " << message << "\n";

	if (level >= loggingLevel && toConsole)
	{
		std::cout << fullMessage.str();
	}
	if (level >= loggingLevel && toFile)
	{
		if (file.is_open() == false)
		{
			file.open("log/log.txt", std::ios::in | std::ios::out | std::ofstream::trunc);
		}
		if (file.is_open() == false) {
			std::cout << "Could not open a log file, opening a LogError.txt file instead" << std::endl;
			file.open("logError.txt", std::ios::in | std::ios::out | std::ofstream::trunc);
		}
		if (file.good())
		{
			file << fullMessage.str();
		}
		else
		{
			std::cout << "Could not access a log file" << std::endl;
		}
	}
}

void Logger::close()
{
	file.close();
}

bool Logger::getToFile()
{
	return toFile;
}

bool Logger::getToConsole()
{
	return toConsole;
}
