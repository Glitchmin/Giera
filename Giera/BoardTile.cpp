#include "pch.h"

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

set< shared_ptr <AbstractCharacter> >& BoardTile::getcharacters()
{
    return characters;
}

void BoardTile::addCharacter(shared_ptr<AbstractCharacter> character)
{
    characters.insert(character);
}

void BoardTile::removeCharacter(shared_ptr<AbstractCharacter> character)
{
    characters.erase(character);
}

