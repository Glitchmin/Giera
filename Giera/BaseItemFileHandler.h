#pragma once
#include "FileHandler.h"
#include "BaseItems.h"
#include <memory.h>
#include <vector>
using std::shared_ptr;
using std::unique_ptr;
using std::vector;

class BaseItemFileHandler
{
public:
	void readBaseItems(vector<vector<shared_ptr<AbstractBaseItem>>>& baseItems);
private:
	unique_ptr<FileHandler> fileHandler;
	static inline vector <string> baseItemFilenames{"Melee","Ranged","Shield", "Armor",
	"BaseFood","BaseArrow","BaseMisc","BaseReadable"};
	static inline string filePath = "baseItems/";
	static inline unsigned int version = 1;
	shared_ptr<AbstractBaseItem> readBaseItem();
	int readVersion();
	void saveVersion();

};

