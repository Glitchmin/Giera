#include "InputConfig.h"

PlayerActionTypes InputConfig::getActionType(SDL_Keycode keycode)
{
    auto action = inputMap.find(keycode);
    if (action == inputMap.end()) {
        return PlayerActionTypes::NONE;
    }

    return action->second;
}

void InputConfig::setActionType(SDL_Keycode keycode, PlayerActionTypes action)
throw (AlreadyTakenException)
{
    if (inputMap.find(keycode) == inputMap.end()) {
        inputMap[keycode] = action;
    }
    else {
        throw AlreadyTakenException("this keycode is taken by other action");
    }
}
