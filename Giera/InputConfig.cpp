#include "pch.h"

#include "InputConfig.h"

PlayerActionTypes InputConfig::getActionType(SDL_Keycode keycode)
{
    auto action = inputActionMap.find(keycode);
    if (action == inputActionMap.end()) {
        return PlayerActionTypes::NONE;
    }

    return action->second;
}

void InputConfig::setActionType(SDL_Keycode keycode, PlayerActionTypes action)
throw (AlreadyTakenException)
{
    if (inputActionMap.find(keycode) == inputActionMap.end()) {
        inputActionMap.erase(actionInputMap[action]);
        inputActionMap[keycode] = action;
        actionInputMap[action] = keycode;
    }
    else {
        throw AlreadyTakenException("this keycode is taken by other action");
    }
}
