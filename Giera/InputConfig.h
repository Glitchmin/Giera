#pragma once
#include "PlayerActionTypes.h"
using std::string;
using std::runtime_error;

class InputConfig
{
	class AlreadyTakenException : public runtime_error {
	public:
		AlreadyTakenException(const string& message) :runtime_error(message) {};
	};
public:
	PlayerActionTypes getActionType(SDL_Keycode keycode);
	void setActionType(SDL_Keycode	keycode, PlayerActionTypes action) throw (AlreadyTakenException);
private:
	std::map <SDL_Keycode, PlayerActionTypes> inputActionMap;
	std::map <PlayerActionTypes, SDL_Keycode> actionInputMap;

};

