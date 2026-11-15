#pragma once
#include "Position.h"
#include "CharacterTypes.h"
#include "DrawableBoardEntity.h"
#include "DamageTypes.h"
#include "SpawningDetails.h"
#include "GameTime.h"
#include "HittableBoardEntity.h"
#include "CharacterHitbox.h"
#include "CharacterObserver.h"
#include "AttackInfo.h"
#include "HitInfo.h"
#include "ModifiersTypes.h"
#include "CharacterAttributeTypes.h"
#include <optional>
#include <vector>
using std::vector;
using character_hp_t = unsigned int;

class Board;
class Inventory;
class Damage;
class HpBarDrawable;
class AbstractWeapon;
class AbstractGearItem;
class AbstractEffect;
class Shield;
class Armor;
class RangedWeapon;
class Arrow;

/**
 * @brief Player and enemies. The fight system: attacks, the damage pipeline, blocking,
 * parrying, dashing, effects. Rules: docs/FightSystem.md.
 *
 * Combat code is split: setup / movement / melee / bow in AbstractCharacter.cpp,
 * damage pipeline, gear modifiers, reactions, dash and effects in AbstractCharacterCombat.cpp.
 */
class AbstractCharacter : public DrawableBoardEntity, public HittableBoardEntity, public std::enable_shared_from_this<AbstractCharacter>
{
public:
	AbstractCharacter();
	virtual string getTextureFilePath();
	virtual string getShadowFilePath();
	/// Walking (input / AI): respects canMove() and the speed multiplier.
	void move(Position moveDifference);

	Position getPosition() const;
	virtual void updateDrawables() override;
	virtual void updateHitboxes() override;
	void addCharacterObserver(weak_ptr<CharacterObserver> observer);
	void setBoard(weak_ptr<Board> board);
	void notifyCharacterObservers(CharacterObserver::Change change);
	shared_ptr <Inventory> getInventory() const;

	// ---------------------------------------------------------------- gear
	/// Equipped melee weapon; nullptr = bare hands (CombatRules::bareHandsDamage).
	virtual shared_ptr <AbstractWeapon> getSelectedWeapon() const;
	virtual shared_ptr <Shield> getSelectedShield() const;
	virtual shared_ptr <Armor> getSelectedArmor() const;
	/// Equipped bow; nullptr = can't shoot.
	virtual shared_ptr <RangedWeapon> getSelectedBow() const;
	/// Next arrow that would be shot (stays in the quiver), nullptr if none.
	virtual shared_ptr <Arrow> peekArrow() const;
	/// Removes the next arrow from the quiver.
	virtual shared_ptr <Arrow> takeArrow();
	/// The weapon of the current setup: the bow in bow mode (if equipped), else the melee weapon.
	shared_ptr<AbstractWeapon> getWeaponInHand() const;
	/// Shield and body armor - their modifiers affect everything you do.
	vector<shared_ptr<AbstractGearItem>> getWornGear() const;
	/// Sum of a gear modifier (in %) over the worn gear plus @p weapon (nullptr = worn gear only).
	double getModifierSum(ModifiersTypes type, const AbstractGearItem* weapon) const;
	Position getShieldPos() const;
	character_hp_t* getHpPtr();
	character_hp_t* getMaxHpPtr();
	virtual void updateBehaviour(Time timeDiff) = 0;

	// ---------------------------------------------------------------- attacking
	/**
	 * @brief Attack button pressed: a melee swing, or (bow mode with a bow and arrows)
	 * start drawing the bow towards @p target. Without a bow or arrows it is a melee swing.
	 */
	virtual void startAttack(Position target);
	/// Attack button held: re-aims a drawn bow.
	void aimAt(Position target);
	/// Attack button released: shoots if the bow is drawn at least CombatRules::minDrawToShoot.
	void releaseAttack(Position target);
	void startMelee(Position& target);
	void startDrawing(Position target);
	/// Lowers the bow without shooting; the arrow stays in the quiver.
	void cancelDrawing();
	/// Points along the flight of an arrow released now (start ... landing / wall). Empty when not drawing.
	std::vector<Position> getArrowTrajectory() const;
	/// Ground point where an arrow released now would land (nullopt when not drawing).
	std::optional<Position> getArrowLandingPoint() const;
	/// Releasing now would shoot (draw >= CombatRules::minDrawToShoot): the aim dots are green.
	bool canReleaseArrow() const;
	/// The aim line and dotted arc are shown while the bow is drawn.
	bool hasAimLine() const { return aimLineDrawable != nullptr || !aimDots.empty(); }
	size_t getAimDotCount() const;
	bool isDrawing() const;
	/// 0..1, how far the bow is drawn.
	double getDrawProgress() const;
	bool isAttacking() const;
	bool canShoot() const;
	/// Wind-up and recovery of a melee swing: weapon AT_SPD (or bare hands) split by
	/// CombatRules::meleeWindupShare, scaled by ATTACK_SPEED modifiers.
	std::pair<Time, Time> getMeleeTimings() const;
	/// Time to fully draw the equipped bow (DRAW_TIME scaled by ATTACK_SPEED modifiers).
	Time getDrawTime() const;
	/// A melee hit from the weapon in hand (damage modifiers applied, crit rolled).
	HitInfo makeMeleeHit();
	/// An arrow hit: (bow + arrow damage) x draw, modifiers and crit of the bow.
	HitInfo makeArrowHit(const shared_ptr<RangedWeapon>& bow, const shared_ptr<Arrow>& arrow, double draw);

	/// Per-frame update: attack timers, bow draw, dash, stagger, parry window, effects.
	virtual void updateAttack(Time timeDiff);
	void visualiseMeleeAttack();
	void updateMelee(Time& timeDiff, bool& retFlag);
	/// true = melee setup, false = bow setup. Use setMeleeMode() to switch.
	bool meleeMode = true;
	/// Switches between melee and bow. Bow mode needs an equipped bow. @return the new mode.
	bool setMeleeMode(bool melee);
	bool toggleWeaponMode();
	bool canMove() const;
	bool canAttack() const;
	/// Walking speed multiplier: busy (draw / shield / eating), MOVEMENT_SPEED modifiers, effects.
	double getMoveSpeedMultiplier() const;

	// ---------------------------------------------------------------- defending
	/// Block button held. With a shield: raises it over its raise time (its armor counts
	/// in proportion). Without a shield: pressing opens a timed parry window.
	virtual void parry(Time timeDiff);
	/// Block button released.
	void cancelParry();
	bool isParryWindowOpen() const;
	Time getParryCooldownLeft() const;
	/// Armor counted against @p type right now: body armor x its effectiveness vs the type,
	/// + shield armor x how far it is raised, + base armor. 0 for non-physical types.
	double getArmorAgainst(DamageTypes type) const;
	/// 0..1 share of a hit of @p type removed by armor after @p armorPiercing.
	double getDamageReduction(DamageTypes type, double armorPiercing) const;
	/// Armor of the character itself (enemies: CombatRules::enemyArmor).
	virtual double getBaseArmor() const;

	/// Resolves an incoming hit: dash, parry, armor, damage, shield stagger, lifesteal, effects.
	HitOutcome receiveHit(const HitInfo& hit);
	/// A plain hit of @p rawDamage (no crit / piercing / effects). @return true if it killed.
	bool receiveHit(double rawDamage, DamageTypes type = DamageTypes::SLASHING);
	/// Restores hp (never above max).
	void heal(double amount);

	/// Can't move or act for @p duration (on top of what's left of the current swing).
	void stagger(Time duration);
	bool isStaggered() const;

	/// Dash in the last walking direction (invulnerable while dashing). @return false if not possible.
	bool startDash();
	bool isDashing() const;
	Time getDashCooldownLeft() const;
	Position getLastMoveDirection() const;

	// ---------------------------------------------------------------- effects
	/// Applies a copy of @p effect. The same kind of effect (type, level) refreshes instead of stacking.
	void addEffect(const shared_ptr<AbstractEffect>& effect, weak_ptr<AbstractCharacter> origin);
	const vector<shared_ptr<AbstractEffect>>& getActiveEffects() const;
	/// Ticks damage / healing over time and expires effects.
	void updateEffects(Time timeDiff);
	/// Combined value (100 = normal) of active stat-changing effects on @p attribute.
	double getStatEffectPercent(CharacterAttributeTypes attribute) const;

	virtual void die();

	/// An arrow that hit and didn't break; dropped on the ground when the character dies.
	void addStuckArrow(shared_ptr<Arrow> arrow);
	const vector<shared_ptr<Arrow>>& getStuckArrows() const;

	/**
	 * @brief Subtracts damage from hp, clamping at 0 (hp is unsigned, so a plain
	 * subtraction used to wrap around to ~4 billion and the character never died).
	 * Calls die() exactly once when hp reaches 0.
	 * @return true if this hit killed the character.
	 */
	bool takeDamage(character_hp_t damage);
	bool isDead() const;
	/// Brings a dead character back with full hp at the given position.
	void revive(Position newPosition);
	character_hp_t getHp() const;
	character_hp_t getMaxHp() const;
	double getParryCompleteness() const;

protected:
	void generateShadowTexture();
	void showShieldDrawable();
	void removeShieldDrawable();
	void updateAimLine();
	void clearAimLine(bool notify);
	shared_ptr<Drawable> aimLineDrawable;
	vector<shared_ptr<Drawable>> aimDots;
	shared_ptr<Texture> aimLineTexture;
	shared_ptr<Texture> aimDotReadyTexture;
	shared_ptr<Texture> aimDotNotReadyTexture;
	/// Moves without canMove() / speed checks (walls still block). @return true if moved.
	bool performMove(Position moveDifference);
	/// Ends the current melee swing and removes its drawable.
	void endMeleeSwing();
	/// Something else occupies the hands (e.g. eating) - no attacking, walking is slowed.
	virtual bool isOccupied() const;
	/// Stagger, dash and death interrupt what isOccupied() reports.
	virtual void onInterrupted();
	/// Chance roll for crits etc. (honours CombatRules::forceRolls).
	bool rollCrit(HitInfo& hit, const AbstractGearItem* weapon);

	character_hp_t hp = 0;
	character_hp_t maxHp = 0;
	bool dead = false;
	Position position;
	optional<MeleeAttackInfo> meleeAttackInfo; //if not empty, character is attacking
	struct BowDrawInfo {
		double progress = 0.; ///< 0..1
		Position target;
	};
	optional<BowDrawInfo> bowDraw; //if not empty, the bow is being drawn
	bool movedSinceLastUpdate = false; ///< drawing / raising is slower while walking
	Position lastMoveDirection = Position(1, 0, 0);
	vector<shared_ptr<Arrow>> stuckArrows;
	pair<double, double> sizeXY{ 0., 0. };
	double height = 0.;
	vector <double> resitances; ///< damage multiplier per DamageTypes (1 = normal), non-physical types only
	double parryCompleteness = 0.; //values from 0 to 1.0
	bool parryHeld = false;        ///< block button currently held
	Time parryWindowLeft;          ///< no shield: timed parry window still open
	Time parryCooldownLeft;        ///< no shield: after a missed parry
	Time staggerLeft;
	Time dashLeft;
	Time dashCooldownLeft;
	Position dashDirection;
	vector<shared_ptr<AbstractEffect>> activeEffects;
	void dropStuckArrows();
	shared_ptr <Inventory> inventory;
	vector <weak_ptr<CharacterObserver>> characterObservers;
	CharacterTypes characterType;
	shared_ptr <Drawable> drawable;
	shared_ptr <Drawable> shadow_drawable;
	shared_ptr <HpBarDrawable> hpBarDrawable;
	shared_ptr <Drawable> shieldDrawable;
	shared_ptr <CharacterHitbox> hitbox;
	weak_ptr <Board> board;
	int level = 1;
};
