#include "SpellProjectile.h"
#include "Drawable.h"

using std::nullopt;

SpellProjectile::SpellProjectile(shared_ptr<FlightPath> flightPath, shared_ptr<ThrownSpell> spell, weak_ptr <HittableBoardEntity> entityToIgnore)
	:AbstractProjectile(flightPath, entityToIgnore)
{
	this->spell = spell;
	drawable = make_shared<Drawable>(flightPath->getPosition().grounded(),
		TextureLoader::makeUniColorTexture(20, 20, { 0, 0, 0, 125 }),
		Drawable::DrawableLayer::ENTITIES, make_pair(.3, .3), 0.1);
	drawables.push_back(drawable);

	drawable = make_shared<Drawable>(flightPath->getPosition(),
		TextureLoader::makeUniColorTexture(20, 20, { 255,0,0,255 }),
		Drawable::DrawableLayer::ENTITIES, make_pair(.3, .3), 0.1);
	drawables.push_back(drawable);
}

void SpellProjectile::onWallHit(Position hitPos, shared_ptr<Board>& board)
{
	Logger::logDebug("projectile hit the wall", hitPos);
	isReadyToBeRemoved = true;
 }

void SpellProjectile::onCharacterHit(shared_ptr<AbstractCharacter> character, shared_ptr<Board>& board)
{
	Logger::logDebug("projectile hit the Character");
	(*(character->getHpPtr()))-=20;
	isReadyToBeRemoved = true;
}

void SpellProjectile::onGroundHit(Position hitPos, shared_ptr<Board>& board)
{
	isReadyToBeRemoved = true;
}

void SpellProjectile::move(Time& timeDiff, shared_ptr<Board>& board)
{
	Position prevPos = flightPath->getPosition();
	Position currPos = flightPath->updatePosition(timeDiff);

	if (currPos != prevPos) {
		notifyDrawableObservers(DrawableEntityObserver::Change::REMOVED);

		Position diff = currPos - prevPos;

		updateAngle(diff);
		drawables[0]->setAngle(atan2(diff.getY(), diff.getX()) * 57.2957795130823 + 90);
		drawables[0]->setHeightModifier(1 - abs(atan2(diff.grounded().getNorm(), diff.getZ())) * 0.636619772367581);
		updateHeightModifier(diff);
		updateDrawables();
		notifyDrawableObservers(DrawableEntityObserver::Change::ADDED);
	}
	if (currPos.getX() < 0 || currPos.getX() >= board->getMap()->getSizeX() ||
		currPos.getY() < 0 || currPos.getY() >= board->getMap()->getSizeY()) {
		isReadyToBeRemoved = 1;
		return;
	}

	auto hitResult = board->calculateHit(LineSegment(prevPos, currPos), entityToIgnore.lock());
	if (!hitResult.has_value()) {
		return;
	}
	if (hitResult.value().character.has_value()) {
		onCharacterHit(hitResult.value().character.value(), board);
		return;
	}
	if (hitResult.value().mapHit.has_value()) {
		if (hitResult.value().mapHit.value().getZ()<=0.) {
			onGroundHit(hitResult.value().mapHit.value(), board);
		}
		else {
			onWallHit(hitResult.value().mapHit.value(), board);
		}
	}
}


void SpellProjectile::updateDrawables()
{
	drawables[0]->setPos(flightPath->getPosition().grounded());
	drawable->setPos(flightPath->getPosition());
}
