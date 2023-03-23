#pragma once
#include <string>
#include <memory>
#include "ValuesRange.h"
#include "ItemTypes.h"
#include "Calculator.h"
#include "AbstractItem.h"
using std::string;
using std::shared_ptr;
using std::make_shared;

typedef unsigned int item_size_t;
class AbstractBaseItem
{
protected:
	item_size_t width;
	item_size_t height;
	ValuesRange value;
	string name;
	string description;

public:
    item_size_t getWidth() const;
    item_size_t getHeight() const;
    ValuesRange getBaseValue() const;
    string getName() const;
    string getDescription() const;
	virtual ItemTypes getItemType() = 0;
	virtual shared_ptr<AbstractItem> generate()=0;
	friend istream& operator>>(istream& is, AbstractBaseItem& it);
};

