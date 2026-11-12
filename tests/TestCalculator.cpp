#include "TestFramework.h"
#include "Calculator.h"

TEST_CASE("Calculator", "getIntFromDoubleWithProb rounds randomly")
{
    int sum = 0;
    for (int i = 0; i < 2000; i++) {
        unsigned int v = Calculator::getIntFromDoubleWithProb(6.9);
        CHECK(v == 6 || v == 7);
        sum += v;
    }
    // expectation 13800, generous bounds
    CHECK_BETWEEN(sum, 13650, 13950);
}

TEST_CASE("Calculator", "getIntFromDoubleWithProb integers are exact")
{
    for (int i = 0; i < 100; i++) {
        CHECK_EQ(Calculator::getIntFromDoubleWithProb(3.0), 3u);
        CHECK_EQ(Calculator::getIntFromDoubleWithProb(0.0), 0u);
    }
}

TEST_CASE("Calculator", "calculateChance frequency")
{
    for (double p : { 0.25, 0.5, 0.75 }) {
        int hits = 0;
        for (int i = 0; i < 4000; i++) {
            hits += Calculator::calculateChance(p);
        }
        CHECK_BETWEEN(hits / 4000.0, p - 0.05, p + 0.05);
    }
}

TEST_CASE("Calculator", "random ranges")
{
    for (int i = 0; i < 2000; i++) {
        double d = Calculator::getRandomDouble(-2.5, 7.5);
        CHECK_BETWEEN(d, -2.5, 7.5);
        int n = Calculator::getRandomInt(15, 17);
        CHECK_BETWEEN(n, 15, 17);
    }
}

TEST_CASE("Calculator", "random int covers whole range")
{
    bool seen[3] = { false, false, false };
    for (int i = 0; i < 500; i++) {
        seen[Calculator::getRandomInt(0, 2)] = true;
    }
    CHECK(seen[0] && seen[1] && seen[2]);
}
