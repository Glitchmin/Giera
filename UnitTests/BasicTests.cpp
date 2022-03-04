#include "pch.h"
#include "CppUnitTest.h"
#include "../Giera/AbstractTimer.h"
#include "../Giera/GeneralTimer.h"
#include <iostream>
#include <SDL.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UtilityTests
{
	TEST_CLASS(TimerTests)
	{
	public:

		TEST_METHOD(GeneralTimerTests)
		{
			Uint32 time = SDL_GetTicks();

		}
	};
}
