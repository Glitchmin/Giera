#include "pch.h"
#include "CppUnitTest.h"
#include "../Giera/AbstractTimer.h"
#include "../Giera/GeneralTimer.h"
#include "../Giera/GeneralTimer.cpp"
#include "../Giera/AbstractTimer.cpp"
#include <iostream>
#include <string>
#include <SDL.h>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UtilityTests
{
	TEST_CLASS(TimerTests)
	{
	public:
		TEST_METHOD(GeneralTimerTests)
		{
			GeneralTimer generalTimer;
			Assert::IsTrue(generalTimer.getTime() < 5);
			Uint32 time = SDL_GetTicks();
			while (SDL_GetTicks() < time + 200) {
				generalTimer.updateTime();
				SDL_Delay(2);
			}
			Assert::IsTrue(generalTimer.getTime() < 205);
			Assert::IsTrue(generalTimer.getTime() > 195);
			std::cout << generalTimer.getTime();
			Logger::WriteMessage("some text");
		}
	};
}
