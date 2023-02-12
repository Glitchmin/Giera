#pragma once
#include "Position.h"
#include "DrawableBoardEntity.h"
class Camera
{
protected:
	Position leftUpperPosition;
	pair<double, double> viewRangeM;
	weak_ptr<DrawableBoardEntity> primaryTarget;
	weak_ptr<DrawableBoardEntity> secondaryTarget;

public:
    Camera();
    Camera(pair<double, double> viewRangeM, weak_ptr<DrawableBoardEntity> primaryTarget);
    void updatePosition();

    pair<double,double> getViewRangeM() const;
    Position getLeftUpperPosition() const;
    void setViewRangeM(double x, double y);

    void setPrimaryTarget(weak_ptr<DrawableBoardEntity> primaryTarget);
    void setSecondaryTarget(weak_ptr<DrawableBoardEntity> secondaryTarget);
    void resetSecondaryTarget();
    

};

