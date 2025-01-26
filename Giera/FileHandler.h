#pragma once
#include <string>
#include <fstream>
#include "Logger.h"
#include "FileModesTypes.h"

using std::string;
using std::fstream;
using std::ios_base;

class FileHandler
{
private:
    SDL_RWops* file;
	static inline string folderPath = string(SAVE_FILES_PATH)+"/";
	string fileExtension;
    std::stringstream fileContent;

public:
	FileHandler(string fileName, FileModeTypes fileMode, string fileExtension=".txt");
	FileHandler();
	~FileHandler();
	void openFile(string fileName, FileModeTypes fileMode);
	template <class T> void saveToFile(T& obj);
	template <class T> void readFromFile(T& obj);
	void closeFile();

};

template<class T> void FileHandler::readFromFile(T& obj)
{
    fileContent >> obj;
}
template<class T> void FileHandler::saveToFile(T& obj)
{
    fileContent << obj<<" ";
}
