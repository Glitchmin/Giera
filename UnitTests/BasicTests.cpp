#include "pch.h"
#include "CppUnitTest.h"
#include "../Giera/AbstractTimer.h"
#include "../Giera/GeneralTimer.h"
#include "../Giera/SubTimer.h"
#include "../Giera/GeneralTimer.cpp"
#include "../Giera/SubTimer.cpp"
#include "../Giera/AbstractTimer.cpp"
#include <iostream>
#include <string>
#include <SDL.h>
#include <Windows.h>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UtilityTests
{
	TEST_CLASS(TimerTests)
	{
	public:
		TEST_METHOD(GeneralTimerTestTempo1_0)
		{
			GeneralTimer generalTimer;
			Assert::IsTrue(generalTimer.getTime() < 5);
			Uint32 time = SDL_GetTicks();
			while (SDL_GetTicks() < time + 200)
			{
				generalTimer.updateTime();
			}
			Assert::IsTrue(generalTimer.getTime() < 210);
			Assert::IsTrue(generalTimer.getTime() > 190);
		}
		TEST_METHOD(GeneralTimerTestTempo1_5)
		{
			GeneralTimer generalTimer;
			Assert::IsTrue(generalTimer.getTime() < 5);
			Uint32 time = SDL_GetTicks();
			generalTimer.setTempo(1.5);
			while (SDL_GetTicks() < time + 200)
			{
				generalTimer.updateTime();
			}
			Assert::IsTrue(generalTimer.getTime() < 315);
			Assert::IsTrue(generalTimer.getTime() > 285);
		}
		TEST_METHOD(GeneralTimerTestTempo0_5)
		{
			GeneralTimer generalTimer;
			Assert::IsTrue(generalTimer.getTime() < 5);
			Uint32 time = SDL_GetTicks();
			generalTimer.setTempo(0.5);
			while (SDL_GetTicks() < time + 200)
			{
				generalTimer.updateTime();
			}
			Assert::IsTrue(generalTimer.getTime() < 105);
			Assert::IsTrue(generalTimer.getTime() > 95);
		}
		TEST_METHOD(GeneralTimerTestPause)
		{
			GeneralTimer generalTimer;
			Assert::IsTrue(generalTimer.getTime() < 5);
			Uint32 time = SDL_GetTicks();
			generalTimer.setTempo(2.5);
			generalTimer.pause();
			while (SDL_GetTicks() < time + 100)
			{
				generalTimer.updateTime();
			}
			generalTimer.unpause();
			time = SDL_GetTicks();
			while (SDL_GetTicks() < time + 100)
			{
				generalTimer.updateTime();
			}
			Assert::IsTrue(generalTimer.getTime() < 2.5 * 105);
			Assert::IsTrue(generalTimer.getTime() > 2.5 * 95);
		}
		TEST_METHOD(SubTimerTest)
		{
			std::shared_ptr <GeneralTimer> generalTimer(new GeneralTimer);
			Assert::IsTrue((*generalTimer).getTime() < 5);
			Uint32 time = SDL_GetTicks();
			std::shared_ptr<SubTimer> subTimer (new SubTimer(generalTimer));
			while (SDL_GetTicks() < time + 200)
			{
				(*generalTimer).updateTime();
				(*subTimer).updateTime();
			}
			Assert::IsTrue((*subTimer).getTime() < 210);
			Assert::IsTrue((*subTimer).getTime() > 190);
		}
		TEST_METHOD(SubTimerTestDiffTempos)
		{
			std::shared_ptr <GeneralTimer> generalTimer(new GeneralTimer);
			Assert::IsTrue((*generalTimer).getTime() < 5);
			Uint32 time = SDL_GetTicks();
			std::shared_ptr<SubTimer> subTimer(new SubTimer(generalTimer));
			(*generalTimer).setTempo(2.0);
			(*subTimer).setTempo(0.5);
			while (SDL_GetTicks() < time + 200)
			{
				(*generalTimer).updateTime();
				(*subTimer).updateTime();
			}
			Assert::IsTrue((*subTimer).getTime() < 210);
			Assert::IsTrue((*subTimer).getTime() > 190);
		}
		TEST_METHOD(SubTimerPauseTest)
		{
			std::shared_ptr <GeneralTimer> generalTimer(new GeneralTimer);
			Assert::IsTrue((*generalTimer).getTime() < 5);
			Uint32 time = SDL_GetTicks();
			std::shared_ptr<SubTimer> subTimer(new SubTimer(generalTimer));
			(*generalTimer).setTempo(2.0);
			(*subTimer).pause();
			while (SDL_GetTicks() < time + 100)
			{
				(*generalTimer).updateTime();
				(*subTimer).updateTime();
			}
			time = SDL_GetTicks();
			(*subTimer).unpause();
			while (SDL_GetTicks() < time + 100)
			{
				(*generalTimer).updateTime();
				(*subTimer).updateTime();
			}
			Assert::IsTrue((*subTimer).getTime() < 210);
			Assert::IsTrue((*subTimer).getTime() > 190);
		}
	};
}
