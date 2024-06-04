#include "pch.h"

#include "SpellProjectile.h"

using std::min;
using std::max;
using std::nullopt;

SpellProjectile::SpellProjectile(shared_ptr<FlightPath> flightPath, shared_ptr<ThrownSpell> spell, weak_ptr <HittableBoardEntity> entityToIgnore)
	:AbstractProjectile(flightPath, entityToIgnore)
{
	this->spell = spell;
	drawable = make_shared<Drawable>(flightPath->getPosition().grounded(),
		TextureLoader::makeUniColorTexture(20, 20, { 0, 0, 0, 128 }),
		Drawable::DrawableLayer::ENTITIES, make_pair(.3, .3), 0.1);
	drawables.push_back(drawable);

	drawable = make_shared<Drawable>(flightPath->getPosition(),
		TextureLoader::makeUniColorTexture(20, 20, { 255,0,0,255 }),
		Drawable::DrawableLayer::ENTITIES, make_pair(.3, .3), 0.1);
	drawables.push_back(drawable);
}

void SpellProjectile::onWallHit(Coordinates hitCoords, shared_ptr<Board>& board)
{
	Logger::logDebug("spell projectile hit the wall", hitCoords);
	isReadyToBeRemoved = true;
 }

void SpellProjectile::onCharacterHit(shared_ptr<AbstractCharacter> character, shared_ptr<Board>& board)
{
	Logger::logDebug("projectile hit the Character");
	(*(character->getHpPtr()))-=20;
	isReadyToBeRemoved = true;
}

void SpellProjectile::onGroundHit(Coordinates hitCoords, shared_ptr<Board>& board)
{
	isReadyToBeRemoved = true;
}

optional<Position> SpellProjectile::calculateHitbox(std::shared_ptr<Hitbox>& hitbox, LineSegment& ls, std::optional<Position>& prevCollisionP, Position& currPos)
{
	auto currCollision = hitbox->getFigure()->getLineSegmentIntersect(ls);
	if (currCollision.has_value()) {
		if (!prevCollisionP.has_value() || (*currCollision - currPos).getNormSq() < (*prevCollisionP - currPos).getNormSq()) {
			return currCollision;
		}
	}
	return nullopt;
}

void SpellProjectile::move(Time& timeDiff, shared_ptr<Board>& board)
{
	Position prevPos = flightPath->getPosition();
	Position currPos = flightPath->updatePosition(timeDiff);

	if (currPos != prevPos) {
		notifyDrawableObservers(DrawableEntityObserver::Change::REMOVED);

		Position diff = currPos - prevPos;

		updateAngle(diff);
		drawables[0]->setAngle((float)(atan2(diff.getY(), diff.getX()) * 57.2957795130823 + 90));
		drawables[0]->setLengthModifier(1 - abs(atan2(diff.grounded().getNorm(), diff.getZ())) * 0.636619772367581);
		updateLengthModifier(diff);
		updateDrawables();
		notifyDrawableObservers(DrawableEntityObserver::Change::ADDED);
	}
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
	shared_ptr<AbstractCharacter> hitCharacter;
	optional <Coordinates> hitMapTile;

	for (int x = minX; x <= maxX;x++) {
		for (int y = minY; y <= maxY;y++) {
			auto mapTile = board->getMap()->getMapTile(Coordinates(x, y));
			if (mapTile != entityToIgnore.lock()) {
				auto& hitboxes = mapTile->getHitboxes();
				for (auto& hitbox : hitboxes) {
					auto currCollision = calculateHitbox(hitbox, ls, collisionP, currPos);
					if (currCollision.has_value()) {
						hitMapTile = Coordinates(x, y);
						//Logger::logInfo(x, y, "map tile");
						hitCharacter = nullptr;
						collisionP = currCollision;
					}
				}
			}

			auto& characters = board->getBoardTile(Coordinates(x, y)).getcharacters();
			for (auto& character : characters) {
				if (character != entityToIgnore.lock()) {
					for (auto& hitbox : character->getHitboxes()) {
						auto currCollision = calculateHitbox(hitbox, ls, collisionP, currPos);
						if (currCollision.has_value()) {
							hitMapTile = nullopt;
							Logger::logInfo(x, y, "character");
							hitCharacter = character;
							collisionP = currCollision;
						}
					}
				}
			}
		}
	}
	if (hitCharacter) {
		onCharacterHit(hitCharacter, board);
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
	drawables[0]->setPos(flightPath->getPosition().grounded());
	drawable->setPos(flightPath->getPosition());
}
