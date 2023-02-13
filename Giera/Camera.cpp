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

Camera::Camera(pair<double, double> viewRangeM, weak_ptr<DrawableBoardEntity> primaryTarget,
	pair<double, double> mapSize)
{
	this->viewRangeM = viewRangeM;
	this->primaryTarget = primaryTarget;
	this->mapSize = mapSize;
	updatePosition(Time(0));
	leftUpperPosition = leftUpperTargetPosition;
}

void Camera::updatePosition(Time timeDiff)
{
	auto secondary_sp = secondaryTarget.lock();
	if (secondary_sp == nullptr) {
		leftUpperTargetPosition = getAvgPos(primaryTarget.lock())
			- Position(viewRangeM.first / 2, viewRangeM.second / 2, 0);
	}
	else {
		leftUpperTargetPosition = getAvgPos(primaryTarget.lock())
			+ getAvgPos(secondary_sp);
		leftUpperTargetPosition.setX(leftUpperTargetPosition.getX() / 2);
		leftUpperTargetPosition.setY(leftUpperTargetPosition.getY() / 2);
		leftUpperTargetPosition = leftUpperTargetPosition
			- Position(viewRangeM.first / 2, viewRangeM.second / 2, 0);
	}
	if (leftUpperTargetPosition.getX() < 0) {
		leftUpperTargetPosition.setX(0);
	}	
	if (leftUpperTargetPosition.getY() < 0) {
		leftUpperTargetPosition.setY(0);
	}	
	if (leftUpperTargetPosition.getX() + viewRangeM.first > mapSize.first) {
		leftUpperTargetPosition.setX(mapSize.first - viewRangeM.first);
	}
	if (leftUpperTargetPosition.getY() + viewRangeM.second > mapSize.second) {
		leftUpperTargetPosition.setY(mapSize.second - viewRangeM.second);
	}
	if (secondary_sp == nullptr) {
		leftUpperPosition = leftUpperTargetPosition;
		return;
	}
	Position delta = leftUpperTargetPosition - leftUpperPosition;
	Logger::logInfo(timeDiff.getTimeMs());
	leftUpperPosition += delta * timeDiff.getTimeMs() * 0.003;

}

pair<double, double> Camera::getViewRangeM() const
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

