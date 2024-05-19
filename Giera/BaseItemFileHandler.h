#pragma once
#include "FileHandler.h"
#include "BaseItems.h"
#include "BaseItemHandler.h"
#include "AbstractEntityFileLoader.h"
using std::shared_ptr;
using std::unique_ptr;
using std::vector;

class BaseItemFileHandler: public AbstractEntityFileLoader<AbstractBaseItem>
{
private:
	static inline vector <string> baseItemFilenames{"Melee","Ranged","Shield", "Armor",
	"Food","Arrow","Misc","Readable"};
	virtual const vector <string>& getFilenames() override;
	virtual const string& getFilePath() override;
	virtual int getVersion() override;
	virtual shared_ptr<AbstractBaseItem> readEntity() override;
	static inline string filePath = "baseItems/";
	static inline unsigned int version = 1;
};

