#include "ActionButton.h"

ActionButton::ActionButton(ButtonUI button, std::function<void()> action)
	: ButtonUI(std::move(button)), action(std::move(action))
{
}

bool ActionButton::handleMouseInput(MouseEventTypes mouseEventType, pair<int, int> pos, Time timeDiff)
{
	if (mouseEventType == MouseEventTypes::PRESS_LEFT && !isPressed && pxRealPosRect.isPointInside(pos.first, pos.second)) {
		isPressed = true;
		if (action) {
			action();
		}
		return true;
	}
	if (mouseEventType == MouseEventTypes::RELEASE_LEFT) {
		isPressed = false;
	}
	return false;
}

UIElement::EventHandleResult ActionButton::handleEvent(const SDL_Event& e, Time timeDiff, int screenSizeX, int screenSizeY)
{
	bool fingerInside = false;
	for (auto f : fingerPositions) {
		if (pxRealPosRect.isPointInside(f.first, f.second)) {
			fingerInside = true;
			handleMouseInput(MouseEventTypes::PRESS_LEFT, f, timeDiff); // once per press
		}
	}
	if (!fingerInside) {
		isPressed = false;
	}
	return EventHandleResult{ false, {} };
}
