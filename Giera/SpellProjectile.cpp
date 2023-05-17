#include "SpellProjectile.h"
#include "Drawable.h"

using std::min;
using std::max;
using std::nullopt;

SpellProjectile::SpellProjectile(shared_ptr<FlightPath> flightPath, shared_ptr<ThrownSpell> spell, weak_ptr <HittableBoardEntity> entityToIgnore)
	:AbstractProjectile(flightPath, entityToIgnore)
{
	this->spell = spell;
	drawable = make_shared<Drawable>(flightPath->getPosition(),
		TextureLoader::makeUniColorTexture(20, 20, { 255,0,0,255 }),
		Drawable::DrawableLayer::ENTITIES, make_pair(.3, .3), 0.1);
	drawables.push_back(drawable);
}

void SpellProjectile::onWallHit(Coordinates hitCoords, shared_ptr<Board>& board)
{
	Logger::logDebug("projectile hit the wall", hitCoords);
 }

void SpellProjectile::onNPCHit(shared_ptr<AbstractNPC> npc, shared_ptr<Board>& board)
{
	Logger::logDebug("projectile hit the NPC");
}

void SpellProjectile::onGroundHit(Coordinates hitCoords, shared_ptr<Board>& board)
{
	isReadyToBeRemoved = true;
}

optional<Position> SpellProjectile::calculateHitbox(std::shared_ptr<Hitbox>& hitbox, LineSegment& ls, std::optional<Position>& collisionP, Position& currPos)
{
	auto currCollision = hitbox->getFigure()->getLineSegmentIntersect(ls);
	if (currCollision.has_value()) {
		if (!collisionP.has_value() || (*currCollision - currPos).getNormSq() < (*collisionP - currPos).getNormSq()) {
			return currCollision;
		}
	}
	return nullopt;
}

void SpellProjectile::move(Time& timeDiff, shared_ptr<Board>& board)
{
	Position prevPos = flightPath->getPosition();
	Position currPos = flightPath->updatePosition(timeDiff);
	notifyDrawableObservers(DrawableEntityObserver::Change::REMOVED);
	updateDrawables();
	notifyDrawableObservers(DrawableEntityObserver::Change::ADDED);
	if (currPos.getX() < 0 || currPos.getX() >= board->getMap()->getSizeX() ||
		currPos.getY() < 0 || currPos.getY() >= board->getMap()->getSizeY()) {
		isReadyToBeRemoved = 1;
		return;
	}
	int minX = min(prevPos.getX(), currPos.getX());
	int maxX = max(prevPos.getX(), currPos.getX());
	int minY = min(prevPos.getY(), currPos.getY());
	int maxY = max(prevPos.getY(), currPos.getY());
	LineSegment ls(prevPos, currPos);
	optional<Position> collisionP;
	shared_ptr<AbstractNPC> hitNPC;
	optional <Coordinates> hitMapTile;

	for (int x = minX; x <= maxX+1;x++) {
		for (int y = minY; y <= maxY+1;y++) {
			auto mapTile = board->getMap()->getMapTile(Coordinates(x, y));
			if (mapTile != entityToIgnore.lock()) {
				auto& hitboxes = mapTile->getHitboxes();
				for (auto& hitbox : hitboxes) {
					auto currCollision = calculateHitbox(hitbox, ls, collisionP, currPos);
					if (currCollision.has_value()) {
						hitMapTile = Coordinates(x, y);
						Logger::logInfo(x, y, "map tile");
						hitNPC = nullptr;
						collisionP = currCollision;
					}
				}
			}

			auto& npcs = board->getBoardTile(Coordinates(x, y)).getNpcs();
			for (auto& npc : npcs) {
				if (npc != entityToIgnore.lock()) {
					for (auto& hitbox : npc->getHitboxes()) {
						auto currCollision = calculateHitbox(hitbox, ls, collisionP, currPos);
						if (currCollision.has_value()) {
							hitMapTile = nullopt;
							Logger::logInfo(x, y, "npc");
							hitNPC = npc;
							collisionP = currCollision;
						}
					}
				}
			}
		}
	}
	isReadyToBeRemoved = collisionP.has_value();
	isReadyToBeRemoved += currPos.getZ() <= 0;
	if (!isReadyToBeRemoved) {
		return;
	}
	Logger::logInfo("removing the projectile, its heigth", currPos.getZ(),hitMapTile.has_value(),hitNPC!=nullptr);
	if (hitNPC) {
		onNPCHit(hitNPC, board);
		return;
	}
	if (hitMapTile) {
		onWallHit(hitMapTile.value(), board);
		return;
	}
	if (currPos.getZ() <= 0) {
		onGroundHit(currPos, board);
		return;
	}
	
}


void SpellProjectile::updateDrawables()
{
	drawable->setPos(flightPath->getPosition());
}
