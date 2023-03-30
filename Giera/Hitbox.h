#pragma once
#include "AbstractGeometryFigure.h"
#include "HittableEntityTypes.h"
#include <memory>
using std::unique_ptr;

class Hitbox
{
protected:
	unique_ptr <AbstractGeometryFigure> figure;
	HittableEntityTypes hittableEntityType;
public:
	Hitbox(unique_ptr<AbstractGeometryFigure> figure, HittableEntityTypes hittableEntityType);

    unique_ptr<AbstractGeometryFigure> & getFigure();
    void setFigure(unique_ptr<AbstractGeometryFigure> figure);

};

