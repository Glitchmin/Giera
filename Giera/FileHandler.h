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
	fstream fs;
	//static inline string folderPath = "";
	static inline string folderPath = "save_files/";
	static inline string fileExtension = ".txt";

public:
	FileHandler(string fileName, FileModeTypes fileMode);
	FileHandler();
	~FileHandler();
	void openFile(string fileName, FileModeTypes fileMode);
	template <class T> void saveToFile(T& obj);
	template <class T> void readFromFile(T& obj);
	template <class T> T createObjectFromFile();
	void closeFile();

};

template<class T> void FileHandler::readFromFile(T& obj)
{
	fs >> obj;
}
template<class T> void FileHandler::saveToFile(T& obj)
{
	fs << obj<<" ";
}
template<class T> T FileHandler::createObjectFromFile()
{
	return T(fs);
}
