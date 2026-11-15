#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <memory>
#include <optional>
#include "BoardLoop.h"
#include "CombatRules.h"
#include "Platform.h"
#include "TextureLoader.h"

namespace {

struct Options {
	bool touchUi = Platform::isAndroid;
	bool autoplay = false;
	bool verbose = false;
	std::optional<Time> runFor;
};

Options parseArgs(int argc, char* argv[])
{
	Options o;
	for (int i = 1; i < argc; i++) {
		if (!std::strcmp(argv[i], "--touch-ui")) {
			o.touchUi = true;          // show the Android on-screen controls on desktop
		}
		else if (!std::strcmp(argv[i], "--autoplay")) {
			o.autoplay = true;         // the player auto-attacks the nearest enemy
		}
		else if (!std::strcmp(argv[i], "--verbose")) {
			o.verbose = true;
		}
		else if (!std::strcmp(argv[i], "--smoke-test") && i + 1 < argc) {
			// Run the real game loop for N seconds of game time, then exit (used by CTest).
			o.runFor = Time(static_cast<unsigned int>(std::atof(argv[++i]) * 1000));
		}
	}
	return o;
}

}

int main(int argc, char* argv[])
{
	Platform::initWorkingDirectory();
	Options options = parseArgs(argc, argv);
	Platform::setTouchControls(options.touchUi);
	Logger::setLevel(options.verbose ? LoggingLevels::DEBUG : LoggingLevels::INFO);

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
		Logger::logError("SDL could not initialize", SDL_GetError());
		return 1;
	}
	if (TTF_Init() == -1) {
		Logger::logError("TTF could not initialize", TTF_GetError());
	}
	if ((IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) & IMG_INIT_PNG) == 0) {
		Logger::logWarning("IMG_Init:", IMG_GetError());
	}

	int exitCode = 0;
	{
		auto window = std::make_shared<Window>();
		if (!window->getRenderer()) {
			Logger::logError("No renderer - cannot start the game");
			exitCode = 1;
		}
		else {
			auto inputConfig = std::make_shared<InputConfig>();
			inputConfig->setActionType(SDL_SCANCODE_A, PlayerActionTypes::MOVE_LEFT);
			inputConfig->setActionType(SDL_SCANCODE_D, PlayerActionTypes::MOVE_RIGHT);
			inputConfig->setActionType(SDL_SCANCODE_W, PlayerActionTypes::MOVE_UP);
			inputConfig->setActionType(SDL_SCANCODE_S, PlayerActionTypes::MOVE_DOWN);
			inputConfig->setActionType(SDL_SCANCODE_C, PlayerActionTypes::ADD_SECONDARY_CAMERA_TARGET);
			inputConfig->setActionType(SDL_SCANCODE_I, PlayerActionTypes::OPEN_EQUIPMENT);
			inputConfig->setActionType(SDL_SCANCODE_ESCAPE, PlayerActionTypes::CLOSE_WINDOW);
			inputConfig->setActionType(SDL_SCANCODE_LCTRL, PlayerActionTypes::PARRY);
			inputConfig->setActionType(SDL_SCANCODE_Q, PlayerActionTypes::CHANGE_WEAPON);
			inputConfig->setActionType(SDL_SCANCODE_SPACE, PlayerActionTypes::DASH);
			inputConfig->setActionType(SDL_SCANCODE_1, PlayerActionTypes::USE_SATCHEL_1);
			inputConfig->setActionType(SDL_SCANCODE_2, PlayerActionTypes::USE_SATCHEL_2);
			inputConfig->setActionType(SDL_SCANCODE_3, PlayerActionTypes::USE_SATCHEL_3);
			inputConfig->setActionType(SDL_SCANCODE_4, PlayerActionTypes::USE_SATCHEL_4);

			CombatRules::load(); // assets/rules/Combat.txt
			auto boardLoop = std::make_unique<BoardLoop>(window, inputConfig);
			boardLoop->setAutoplay(options.autoplay);
			Logger::logInfo("window", window->getSize().first, "x", window->getSize().second);
			BoardLoop::Stats stats = boardLoop->start(options.runFor);

			std::cout << "Game loop finished: " << stats.updates << " updates, "
				<< stats.framesRendered << " frames, game time " << stats.gameTime
				<< ", enemies spawned " << stats.enemiesSpawned
				<< ", killed " << stats.enemiesKilled
				<< ", player deaths " << stats.playerDeaths
				<< ", respawns " << stats.playerRespawns << std::endl;
			if (options.runFor.has_value() && (stats.framesRendered == 0 || stats.updates == 0)) {
				exitCode = 2; // smoke test: the loop did not actually run
			}
		}
		TextureLoader::clearCache(); // before the renderer is destroyed with the window
	}

	Logger::logInfo("end of the program");
	Logger::close();
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
	return exitCode;
}
