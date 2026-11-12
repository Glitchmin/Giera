#pragma once
#include "Damage.h"
class Lifesteal :
    public Damage
{
private:
    double probability;
    double percentStolen;
public:
    Lifesteal(double value,double ap, DamageTypes damageType,
        double percentStolen, double probability = 1.0, weak_ptr<AbstractCharacter> damageDealer=std::shared_ptr<AbstractCharacter>(nullptr));
    bool isLifesteal() override;
    double getProbability() const;
    double getPercentStolen() const;

};

