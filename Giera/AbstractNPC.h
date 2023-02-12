#pragma once
#include "Position.h"
#include "NPCTypes.h"
#include "DrawableBoardEntity.h"
#include "DamageTypes.h"
#include "SpawningDetails.h"
#include <vector>
using std::vector;
using npc_hp_t = typename unsigned int;

class Board;

class AbstractNPC: public DrawableBoardEntity
{
public:
	AbstractNPC();
	virtual string getTextureFilePath();
	void move(Position moveDifference);

	Position getPosition() const;
	virtual void updateDrawables() override;
	void setBoard(weak_ptr<Board> board);
protected:
	npc_hp_t hp;
	Position position;
	double width;
	double length;
	double height;
	vector <double> resitances;
	double armor;
	//Inventory intentory;
	vector <SpawningDetails> drops;
	NPCTypes npcType;
	Position target;
	shared_ptr <Sprite> sprite;
	weak_ptr <Board> board;
};

