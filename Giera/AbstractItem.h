#pragma once
#include <string>
#include <sstream>
#include <optional>
#include <memory>
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
	shared_ptr <Texture> texture;
	static inline string txFolderPath = "../../save_files/tx/items/";
	optional <pair<Position, pair<double, double> > > boardRect;
	shared_ptr <Drawable> drawable;

public:
	AbstractItem(item_size_t width, item_size_t height, int value,
		string& name, string& description);
	void updateDrawables() override;

    item_size_t getWidth() const;
    item_size_t getHeight() const;
    int getValue() const;
    string getName() const;
    string getDescription() const;
	optional <pair<Position, pair<double, double> > > getBoardRect() const;
    void setBoardRect(optional <pair<Position, pair<double, double> > >);

	virtual std::string getFilePath() = 0;
	virtual ItemTypes getItemType() = 0;


};

