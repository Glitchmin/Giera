#pragma once
#include <set>
#include <memory>

class AbstractItem;
class AbstractCharacter;


using std::shared_ptr;
using std::set;

class BoardTile
{
	set < shared_ptr < AbstractItem > > items;
	set < shared_ptr <AbstractCharacter> > characters;
public:
	set<shared_ptr<AbstractItem>>& getItems();
	void addItem(shared_ptr<AbstractItem> item);
	void removeItem(shared_ptr<AbstractItem> item);
	void addcharacter(shared_ptr<AbstractCharacter> character);
	void removecharacter(shared_ptr<AbstractCharacter> character);
	set< shared_ptr <AbstractCharacter>>& getcharacters();

};

