#pragma once
#include <string>
#include <sstream>
#include "ValuesRange.h"
#include "ItemTypes.h"
using std::string;
using std::stringstream;

typedef unsigned int item_size_t;
class AbstractItem
{
protected:
	item_size_t width;
	item_size_t height;
	int value;
	string name;
	string description;

public:
	AbstractItem(item_size_t width, item_size_t height, int Value, string& name,
		string& description);
    item_size_t getWidth() const;
    item_size_t getHeight() const;
    int getValue() const;
    string getName() const;
    string getDescription() const;

	virtual ItemTypes getItemType() = 0;

};

