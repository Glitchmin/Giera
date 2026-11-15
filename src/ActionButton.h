#pragma once
#include "ButtonUI.h"
#include <functional>

/**
 * @brief A touch / click button that runs an action once per press
 * (used for dash and eating on touch screens).
 */
class ActionButton : public ButtonUI
{
public:
	ActionButton(ButtonUI button, std::function<void()> action);

	bool handleMouseInput(MouseEventTypes mouseEventType, pair<int, int> pos, Time timeDiff) override;
	EventHandleResult handleEvent(const SDL_Event& e, Time timeDiff, int screenSizeX, int screenSizeY) override;

protected:
	std::function<void()> action;
	bool isPressed = false;
};
