#pragma once
#include <set>
#include <memory>

class AbstractItem;
class AbstractNPC;


using std::shared_ptr;
using std::set;

class BoardTile
{
	set < shared_ptr < AbstractItem > > items;
	set < shared_ptr <AbstractNPC> > npcs;
public:
	set<shared_ptr<AbstractItem>>& getItems();
	void addItem(shared_ptr<AbstractItem> item);
	void removeItem(shared_ptr<AbstractItem> item);
	void addNpc(shared_ptr<AbstractNPC> npc);
	void removeNpc(shared_ptr<AbstractNPC> npc);
	set< shared_ptr <AbstractNPC>>& getNpcs();

};

