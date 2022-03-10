#include "Logger.h"
#include <iostream>
#include <string>
#include <sstream>
bool Logger::toConsole = true;
bool Logger::toFile = false;
LoggingLevels Logger::loggingLevel = DEBUG;
std::fstream Logger::file;

void Logger::setLevel(LoggingLevels level)
{
	Logger::loggingLevel = level;
}

void Logger::logDebug(std::string message)
{
	emit(DEBUG, message);
}
void Logger::logInfo(std::string message)
{
	emit(INFO, message);
}
void Logger::logWarning(std::string message)
{
	emit(WARNING, message);
}
void Logger::logError(std::string message)
{
	emit(ERROR, message);
}

void Logger::setHandler(bool toConsole, bool toFile)
{
	Logger::toConsole = toConsole;
	Logger::toFile = toFile;
}


void Logger::emit(LoggingLevels level, std::string message)
{
	std::string levelName;
	switch (level) {
	case INFO:
		levelName = "INFO";
		break;
	case DEBUG:
		levelName = "DEBUG";
		break;
	case WARNING:
		levelName = "WARNING";
		break;
	case ERROR:
		levelName = "ERROR";
		break;
	}

	std::stringstream fullMessage;
	fullMessage << "[" + levelName + "] "
		<< (double)SDL_GetTicks() / 1000 << "s " << message << "\n";

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
			std::cout << "Could not open a log file" << std::endl;
		}
		if (file.good())
		{
			file << fullMessage.str();
		}
		else
		{
			std::cout << "Could not acces a log file" << std::endl;
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


