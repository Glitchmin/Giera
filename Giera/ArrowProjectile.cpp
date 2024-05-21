#include "pch.h"

#include "ArrowProjectile.h"
#include "Board.h"

using std::min;
using std::max;
using std::nullopt;

ArrowProjectile::ArrowProjectile(shared_ptr<FlightPath> flightPath, weak_ptr <HittableBoardEntity> entityToIgnore)
	:AbstractProjectile(flightPath, entityToIgnore)
{

	// generate shadow
	drawable = make_shared<Drawable>(flightPath->getPosition().grounded(),
		TextureLoader::makeUniColorTexture(20, 20, { 0, 0, 0, 128 }),
		Drawable::DrawableLayer::SHADOWS, make_pair(.05, 1), 0.5);
	drawables.push_back(drawable);

	// read texture not use hardcoded path
	string path = "../../save_files/tx/items/arrow/arrow0.png";
	drawable = make_shared<Drawable>(flightPath->getPosition(),
		TextureLoader::getTextureCopy(path),
		Drawable::DrawableLayer::ENTITIES, make_pair(1, 2), 1);
	drawables.push_back(drawable);
}

void ArrowProjectile::onWallHit(Coordinates hitCoords, shared_ptr<Board>& board)
{
	Logger::logDebug("arrow projectile hit the wall", hitCoords);
	isReadyToBeRemoved = true;
}

void ArrowProjectile::onCharacterHit(shared_ptr<AbstractCharacter> character, shared_ptr<Board>& board)
{
	Logger::logDebug("arrow projectile hit the Character");
	// use damage
	(*(character->getHpPtr())) -= 10;
	isReadyToBeRemoved = true;
}

void ArrowProjectile::onGroundHit(Coordinates hitCoords, shared_ptr<Board>& board)
{
	isReadyToBeRemoved = true;
}

optional<Position> ArrowProjectile::calculateHitbox(std::shared_ptr<Hitbox>& hitbox, LineSegment& ls, std::optional<Position>& prevCollisionP, Position& currPos)
{
	auto currCollision = hitbox->getFigure()->getLineSegmentIntersect(ls);
	if (currCollision.has_value()) {
		if (!prevCollisionP.has_value() || (*currCollision - currPos).getNormSq() < (*prevCollisionP - currPos).getNormSq()) {
			return currCollision;
		}
	}
	return nullopt;
}

void ArrowProjectile::move(Time& timeDiff, shared_ptr<Board>& board)
{
	Position prevPos = flightPath->getPosition();
	Position currPos = flightPath->updatePosition(timeDiff);
	if (currPos != prevPos) {
		notifyDrawableObservers(DrawableEntityObserver::Change::REMOVED);

		Position diff = currPos - prevPos;

		updateAngle(diff);
		drawables[0]->setAngle((float)(atan2(diff.getY(), diff.getX()) * 57.2957795130823 + 90.));
		drawables[0]->setHeightModifier(1.8 / sqrt((Calculator::squared(diff.getZ())/diff.grounded().getNormSq()) + 1));
		updateHeightModifier(diff);
		updateDrawables();
		notifyDrawableObservers(DrawableEntityObserver::Change::ADDED);
	}
	if (currPos.getX() < 0 || currPos.getX() >= board->getMap()->getSizeX() ||
		currPos.getY() < 0 || currPos.getY() >= board->getMap()->getSizeY()) {
		isReadyToBeRemoved = 1;
		return;
	}

	double maxSize = 2.;

	int minX = (int)max(min(prevPos.getX(), currPos.getX()) - maxSize, 0.0);
	int maxX = (int)min(max(prevPos.getX(), currPos.getX()) + maxSize, (double)board->getMap()->getSizeX());

	int minY = (int)max(min(prevPos.getY(), currPos.getY()) - maxSize, 0.0);
	int maxY = (int)min(max(prevPos.getY(), currPos.getY()) + maxSize, (double)board->getMap()->getSizeY());

	LineSegment ls(prevPos, currPos);
	optional<Position> collisionP;
	shared_ptr<AbstractCharacter> hitCharacter;
	optional <Coordinates> hitMapTile;

	for (int x = minX; x <= maxX; x++) {
		for (int y = minY; y <= maxY; y++) {
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


void ArrowProjectile::updateDrawables()
{
	drawables[0]->setPos(flightPath->getPosition().grounded());
	drawable->setPos(flightPath->getPosition());
}
