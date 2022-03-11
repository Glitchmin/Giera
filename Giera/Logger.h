#pragma once
#include <string>
#include "GeneralTimer.h"
#include <SDL.h>
#include <fstream>

enum LoggingLevels {
	DEBUG,
	INFO,
	WARNING,
	ERROR,
	COUNT
};

class Logger
{
private:
	static LoggingLevels loggingLevel;
	static bool toConsole;
	static bool toFile;
	static std::ofstream file;
public: 
	static void setLevel(LoggingLevels level);
	static void logDebug(std::string message);
	static void logInfo(std::string message);
	static void logWarning(std::string message);
	static void logError(std::string message);

	static void setHandler(bool toConsole, bool toFile);

	static bool getToConsole();
	static bool getToFile();
	static void close();

private:
	static void emit(LoggingLevels level, std::string message);
};





