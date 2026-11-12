#include "TestFramework.h"
#include "Damage.h"
#include "DamageEffect.h"
#include "EffectsHandler.h"
#include "StatChangingEffect.h"
#include "EffectTypes.h"
#include "CharacterAttributeTypes.h"

namespace {
std::shared_ptr<AbstractCharacter> noCharacter() { return nullptr; }
}

TEST_CASE("Effects", "damage basics")
{
    Damage damage(2.0, -1, DamageTypes::POISON);
    CHECK_APPROX(damage.getValue(), 2.0);
    CHECK_EQ((int)damage.getDamageType(), (int)DamageTypes::POISON);
    damage.multiply(3);
    CHECK_APPROX(damage.getValue(), 6.0);
    CHECK(!damage.isLifesteal());
}

TEST_CASE("Effects", "damage effect constructor")
{
    DamageEffect effect(std::make_unique<Damage>(2.0, -1, DamageTypes::POISON), Time(1000),
        0, 1, noCharacter(), noCharacter(), Time(300));
    CHECK_APPROX(effect.getDamage()->getValue(), 2.0);
    CHECK_EQ(effect.getDuration().getTimeMs(), 1000u);
    CHECK_EQ(effect.getTimeLeft().getTimeMs(), 1000u);
    CHECK_EQ(effect.getTimeUntilTick().getTimeMs(), 300u);
    CHECK_EQ(effect.getTickrate().getTimeMs(), 300u);
    CHECK_APPROX(effect.getDamageIncrease(), 1.0);
}

TEST_CASE("Effects", "damage effect total damage")
{
    DamageEffect constant(std::make_unique<Damage>(2.0, -1, DamageTypes::POISON), Time(1000),
        0, 1, noCharacter(), noCharacter(), Time(300));
    CHECK_APPROX(constant.calculateTotalDamage(), 6.0);   // 3 ticks * 2
    DamageEffect growing(std::make_unique<Damage>(1.0, -1, DamageTypes::POISON), Time(1000),
        0, 1, noCharacter(), noCharacter(), Time(300), 2);
    CHECK_APPROX(growing.calculateTotalDamage(), 7.0);    // 1 + 2 + 4
}

TEST_CASE("Effects", "damage effect expires")
{
    DamageEffect effect(std::make_unique<Damage>(1.0, -1, DamageTypes::POISON), Time(1000),
        0, 1, noCharacter(), noCharacter(), Time(300));
    CHECK(!effect.subtractFromTimeLeft(Time(400)));
    CHECK_EQ(effect.getTimeLeft().getTimeMs(), 600u);
    CHECK(effect.subtractFromTimeLeft(Time(700)));
}

TEST_CASE("Effects", "stat changing effect")
{
    StatChangingEffect effect(Time(1000), 1, 1, noCharacter(), noCharacter(), 10, 15, Time(500), CharacterAttributeTypes::HP);
    CHECK_APPROX(effect.getCurrentValue(), 10.0);
    CHECK_EQ(effect.getTimeLeft().getTimeMs(), 1000u);
    CHECK_EQ((int)effect.getAttributeType(), (int)CharacterAttributeTypes::HP);
    double previous = effect.getCurrentValue();
    for (int i = 0; i < 10; i++) {
        effect.subtractFromTimeLeft(Time(50));
        CHECK(effect.getCurrentValue() >= previous);
        previous = effect.getCurrentValue();
    }
    CHECK_APPROX(effect.getCurrentValue(), 15.0);
}

TEST_CASE("Effects", "effects handler reads csv")
{
    auto dmg = EffectsHandler::getEffect<DamageEffect>(EffectTypes::DAMAGE_EFFECT, 0);
    REQUIRE(dmg != nullptr);
    CHECK(!dmg->IsBuff());
    CHECK_EQ(dmg->getDuration().getTimeMs(), 1000u);
    CHECK_EQ(dmg->getLevel(), (short)3);
    CHECK_APPROX(dmg->getDamage()->getValue(), 1.5);
    CHECK_EQ(dmg->getTickrate().getTimeMs(), 100u);
    CHECK_APPROX(dmg->getDamageIncrease(), 1.2);

    auto stat = EffectsHandler::getEffect<StatChangingEffect>(EffectTypes::STAT_CHANGING_EFFECT, 0);
    REQUIRE(stat != nullptr);
    CHECK(stat->IsBuff());
    CHECK_EQ(stat->getDuration().getTimeMs(), 800u);
    CHECK_APPROX(stat->getInitialPercentValue(), 50.0);
    CHECK_APPROX(stat->getFinalPercentValue(), 80.0);
    CHECK_EQ(stat->getTimeToReachFinal().getTimeMs(), 200u);
}
