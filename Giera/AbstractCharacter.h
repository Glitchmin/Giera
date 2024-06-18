#pragma once
#include "Position.h"
#include "CharacterTypes.h"
#include "DrawableBoardEntity.h"
#include "DamageTypes.h"
#include "SpawningDetails.h"
#include "HittableBoardEntity.h"
#include "CharacterHitbox.h"
#include "CharacterObserver.h"
#include "AttackInfo.h"
#include <vector>
using std::vector;
using character_hp_t = unsigned int;

class Board;
class Inventory;
class Damage;

class AbstractCharacter : public DrawableBoardEntity, public HittableBoardEntity, public std::enable_shared_from_this<AbstractCharacter>
{
public:
	AbstractCharacter();
	virtual string getTextureFilePath();
	virtual string getShadowFilePath();
	void move(Position moveDifference);

	Position getPosition() const;
	virtual void updateDrawables() override;
	virtual void updateHitboxes() override;
	void addCharacterObserver(weak_ptr<CharacterObserver> observer);
	void setBoard(weak_ptr<Board> board);
	void notifyCharacterObservers(CharacterObserver::Change change);
	shared_ptr <Inventory> getInventory();
	character_hp_t* getHpPtr();
	character_hp_t* getMaxHpPtr();
	virtual void updateBehaviour(Time timeDiff) = 0;
	virtual void startAttack(Position target);
	//virtual void takeDamage(Damage damage);
	virtual void updateAttack(Time timeDiff);
	bool canMove();
	bool canAttack();
	virtual void die();
protected:
	void generateShadowTexture();
	character_hp_t hp;
	character_hp_t maxHp;
	bool isStunned = false;
	Position position;
	optional<AttackInfo> attackInfo; //if not empty, character is attacking
	pair<double, double> sizeXY;
	double height;
	vector <double> resitances;
	double armor;
	shared_ptr <Inventory> inventory;
	vector <weak_ptr<CharacterObserver>> characterObservers;
	CharacterTypes characterType;
	shared_ptr <Drawable> drawable;
	shared_ptr <Drawable> shadow_drawable;
	shared_ptr <CharacterHitbox> hitbox;
	weak_ptr <Board> board;
	int level;
};

