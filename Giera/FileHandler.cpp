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
	int mode = 0;
	switch (fileMode) {
	case FileModeTypes::READ:
		mode = ios_base::in;
		break;
	case FileModeTypes::WRITE:
		mode = ios_base::out;
		break;
	case FileModeTypes::APPEND:
		mode = ios_base::app;
		break;
	case FileModeTypes::READ_WRITE:
		mode = ios_base::in | ios_base::out;
		break;
	}
	fs = fstream(folderPath+fileName + fileExtension, mode);
	if (!fs.is_open()) 
	{
		Logger::logError("could not open file: " + folderPath + fileName+ fileExtension +" in mode ",mode);
		return;
	}
	Logger::logInfo("opened " + folderPath + fileName + fileExtension + " in mode ", mode);
}



void FileHandler::closeFile()
{
	fs.close();
}
