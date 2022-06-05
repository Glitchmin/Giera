#pragma once
#include <string>
#include "ValuesRange.h"
#include "ItemTypes.h"
using std::string;

typedef unsigned int item_size_t;
class AbstractItem
{
protected:
	item_size_t width;
	item_size_t height;
	int value;
	string name;
	string description;
	inline static string folderPath = "files/textures";

public:
	AbstractItem(item_size_t width, item_size_t heigth, int Value, string& name,
		string& description);
    item_size_t getWidth() const;
    item_size_t getHeight() const;
    int getValue() const;
    string getName() const;
    string getDescription() const;

	virtual string getPath() = 0;
	virtual ItemTypes getItemType() = 0;

};

