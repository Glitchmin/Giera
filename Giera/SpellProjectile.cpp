#include "SpellProjectile.h"
#include "Drawable.h"

using std::min;
using std::max;

SpellProjectile::SpellProjectile(shared_ptr<FlightPath> flightPath, shared_ptr<ThrownSpell> spell)
	:AbstractProjectile(flightPath)
{
	this->spell = spell;
	drawable = make_shared<Drawable>(flightPath->getPosition(),
		TextureLoader::makeUniColorTexture(20, 20, { 255,0,0,255 }),
		Drawable::DrawableLayer::ENTITIES, make_pair(.3, .3), 0.1);
	drawables.push_back(drawable);
}

void SpellProjectile::onWallHit(shared_ptr<MapTile> tile)
{
	Logger::logDebug("projectile hit the wall");
}

void SpellProjectile::onNPCHit(shared_ptr<AbstractNPC> npc)
{
	Logger::logDebug("projectile hit the NPC");
}

void SpellProjectile::onGroundHit(shared_ptr<MapTile> tile)
{
	isReadyToBeRemoved = true;
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
	for (int x = minX; x <= maxX;x++) {
		for (int y = minY; y <= maxY;y++) {
			auto& hitboxes = board->getMap()->getMapTile(Coordinates(x, y))->getHitboxes();
			for (auto& hitbox : hitboxes) {
				calculateHitbox(hitbox, ls, collisionP, currPos);
			}
			auto& npcs = board->getBoardTile(Coordinates(x, y)).getNpcs();
			for (auto& npc : npcs) {
				for (auto& hitbox : npc->getHitboxes()) {
					calculateHitbox(hitbox, ls, collisionP, currPos);
				}
			}
		}
	}
	isReadyToBeRemoved = collisionP.has_value();
	isReadyToBeRemoved += currPos.getZ() < 0;
	if (isReadyToBeRemoved) {
		Logger::logInfo("removing the projectile, its heigth", currPos.getZ());
	}
	notifyDrawableObservers(DrawableEntityObserver::Change::REMOVED);
	updateDrawables();
	notifyDrawableObservers(DrawableEntityObserver::Change::ADDED);
}

void SpellProjectile::calculateHitbox(std::shared_ptr<Hitbox>& hitbox, LineSegment& ls, std::optional<Position>& collisionP, Position& currPos)
{
	auto currCollision = hitbox->getFigure()->getLineSegmentIntersect(ls);
	if (currCollision.has_value()) {
		if (!collisionP.has_value() || (*currCollision - currPos).getNormSq() < (*collisionP - currPos).getNormSq()) {
			collisionP = currCollision;
		}
	}
}

void SpellProjectile::updateDrawables()
{
	drawable->setPos(flightPath->getPosition());
}
