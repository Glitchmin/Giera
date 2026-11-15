#pragma once
#include "AbstractCharacter.h"
#include "CharacterTypes.h"

/// Enemy: walks to the player and fights with a melee weapon (CombatRules enemy* values).
class AiCharacter :
	public AbstractCharacter
{
public:
	AiCharacter(CharacterTypes characterType, Position pos, int level);
	virtual void updateBehaviour(Time timeDiff) override;
	void die() override;
	shared_ptr<AbstractWeapon> getSelectedWeapon() const override;
	/// CombatRules::enemyArmor (enemies wear no armor items yet).
	double getBaseArmor() const override;
protected:
	double attackRange = 1.0;   ///< meters
	double stopDistance = 0.8;  ///< meters - stop walking when this close to the player
	double walkSpeed = 1.0;     ///< meters per second
	shared_ptr<AbstractWeapon> weapon; ///< generated once (Melee.csv row CombatRules::enemyMeleeWeaponId)
	vector <SpawningDetails> drops;
	Position target;
};
