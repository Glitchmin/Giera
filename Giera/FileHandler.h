#pragma once
#include <SDL.h>
#include <string>
#include "Logger.h"
#include "FileModesTypes.h"
class FileHandler
{
private:
	bool isFileOpened;
	SDL_RWops* rw;
	static std::string folderPath;
	static std::string fileExtension;

public:
	void openFile(std::string fileName, FileModeTypes fileMode);
	void saveToFile(void* a, size_t size);
	void readFromFile(void* a, size_t size);
	void rewind();
	void closeFile();
};

