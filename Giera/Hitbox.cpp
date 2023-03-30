#include "Hitbox.h"

Hitbox::Hitbox(unique_ptr<AbstractGeometryFigure> figure, HittableEntityTypes hittableEntityType)
{
    this->figure = std::move(figure);
    this->hittableEntityType = hittableEntityType;
}

unique_ptr<AbstractGeometryFigure> & Hitbox::getFigure()
{
    return figure;
}

void Hitbox::setFigure(unique_ptr<AbstractGeometryFigure> figure)
{
    this->figure = std::move(figure);
}

