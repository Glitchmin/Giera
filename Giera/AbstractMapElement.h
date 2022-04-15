#pragma once
#include <string>
#include <sstream>
#include "Time.h"
class AbstractMapElement
{
protected:
	static std::string txFolderPath;
	unsigned int elementID;
	unsigned int framesNumber;
	Time refreshTime;
public:
	virtual std::string getFileName()=0;
};

