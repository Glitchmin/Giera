#pragma once
#include "Position.h"
#include "NPCTypes.h"
#include "DrawableBoardEntity.h"
#include "DamageTypes.h"
#include "SpawningDetails.h"
#include "HittableBoardEntity.h"
#include "NPCHitbox.h"
#include "NPCObserver.h"
//#include "Inventory.h"
#include <vector>
using std::vector;
using npc_hp_t = unsigned int;

class Board;
class Inventory;


class AbstractNPC : public DrawableBoardEntity, public HittableBoardEntity, public std::enable_shared_from_this<AbstractNPC>
{
public:
	AbstractNPC();
	virtual string getTextureFilePath();
	void move(Position moveDifference);

	Position getPosition() const;
	virtual void updateDrawables() override;
	virtual void updateHitboxes() override;
	void addNPCObserver(weak_ptr<NPCObserver> observer);
	void setBoard(weak_ptr<Board> board);
	void notifyNPCObservers(NPCObserver::Change change);
	shared_ptr <Inventory> getInventory();
protected:
	npc_hp_t hp;
	Position position;
	pair<double, double> sizeXY;
	double height;
	vector <double> resitances;
	double armor;
	shared_ptr <Inventory> inventory;
	vector <SpawningDetails> drops;
	vector <weak_ptr<NPCObserver>> npcObservers;
	NPCTypes npcType;
	Position target;
	shared_ptr <Drawable> drawable;
	shared_ptr <NPCHitbox> hitbox;
	weak_ptr <Board> board;
};

