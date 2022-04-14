#pragma once
#include <string>
#include <sstream>
class AbstractMapElement
{
protected:
	static std::string txFolderPath;
	unsigned int elementID;
public:
	virtual std::string getFileName()=0;
};

