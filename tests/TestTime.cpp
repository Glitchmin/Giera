#include "TestFramework.h"
#include "GameTime.h"
#include "GeneralTimer.h"
#include "SubTimer.h"
#include <SDL.h>

TEST_CASE("Time", "operators")
{
    Time t1(100), t2(250);
    CHECK_EQ((t1 + t2).getTimeMs(), 350u);
    CHECK_EQ((t2 - t1).getTimeMs(), 150u);
    CHECK_EQ(t1.getTimeMs(), 100u);
    CHECK_EQ(t2.getTimeMs(), 250u);

    Time t5(4);
    t5 += t1;
    CHECK_EQ(t5.getTimeMs(), 104u);

    Time t6(104);
    t6 -= t1;
    CHECK_EQ(t6.getTimeMs(), 4u);

    CHECK_EQ((t1 * 1.5).getTimeMs(), 150u);
    CHECK_APPROX(t1.getTimeS(), 0.1);
    CHECK_APPROX(Time(300) / Time(600), 0.5);
}

TEST_CASE("Time", "subtraction saturates at zero")
{
    Time small(100), big(150);
    CHECK_EQ((small - big).getTimeMs(), 0u);
    small -= big;
    CHECK_EQ(small.getTimeMs(), 0u);
}

TEST_CASE("Time", "comparisons")
{
    CHECK(Time(1) < Time(2));
    CHECK(Time(3) > Time(2));
    CHECK(!(Time(2) < Time(2)));
    CHECK(!(Time(2) > Time(2)));
}

TEST_CASE("Time", "stream round trip")
{
    std::stringstream ss("1234");
    Time t;
    ss >> t;
    CHECK_EQ(t.getTimeMs(), 1234u);
}

TEST_CASE("GeneralTimer", "tempo 1.0 follows real time")
{
    GeneralTimer timer;
    CHECK(timer.getTime().getTimeMs() < 5);
    Uint32 start = SDL_GetTicks();
    while (SDL_GetTicks() < start + 200) {
        timer.updateTime();
    }
    CHECK_BETWEEN(timer.getTime().getTimeMs(), 185u, 230u);
}

TEST_CASE("GeneralTimer", "tempo 0.5 halves time")
{
    GeneralTimer timer;
    timer.setTempo(0.5);
    Uint32 start = SDL_GetTicks();
    while (SDL_GetTicks() < start + 200) {
        timer.updateTime();
    }
    CHECK_BETWEEN(timer.getTime().getTimeMs(), 90u, 115u);
}

TEST_CASE("GeneralTimer", "paused timer does not advance")
{
    GeneralTimer timer;
    timer.pause();
    Uint32 start = SDL_GetTicks();
    while (SDL_GetTicks() < start + 100) {
        timer.updateTime();
    }
    CHECK(timer.getTime().getTimeMs() < 10);
    timer.unpause();
    start = SDL_GetTicks();
    while (SDL_GetTicks() < start + 100) {
        timer.updateTime();
    }
    CHECK_BETWEEN(timer.getTime().getTimeMs(), 90u, 130u);
}

TEST_CASE("SubTimer", "follows general timer")
{
    auto general = std::make_shared<GeneralTimer>();
    auto sub = std::make_shared<SubTimer>(general);
    Uint32 start = SDL_GetTicks();
    while (SDL_GetTicks() < start + 200) {
        general->updateTime();
        sub->updateTime();
    }
    CHECK_BETWEEN(sub->getTime().getTimeMs(), 185u, 230u);
}

TEST_CASE("SubTimer", "tempos multiply")
{
    auto general = std::make_shared<GeneralTimer>();
    auto sub = std::make_shared<SubTimer>(general);
    general->setTempo(2.0);
    sub->setTempo(0.5);
    Uint32 start = SDL_GetTicks();
    while (SDL_GetTicks() < start + 200) {
        general->updateTime();
        sub->updateTime();
    }
    CHECK_BETWEEN(sub->getTime().getTimeMs(), 185u, 230u);
}

TEST_CASE("GeneralTimer", "tempo 1.5 speeds time up")
{
    GeneralTimer timer;
    timer.setTempo(1.5);
    Uint32 start = SDL_GetTicks();
    while (SDL_GetTicks() < start + 200) {
        timer.updateTime();
    }
    CHECK_BETWEEN(timer.getTime().getTimeMs(), 280u, 330u);
}

TEST_CASE("SubTimer", "pausing the sub timer stops only it")
{
    auto general = std::make_shared<GeneralTimer>();
    auto sub = std::make_shared<SubTimer>(general);
    sub->pause();
    Uint32 start = SDL_GetTicks();
    while (SDL_GetTicks() < start + 100) {
        general->updateTime();
        sub->updateTime();
    }
    CHECK(sub->getTime().getTimeMs() < 10);
    CHECK(general->getTime().getTimeMs() >= 90);
    sub->unpause();
    start = SDL_GetTicks();
    while (SDL_GetTicks() < start + 100) {
        general->updateTime();
        sub->updateTime();
    }
    CHECK_BETWEEN(sub->getTime().getTimeMs(), 90u, 130u);
}
