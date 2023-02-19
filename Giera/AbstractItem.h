#pragma once
#include <string>
#include <sstream>
#include <optional>
#include "ValuesRange.h"
#include "ItemTypes.h"
#include "DrawableBoardEntity.h"
using std::string;
using std::stringstream;
using std::optional;

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
	static inline string txFolderPath = "../../save_files/tx/items/";
	optional <Position> boardPosition;

public:
	AbstractItem(item_size_t width, item_size_t height, int value,
		string& name, string& description);
	void updateDrawables() override;

    item_size_t getWidth() const;
    item_size_t getHeight() const;
    int getValue() const;
    string getName() const;
    string getDescription() const;
    optional<Position> getBoardPosition() const;
    void setBoardPosition(optional<Position> boardPosition);

	virtual std::string getFilePath() = 0;
	virtual ItemTypes getItemType() = 0;


};

