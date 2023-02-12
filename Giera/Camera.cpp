#include "Camera.h"

Position getAvgPos(shared_ptr<DrawableBoardEntity> entity) {
    Position pos;
    int drawablesCount = entity->getDrawables().size();
    for (auto& ptr : entity->getDrawables()) {
        pos += ptr.getPos();
    }
    pos.setX(pos.getX() / drawablesCount);
    pos.setY(pos.getY() / drawablesCount);

    return pos;
}

Camera::Camera()
{
    leftUpperPosition = Position(0, 0, 0);
}

Camera::Camera(pair<double, double> viewRangeM, 
    weak_ptr<DrawableBoardEntity> primaryTarget)
{
    this->viewRangeM = viewRangeM;
    this->primaryTarget = primaryTarget;
    updatePosition();
}

void Camera::updatePosition()
{
    if (auto secondary_sp = secondaryTarget.lock(); secondary_sp == nullptr) {
        leftUpperPosition = getAvgPos(primaryTarget.lock())
            -Position(viewRangeM.first/2, viewRangeM.second/2,0);
    }
    else {
        leftUpperPosition = getAvgPos(primaryTarget.lock())
            + getAvgPos(secondary_sp);
        leftUpperPosition.setX(leftUpperPosition.getX() / 2);
        leftUpperPosition.setY(leftUpperPosition.getY() / 2);
        leftUpperPosition = leftUpperPosition 
            - Position(viewRangeM.first / 2, viewRangeM.second / 2, 0);
    }
}

pair<double,double> Camera::getViewRangeM() const
{
    return viewRangeM;
}

void Camera::setViewRangeM(double x, double y)
{
    viewRangeM.first = x;
    viewRangeM.second = y;
}


void Camera::setPrimaryTarget(weak_ptr<DrawableBoardEntity> primaryTarget)
{
    this->primaryTarget = primaryTarget;
}

void Camera::setSecondaryTarget(weak_ptr<DrawableBoardEntity> secondaryTarget)
{
    this->secondaryTarget = secondaryTarget;
}

void Camera::resetSecondaryTarget()
{
    secondaryTarget.reset();
}

Position Camera::getLeftUpperPosition() const
{
    return leftUpperPosition;
}

