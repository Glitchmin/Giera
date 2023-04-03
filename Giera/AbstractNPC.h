#pragma once
#include "Position.h"
#include "NPCTypes.h"
#include "DrawableBoardEntity.h"
#include "DamageTypes.h"
#include "SpawningDetails.h"
#include "HittableBoardEntity.h"
#include "NPCHitbox.h"
#include <vector>
using std::vector;
using npc_hp_t = typename unsigned int;

class Board;

class AbstractNPC: public DrawableBoardEntity, public HittableBoardEntity
{
public:
	AbstractNPC();
	virtual string getTextureFilePath();
	void move(Position moveDifference);

	Position getPosition() const;
	virtual void updateDrawables() override;
	virtual void updateHitboxes() override;
	void setBoard(weak_ptr<Board> board);
protected:
	npc_hp_t hp;
	Position position;
	pair<double, double> sizeXY;
	double height;
	vector <double> resitances;
	double armor;
	//Inventory intentory;
	vector <SpawningDetails> drops;
	NPCTypes npcType;
	Position target;
	shared_ptr <Drawable> drawable;
	shared_ptr <NPCHitbox> hitbox;
	weak_ptr <Board> board;
};

