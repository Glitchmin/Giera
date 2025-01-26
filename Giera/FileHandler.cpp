#include "FileHandler.h"
#include <sstream>
#include <fstream>
FileHandler::FileHandler(string fileName, FileModeTypes fileMode, string fileExtension)
{
	this->fileExtension = fileExtension;
	openFile(fileName, fileMode);
}
FileHandler::FileHandler()
{
}
FileHandler::~FileHandler()
{
	closeFile();
}
void FileHandler::openFile(string fileName, FileModeTypes fileMode)
{
	string mode;
	switch (fileMode) {
	case FileModeTypes::READ:
		mode = "r";
		break;
	case FileModeTypes::WRITE:
		mode = "w";
		break;
	case FileModeTypes::APPEND:
		mode = "a";
		break;
	case FileModeTypes::READ_WRITE:
		mode = "r+";
		break;
	}
    string fullPath = folderPath + fileName + fileExtension;
    file = SDL_RWFromFile(fullPath.c_str(),mode.c_str());
	if (!file)
	{
        Logger::logError("File",folderPath + fileName+ fileExtension,
                         " could not be opened in mode",mode,". SDL_Error: ", SDL_GetError());
		return;
	}
    Logger::logInfo("Opened",folderPath + fileName+ fileExtension," in mode",mode);
    Logger::logInfo("File size is",SDL_RWsize(file));
    Sint64 length = SDL_RWsize(file);
    char* buffer = new char[length];
    SDL_RWread(file,buffer,1,length);
    std::stringstream ss;
    ss << buffer;
    fileContent = std::move(ss);
}

void FileHandler::closeFile()
{
    //SDL_RWclose(file);
}
