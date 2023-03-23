#pragma once
#include "Position.h"
#include "DrawableBoardEntity.h"
class Camera
{
protected:
	Position leftUpperPosition;
	Position leftUpperTargetPosition;
	pair<double, double> viewRangeM;
	weak_ptr<DrawableBoardEntity> primaryTarget;
	weak_ptr<DrawableBoardEntity> secondaryTarget;
    double cameraFollowSpeed = 0.005;

public:
    Camera();
    Camera(pair<double, double> viewRangeM, weak_ptr<DrawableBoardEntity> primaryTarget,
        pair<double, double> mapSize);
    void updatePosition(Time timeDiff);
    pair<double, double> mapSize;
    pair<double,double> getViewRangeM() const;
    Position getLeftUpperPosition() const;
    void setViewRangeM(double x, double y);

    void setPrimaryTarget(weak_ptr<DrawableBoardEntity> primaryTarget);
    void setSecondaryTarget(weak_ptr<DrawableBoardEntity> secondaryTarget);
    void resetSecondaryTarget();
    

};

