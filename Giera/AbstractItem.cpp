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
    if (boardRect) {
        if (drawable == nullptr) {
            drawable = make_shared<Drawable>(boardRect.value().first, texture,
                Drawable::DrawableLayer::ENTITIES, make_pair(boardRect.value().second.first,
                    boardRect.value().second.second), 0);
        }
        drawables.push_back(drawable);
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

optional<pair<Position, pair<double, double>>> AbstractItem::getBoardRect() const
{
    return boardRect;
}

void AbstractItem::setBoardRect(optional<pair<Position, pair<double, double>>> boardRect)
{
    this->boardRect = boardRect;
}




