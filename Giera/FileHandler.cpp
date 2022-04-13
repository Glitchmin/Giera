#include "FileHandler.h"
#include <sstream>
std::string FileHandler::folderPath = "save_files/";
std::string FileHandler::fileExtension = ".txt";
void FileHandler::openFile(std::string fileName, FileModeTypes fileMode)
{
	std::string mode = "b";
	switch (fileMode) {
	case FileModeTypes::READ_ONLY:
		mode = "r";
		break;
	case FileModeTypes::WRITE_ONLY:
		mode = "w";
		break;
	case FileModeTypes::APPEND:
		mode = "a";
		break;
	case FileModeTypes::READ_WRITE:
		mode = "r+";
		break;
	}
	rw = SDL_RWFromFile((folderPath + fileName+fileExtension).c_str(),mode.c_str());
	if (rw == NULL) 
	{
		Logger::logError("could open file: " + folderPath + fileName+ fileExtension +" in mode "+mode);
		return;
	}
	Logger::logInfo("opened" + folderPath + fileName + fileExtension + " in mode " + mode);
}

void FileHandler::saveToFile(void* a, size_t size)
{
	if (rw != NULL)
	{
		std::stringstream ss;
		ss << size;
		Logger::logInfo("saving something with " + ss.str() + " size");
		SDL_RWwrite(rw, a, size, 1);
	}
}

void FileHandler::readFromFile(void* a, size_t size)
{
	if (rw != NULL)
	{
		std::stringstream ss;
		ss << size;
		Logger::logInfo("reading something with " + ss.str()+" size");
		SDL_RWread(rw, a, size, 1);
	}
}

void FileHandler::rewind()
{
	SDL_RWseek(rw, 0, RW_SEEK_SET);
}

void FileHandler::closeFile()
{
	if (rw != NULL)
	{
		SDL_RWclose(rw);
	}
}
