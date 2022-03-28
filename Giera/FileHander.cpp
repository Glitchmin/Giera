#include "FileHander.h"
#include <sstream>
std::string FileHander::folderPath = "saves/save1/";
void FileHander::openFile(std::string fileName, FileModeTypes fileMode)
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
	rw = SDL_RWFromFile((folderPath + fileName).c_str(),mode.c_str());
	if (rw == NULL) {
		Logger::logError("could open file: " + folderPath + fileName+" in mode "+mode);
		return;
	}
	Logger::logInfo("opened" + folderPath + fileName + " in mode " + mode);
}

void FileHander::saveToFile(void* a, size_t size)
{
	SDL_RWwrite(rw, a, size, 1);
}
