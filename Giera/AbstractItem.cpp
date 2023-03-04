#include "AbstractItem.h"
using std::make_pair;
using std::make_shared;

AbstractItem::AbstractItem(item_size_t width, item_size_t height, int value, string& name, string& description)
{
    this->width = width;
    this->height = height;
    this->value = value;
    this->name = name;
    this->description = description;
    this->texture = nullptr;
}

void AbstractItem::updateDrawables()
{
    if (!texture) {
        string path = getFilePath();
        texture = make_shared<Texture>(path);
    }
    drawables.clear();
    if (boardPosition) {
        drawables.push_back(Drawable(boardPosition.value(), texture, Drawable::DrawableLayer::ENTITIES, 0.5, 0.5));
    }
}

item_size_t AbstractItem::getWidth() const
{
    return width;
}

item_size_t AbstractItem::getHeight() const
{
    return height;
}

int AbstractItem::getValue() const
{
    return value;
}

string AbstractItem::getName() const
{
    return name;
}

string AbstractItem::getDescription() const
{
    return description;
}


optional<Position> AbstractItem::getBoardPosition() const
{
    return boardPosition;
}

void AbstractItem::setBoardPosition(optional<Position> boardPosition)
{
    this->boardPosition = boardPosition;
}

