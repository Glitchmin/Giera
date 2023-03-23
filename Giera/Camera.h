#pragma once
#include "Position.h"
#include "DrawableBoardEntity.h"
#include "Window.h"
class Camera
{
protected:
	Position leftUpperPosition;
	Position leftUpperTargetPosition;
	pair<double, double> viewRangeM;
	weak_ptr<DrawableBoardEntity> primaryTarget;
	weak_ptr<DrawableBoardEntity> secondaryTarget;
    double cameraFollowSpeed = 0.0045;
    bool followMouse;
    shared_ptr <Window> window;
public:
    Camera();
    Camera(pair<double, double> viewRangeM, weak_ptr<DrawableBoardEntity> primaryTarget,
        pair<double, double> mapSize, shared_ptr<Window> window);
    void updatePosition(Time timeDiff);
    pair<double, double> mapSize;
    pair<double,double> getViewRangeM() const;
    Position getLeftUpperPosition() const;
    Position getBoardCursorPosition();
    void setViewRangeM(double x, double y);

    void setPrimaryTarget(weak_ptr<DrawableBoardEntity> primaryTarget);
    void setSecondaryTarget(weak_ptr<DrawableBoardEntity> secondaryTarget);
    void setMouseAsSecondaryTarget();
    void resetSecondaryTarget();
    

};

