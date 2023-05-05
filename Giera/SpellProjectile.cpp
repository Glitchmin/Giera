#include "SpellProjectile.h"
#include "Drawable.h"

using std::min;
using std::max;
using std::nullopt;

SpellProjectile::SpellProjectile(shared_ptr<FlightPath> flightPath, shared_ptr<ThrownSpell> spell)
	:AbstractProjectile(flightPath)
{
	this->spell = spell;
	drawable = make_shared<Drawable>(flightPath->getPosition(),
		TextureLoader::makeUniColorTexture(20, 20, { 255,0,0,255 }),
		Drawable::DrawableLayer::ENTITIES, make_pair(.3, .3), 0.1);
	drawables.push_back(drawable);
}

void SpellProjectile::onWallHit(Coordinates hitCoords, shared_ptr<Board>& board)
{
	Logger::logDebug("projectile hit the wall");
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
	int minX = min(prevPos.getX(), currPos.getX());
	int maxX = max(prevPos.getX(), currPos.getX());
	int minY = min(prevPos.getY(), currPos.getY());
	int maxY = max(prevPos.getY(), currPos.getY());
	LineSegment ls(prevPos, currPos);
	optional<Position> collisionP;
	shared_ptr<AbstractNPC> hitNPC;
	optional <Coordinates> hitMapTile;


	for (int x = minX; x <= maxX;x++) {
		for (int y = minY; y <= maxY;y++) {
			auto& hitboxes = board->getMap()->getMapTile(Coordinates(x, y))->getHitboxes();
			for (auto& hitbox : hitboxes) {
				auto currCollision = calculateHitbox(hitbox, ls, collisionP, currPos);
				if (currCollision.has_value()) {
					hitMapTile = Coordinates(x, y);
				}
			}
			auto& npcs = board->getBoardTile(Coordinates(x, y)).getNpcs();
			for (auto& npc : npcs) {
				for (auto& hitbox : npc->getHitboxes()) {
					auto currCollision = calculateHitbox(hitbox, ls, collisionP, currPos);
					if (currCollision.has_value()) {
						hitMapTile = nullopt;
						hitNPC = npc;
					}
				}
			}
		}
	}
	isReadyToBeRemoved = collisionP.has_value();
	isReadyToBeRemoved += currPos.getZ() <= 0;
	if (isReadyToBeRemoved) {
		Logger::logInfo("removing the projectile, its heigth", currPos.getZ());
		if (hitNPC) {
			onNPCHit(hitNPC, board);
		}
		else if (hitMapTile) {
			onWallHit(hitMapTile.value(), board);
		}
		else if (currPos.getZ() <= 0) {
			onGroundHit(currPos, board);
		}
	}
	notifyDrawableObservers(DrawableEntityObserver::Change::REMOVED);
	updateDrawables();
	notifyDrawableObservers(DrawableEntityObserver::Change::ADDED);
}


void SpellProjectile::updateDrawables()
{
	drawable->setPos(flightPath->getPosition());
}
