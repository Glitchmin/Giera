#pragma once
#include <string>
#include <sstream>
#include "Logger.h"
#include "FileModesTypes.h"

using std::string;
using std::ios_base;

class FileHandler
{
private:
	SDL_RWops* file = nullptr;
	static inline string folderPath = string(SAVE_FILES_PATH)+"/";
	static inline string saveFolderPath; // empty = Platform::writableDir()
	string fileExtension;
	FileLocation location = FileLocation::GAME_DATA;
	std::stringstream fileContent;
	FileModeTypes currentMode = FileModeTypes::READ;

public:
	FileHandler(string fileName, FileModeTypes fileMode, string fileExtension=".txt",
		FileLocation location = FileLocation::GAME_DATA);
	FileHandler();
	~FileHandler();
	FileHandler(const FileHandler&) = delete;
	FileHandler& operator=(const FileHandler&) = delete;

	void openFile(string fileName, FileModeTypes fileMode);
	template <class T> void saveToFile(T& obj);
	template <class T> void readFromFile(T& obj);
	/// Flushes buffered writes (WRITE/APPEND modes) and closes the file.
	void closeFile();
	bool isOpen() const;

	/// Base folder for game data (default "assets/").
	static void setFolderPath(const string& path);
	static const string& getFolderPath();
	/// Base folder for saves (default Platform::writableDir()). Tests redirect it.
	static void setSaveFolderPath(const string& path);
	static string getSaveFolderPath();
	void setLocation(FileLocation location);
};

template<class T> void FileHandler::readFromFile(T& obj)
{
	fileContent >> obj;
}
template<class T> void FileHandler::saveToFile(T& obj)
{
	fileContent << obj << " ";
}
