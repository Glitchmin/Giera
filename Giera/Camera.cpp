#include "Camera.h"
#include "MapTile.h"

Position getAvgPos(shared_ptr<DrawableBoardEntity> entity) {
	Position pos(0, 0, 0);
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
	pair<double, double> mapSize, shared_ptr<Window> window)
{
	this->viewRangeM = viewRangeM;
	this->primaryTarget = primaryTarget;
	this->mapSize = mapSize;
	this->followMouse = false;
	this->window = window;
	updatePosition(Time(0));
	leftUpperPosition = leftUpperTargetPosition;
}

void Camera::updatePosition(Time timeDiff)
{
	auto secondary_sp = secondaryTarget.lock();
	if (secondary_sp == nullptr && !followMouse) {
		leftUpperTargetPosition = getAvgPos(primaryTarget.lock());
	}
	else {
		leftUpperTargetPosition = getAvgPos(primaryTarget.lock()) + (followMouse ?
			 getBoardCursorPosition() : getAvgPos(secondary_sp));

		leftUpperTargetPosition.setX(leftUpperTargetPosition.getX() / 2);
		leftUpperTargetPosition.setY(leftUpperTargetPosition.getY() / 2);
		
	}
	leftUpperTargetPosition = leftUpperTargetPosition
		- Position(viewRangeM.first / 2, viewRangeM.second / 2, 0);
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
	Position delta = leftUpperTargetPosition - leftUpperPosition;
	leftUpperPosition += delta * timeDiff.getTimeMs() * cameraFollowSpeed;

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
	followMouse = false;
}

void Camera::setMouseAsSecondaryTarget()
{
	resetSecondaryTarget();
	followMouse = true;
}

void Camera::resetSecondaryTarget()
{
	secondaryTarget.reset();
	followMouse = false;
}

Position Camera::getLeftUpperPosition() const
{
	return leftUpperPosition;
}

Position Camera::getBoardCursorPosition()
{
	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);
	double xCursorRatio = (double)mouseX / window->getSize().first;
	double yCursorRatio = (double)mouseY / window->getSize().second;
	return leftUpperPosition + Position(xCursorRatio * viewRangeM.first, 
		yCursorRatio * viewRangeM.second, 0);
}

