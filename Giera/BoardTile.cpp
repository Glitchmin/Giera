#include "BoardTile.h"
#include "NormalItems.h"

set<shared_ptr<AbstractItem>>& BoardTile::getItems()
{
    return items;
}

void BoardTile::addItem(shared_ptr<AbstractItem> item)
{
    items.insert(item);
}

void BoardTile::removeItem(shared_ptr<AbstractItem> item)
{
    items.erase(item);
}

set< shared_ptr <AbstractNPC> >& BoardTile::getNpcs()
{
    return npcs;
}

void BoardTile::addNpc(shared_ptr<AbstractNPC> npc)
{
    npcs.insert(npc);
}

void BoardTile::removeNpc(shared_ptr<AbstractNPC> npc)
{
    npcs.erase(npc);
}

