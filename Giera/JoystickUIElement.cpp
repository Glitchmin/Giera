#include "JoystickUIElement.h"

bool JoystickUIElement::handleMouseInput(MouseEventTypes mouseEventType, std::pair<int, int> pos, Time timeDiff)
{
    auto fieldSize = texture->getSize();
	bool ans = false;
	for (auto& child : children) {
		ans = ans || child->handleMouseInput(mouseEventType, pos, timeDiff);
	}
	if (pxRealPosRect.isPointInside(pos.first, pos.second)) {
		xDeflection = pos.first - (pxRealPosRect.x + pxRealPosRect.w / 2);
		yDeflection = pos.second - (pxRealPosRect.y + pxRealPosRect.h / 2);
		float deflectionsLength = sqrt(xDeflection * xDeflection + yDeflection * yDeflection);
		xDeflection /= deflectionsLength;
		yDeflection /= deflectionsLength;
		pressed = true;
		updateNeeded = true;
		player->move(Position(xDeflection * timeDiff.getTimeS()*4, yDeflection * timeDiff.getTimeS()*4, 0));
		return true;
	}
	if (pressed) {
		updateNeeded = true;
	}
	pressed = false;
	return false;
}

void JoystickUIElement::drawInside()
{
	auto fieldSize = texture->getSize();
	image->draw(*texture, nullopt, SDL_Rect{
		(int)(fieldSize.first*.4),(int)(fieldSize.second * .4),
		(int)(fieldSize.first * .2),(int)(fieldSize.second * .2) });
	knobImage->draw(*texture, nullopt, SDL_Rect{
		(int)(fieldSize.first * (.45 + pressed * xDeflection*.1)),
		(int)(fieldSize.second * (.45 + pressed * yDeflection*.1)),
		(int)(fieldSize.first * .1),(int)(fieldSize.second * .1) });
}

UIElement::EventHandleResult
JoystickUIElement::handleEvent(const SDL_Event &e, Time timeDiff, int screenSizeX,
                               int screenSizeY) {
    bool atLeastOneFingerInside = false;
    for (auto f:fingerPositions){
        if (pxRealPosRect.isPointInside(f.first,f.second)){
            atLeastOneFingerInside = true;
            handleMouseInput(UIElement::MouseEventTypes::PRESS_LEFT,f,timeDiff);
        }
    }
    if (!atLeastOneFingerInside){
        pressed = false;
        updateNeeded = true;
    }
    EventHandleResult ret{ false, {} };
    return ret;
}
