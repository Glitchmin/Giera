#pragma once
#include <SDL.h>
#include <string>
#include "Logger.h"
#include "FileModesTypes.h"
class FileHander
{
private:
	bool isFileOpened;
	SDL_RWops* rw;
	static std::string folderPath;

public:
	void openFile(std::string fileName, FileModeTypes fileMode);
	void saveToFile(void* a, size_t size);
	void readFromFile(void* a, size_t size);
	void closeFile();
};

