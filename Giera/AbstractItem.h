#pragma once
#include <string>
#include <sstream>
#include "ValuesRange.h"
#include "ItemTypes.h"
#include "DrawableBoardEntity.h"
using std::string;
using std::stringstream;

typedef unsigned int item_size_t;
class AbstractItem : public DrawableBoardEntity
{
protected:
	item_size_t width;
	item_size_t height;
	int value;
	string name;
	string description;
	shared_ptr <Sprite> sprite;
	static inline string txFolderPath = "../../save_files/tx/";

public:
	AbstractItem(item_size_t width, item_size_t height, int value,
		string& name, string& description);
	void updateDrawables() override;

    item_size_t getWidth() const;
    item_size_t getHeight() const;
    int getValue() const;
    string getName() const;
    string getDescription() const;

	virtual std::string getFilePath() = 0;
	virtual ItemTypes getItemType() = 0;

};

