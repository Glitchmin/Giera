#pragma once
#include "FileHandler.h"
#include <vector>
using std::vector;
using std::shared_ptr;
using std::make_unique;
using std::unique_ptr;

template <class Entity> 
class AbstractEntityFileLoader
{
public:
	virtual void readEntities(vector <vector <shared_ptr<Entity>>>& entities);
protected:
	unique_ptr<FileHandler> fileHandler;
	virtual const vector <string>& getFilenames() = 0;
	virtual const string& getFilePath() = 0;
	virtual int getVersion() = 0;
	virtual shared_ptr<Entity> readEntity() = 0;
	int readVersion();
	void saveVersion();
};

template<class Entity> int AbstractEntityFileLoader<Entity>::readVersion()
{
	int version = 0;
	fileHandler->readFromFile(version);
	return version;
}
template<class Entity> void AbstractEntityFileLoader<Entity>::saveVersion()
{
	int version = getVersion();
	fileHandler->saveToFile(version);
}
template<class Entity> void AbstractEntityFileLoader<Entity>::readEntities(vector<vector<shared_ptr<Entity>>>& entities)
{

	if (entities.size() == getFilenames().size()) {
		return;
	}
	entities.resize(getFilenames().size());
	for (int i = 0; i < getFilenames().size();i++) {
		const string& filename = getFilenames()[i];
		std::stringstream ss;
		ss << getFilePath() << filename;
		fileHandler = make_unique<FileHandler>(ss.str(), FileModeTypes::READ, ".csv");
		string fillers = "";
		fileHandler->readFromFile(fillers);
		int version = readVersion();
		ss << " opening with " << version << " version for reading";
		Logger::logInfo(ss.str());
		fileHandler->readFromFile(fillers);
		int itemsNumber;
		fileHandler->readFromFile(itemsNumber);
		fileHandler->readFromFile(fillers);
		int blankFieldsNumber;
		fileHandler->readFromFile(blankFieldsNumber);
		for (int j = 0; j < 2 * blankFieldsNumber + 6;j++) {
			fileHandler->readFromFile(fillers);
		}
		for (int j = 0; j < itemsNumber;j++)
		{
			entities[i].push_back(readEntity());
		}
		Logger::logInfo(filename, " loaded");

		fileHandler->closeFile();
	}
	Logger::logInfo("all data");
}