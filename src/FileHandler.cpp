#include "FileHandler.h"
#include "Platform.h"
#include <filesystem>
#include <sstream>
#include <string>

FileHandler::FileHandler(string fileName, FileModeTypes fileMode, string fileExtension, FileLocation location)
{
	this->fileExtension = fileExtension;
	this->location = location;
	openFile(fileName, fileMode);
}

FileHandler::FileHandler()
{
}

FileHandler::~FileHandler()
{
	closeFile();
}

void FileHandler::setFolderPath(const string& path)
{
	folderPath = path;
	if (!folderPath.empty() && folderPath.back() != '/' && folderPath.back() != '\\') {
		folderPath += '/';
	}
}

const string& FileHandler::getFolderPath()
{
	return folderPath;
}

void FileHandler::setSaveFolderPath(const string& path)
{
	saveFolderPath = path;
	if (!saveFolderPath.empty() && saveFolderPath.back() != '/' && saveFolderPath.back() != '\\') {
		saveFolderPath += '/';
	}
}

string FileHandler::getSaveFolderPath()
{
	return saveFolderPath.empty() ? Platform::writableDir() : saveFolderPath;
}

void FileHandler::setLocation(FileLocation location)
{
	this->location = location;
}

bool FileHandler::isOpen() const
{
	return file != nullptr;
}

void FileHandler::openFile(string fileName, FileModeTypes fileMode)
{
	closeFile(); // re-opening used to leak the previous SDL_RWops

	// Binary modes: no CRLF translation, identical behaviour on Windows and Android.
	string mode;
	switch (fileMode) {
	case FileModeTypes::READ:
		mode = "rb";
		break;
	case FileModeTypes::WRITE:
		mode = "wb";
		break;
	case FileModeTypes::APPEND:
		mode = "ab";
		break;
	case FileModeTypes::READ_WRITE:
		mode = "r+b";
		break;
	}
	currentMode = fileMode;
	fileContent = std::stringstream();

	const string base = location == FileLocation::SAVES ? getSaveFolderPath() : folderPath;
	string fullPath = base + fileName + fileExtension;
	if (location == FileLocation::SAVES && fileMode != FileModeTypes::READ) {
		// make sure e.g. "maps/" exists inside the save folder
		std::error_code ec;
		std::filesystem::create_directories(std::filesystem::u8path(fullPath).parent_path(), ec);
	}
	file = SDL_RWFromFile(fullPath.c_str(), mode.c_str());
	if (!file)
	{
		Logger::logError("File", fullPath, " could not be opened in mode", mode, ". SDL_Error: ", SDL_GetError());
		return;
	}
	Logger::logDebug("Opened", fullPath, " in mode", mode);

	if (fileMode == FileModeTypes::READ || fileMode == FileModeTypes::READ_WRITE) {
		Sint64 length = SDL_RWsize(file);
		if (length > 0) {
			// The old code streamed a raw, non-null-terminated char buffer
			// (read past its end) and leaked it on every file read.
			string buffer(static_cast<size_t>(length), '\0');
			size_t read = SDL_RWread(file, buffer.data(), 1, static_cast<size_t>(length));
			buffer.resize(read);
			fileContent.str(buffer);
		}
	}
}

void FileHandler::closeFile()
{
	if (!file) {
		return;
	}
	// saveToFile() only buffers into fileContent; flush it now.
	// (Previously nothing written was ever persisted.)
	if (currentMode == FileModeTypes::WRITE || currentMode == FileModeTypes::APPEND) {
		string data = fileContent.str();
		if (!data.empty()) {
			SDL_RWwrite(file, data.data(), 1, data.size());
		}
	}
	SDL_RWclose(file);
	file = nullptr;
}
