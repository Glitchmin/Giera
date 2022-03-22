#include "pch.h"
#include "CppUnitTest.h"
#include "../Giera/AbstractTimer.h"
#include "../Giera/GeneralTimer.h"
#include "../Giera/SubTimer.h"
#include "../Giera/Time.h"
#include "../Giera/Calculator.h"
#include "../Giera/GeneralTimer.cpp"
#include "../Giera/SubTimer.cpp"
#include "../Giera/AbstractTimer.cpp"
#include "../Giera/Time.cpp"
#include "../Giera/Calculator.cpp"
#include <iostream>
#include <string>
#include <SDL.h>
#include <Windows.h>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UtilityTests
{
	TEST_CLASS(TimeTests)
	{
	public:
		//those tests might not be working if launched on a VERY slow (or occupied with something else) device. Just rerun them before drawing conclusions
		TEST_METHOD(TimeOperatorsOverloadTest)
		{
			Time time1(100);
			Time time2(250);
			Time time3 = time1 + time2;
			Assert::AreEqual(time3.getTimeMs(), (unsigned int)350);
			Assert::AreEqual(time1.getTimeMs(), (unsigned int)100);
			Assert::AreEqual(time2.getTimeMs(), (unsigned int)250);

			Time time4 = time2 - time1;
			Assert::AreEqual(time4.getTimeMs(), (unsigned int)150);
			Assert::AreEqual(time1.getTimeMs(), (unsigned int)100);
			Assert::AreEqual(time2.getTimeMs(), (unsigned int)250);

			Time time5(4);
			time5 += time1;
			Assert::AreEqual(time5.getTimeMs(), (unsigned int)104);
			Assert::AreEqual(time1.getTimeMs(), (unsigned int)100);

			Time time6(104);
			time6 -= time1;
			Assert::AreEqual(time6.getTimeMs(), (unsigned int)4);
			Assert::AreEqual(time1.getTimeMs(), (unsigned int)100);

			Time time7 = time1 * 1.5;
			Assert::AreEqual(time7.getTimeMs(), (unsigned int)150);
			Assert::AreEqual(time1.getTimeMs(), (unsigned int)100);

			Assert::AreEqual(time1.getTimeS(),0.1);
		}
		TEST_METHOD(GeneralTimerTestTempo1_0)
		{
			GeneralTimer generalTimer;
			Assert::IsTrue(generalTimer.getTime().getTimeMs() < 5);
			unsigned int time = SDL_GetTicks();
			while (SDL_GetTicks() < time + 200)
			{
				generalTimer.updateTime();
			}
			Assert::IsTrue(generalTimer.getTime().getTimeMs() < 210);
			Assert::IsTrue(generalTimer.getTime().getTimeMs() > 190);
		}
		TEST_METHOD(GeneralTimerTestTempo1_5)
		{
			GeneralTimer generalTimer;
			Assert::IsTrue(generalTimer.getTime().getTimeMs() < 5);
			unsigned int time = SDL_GetTicks();
			generalTimer.setTempo(1.5);
			Time timeChange(0);
			while (SDL_GetTicks() < time + 200)
			{
				timeChange += generalTimer.updateTime();
			}
			Assert::AreEqual(timeChange.getTimeMs(), generalTimer.getTime().getTimeMs());
			Assert::IsTrue(generalTimer.getTime().getTimeMs() < 315);
			Assert::IsTrue(generalTimer.getTime().getTimeMs() > 285);
		}
		TEST_METHOD(GeneralTimerTestTempo0_5)
		{
			GeneralTimer generalTimer;
			Time answer = generalTimer.getTime();
			Assert::IsTrue(answer.getTimeMs() < 5);
			unsigned int time = SDL_GetTicks();
			generalTimer.setTempo(0.5);
			while (SDL_GetTicks() < time + 200)
			{
				generalTimer.updateTime();
			}
			answer = generalTimer.getTime();

			Assert::IsTrue(answer.getTimeMs() < 105);
			Assert::IsTrue(answer.getTimeMs() > 95);
		}
		TEST_METHOD(GeneralTimerTestPause)
		{
			GeneralTimer generalTimer;
			Assert::IsTrue(generalTimer.getTime().getTimeMs() < 5);
			unsigned int time = SDL_GetTicks();
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
			Assert::IsTrue(generalTimer.getTime().getTimeMs() < 2.5 * 105);
			Assert::IsTrue(generalTimer.getTime().getTimeMs() > 2.5 * 95);
		}
		TEST_METHOD(SubTimerTest)
		{
			std::shared_ptr <GeneralTimer> generalTimer(new GeneralTimer);
			Assert::IsTrue(generalTimer->getTime().getTimeMs() < 5);
			unsigned int time = SDL_GetTicks();
			std::shared_ptr<SubTimer> subTimer(new SubTimer(generalTimer));
			while (SDL_GetTicks() < time + 200)
			{
				generalTimer->updateTime();
				subTimer->updateTime();
			}
			Assert::IsTrue(subTimer->getTime().getTimeMs() < 210);
			Assert::IsTrue(subTimer->getTime().getTimeMs() > 190);
		}
		TEST_METHOD(SubTimerTestDiffTempos)
		{
			std::shared_ptr <GeneralTimer> generalTimer(new GeneralTimer);
			Assert::IsTrue(generalTimer->getTime().getTimeMs() < 5);
			unsigned int time = SDL_GetTicks();
			std::shared_ptr<SubTimer> subTimer(new SubTimer(generalTimer));
			generalTimer->setTempo(2.0);
			subTimer->setTempo(0.5);
			while (SDL_GetTicks() < time + 200)
			{
				generalTimer->updateTime();
				subTimer->updateTime();
			}
			Assert::IsTrue(subTimer->getTime().getTimeMs() < 210);
			Assert::IsTrue(subTimer->getTime().getTimeMs() > 190);
		}
		TEST_METHOD(SubTimerPauseTest)
		{
			std::shared_ptr <GeneralTimer> generalTimer(new GeneralTimer);
			Assert::IsTrue(generalTimer->getTime().getTimeMs() < 5);
			unsigned int time = SDL_GetTicks();
			std::shared_ptr<SubTimer> subTimer(new SubTimer(generalTimer));
			generalTimer->setTempo(2.0);
			subTimer->pause();
			while (SDL_GetTicks() < time + 100)
			{
				generalTimer->updateTime();
				subTimer->updateTime();
			}
			time = SDL_GetTicks();
			subTimer->unpause();
			while (SDL_GetTicks() < time + 100)
			{
				generalTimer->updateTime();
				subTimer->updateTime();
			}
			Assert::IsTrue(subTimer->getTime().getTimeMs() < 210);
			Assert::IsTrue(subTimer->getTime().getTimeMs() > 190);
		}
	};
	TEST_CLASS(CalculatorTests)
	{
	public:
		//those tests might not be working if launched on a VERY slow (or occupied with something else) device. Just rerun them before drawing conclusions
		TEST_METHOD(getIntFromDoubleWithProbTest) {
			srand(time(NULL));

			int wyn, sum = 0;

			for (int i = 0; i < 1000; i++) {
				wyn = Calculator::getIntFromDoubleWithProb(6.9);
				Assert::IsTrue(wyn == 6 || wyn == 7);
				sum += wyn;
			}

			Assert::IsTrue(sum >= 6800 && sum <= 7000);
		}
		TEST_METHOD(calculateChanceTest) {
			srand(time(NULL));

			int sum = 0;
			for (int i = 0; i < 1000; i++)
				sum += Calculator::calculateChance(0.25);

			Assert::IsTrue(sum >= 200 && sum <= 300);

			sum = 0;
			for (int i = 0; i < 1000; i++)
				sum += Calculator::calculateChance(0.5);

			Assert::IsTrue(sum >= 450 && sum <= 550);

			sum = 0;
			for (int i = 0; i < 1000; i++)
				sum += Calculator::calculateChance(0.75);

			Assert::IsTrue(sum >= 700 && sum <= 800);
		}
		TEST_METHOD(getRandomDoubleTest) {
			srand(time(NULL));

			double wyn = 0;
			int _min = 0, _max = 0, wow = 0;
			unsigned int sum[25] = {};

			for (int i = 0; i < 1000; i++) {
				wyn = Calculator::getRandomDouble(0, 25);
				Assert::IsTrue(wyn >= 0 && wyn <= 25);
				if ((int)wyn >= 25) {
					wow++;  // how many times the double got really close to 25
					continue;
				}
				sum[int(wyn)]++;
				_min = min(_min, sum[int(wyn)]);
				_max = max(_max, sum[int(wyn)]);
			}

			Assert::IsTrue(_max - _min <= 75);
			Assert::IsTrue(wow < 2); // very not likely to happen even once
		}
		TEST_METHOD(getRandomIntTest) {
			srand(time(NULL));

			int wyn = 0, _min = 0, _max = 0;
			unsigned int sum[26] = {};

			for (int i = 0; i < 1000; i++) {
				wyn = Calculator::getRandomInt(0, 25);
				Assert::IsTrue(wyn >= 0 && wyn <= 25);
				sum[wyn]++;
				_min = min(_min, sum[wyn]);
				_max = max(_max, sum[wyn]);
			}
			Assert::IsTrue(_max - _min <= 75);
		}
	};
}
