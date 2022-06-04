#pragma once
#include <string>
#include "ValuesRange.h"
#include "ItemTypes.h"
using std::string;

typedef unsigned int item_size_t;
class AbstractBaseItem
{
protected:
	static ItemTypes itemType;
	item_size_t width;
	item_size_t height;
	ValuesRange baseValue;
	string name;
	string description;
	inline static string folderPath = "files/textures";

public:
    item_size_t getWidth() const;
    item_size_t getHeight() const;
    ValuesRange getBaseValue() const;
    string getName() const;
    string getDescription() const;
	virtual string getPath() = 0;
	virtual ItemTypes getItemType() = 0;
};

