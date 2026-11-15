#include "ArrowProjectile.h"
#include "Board.h"
#include "Calculator.h"
#include "TextureLoader.h"
#include <cmath>

namespace {
constexpr double RAD_TO_DEG = 57.2957795130823;
}

ArrowProjectile::ArrowProjectile(shared_ptr<FlightPath> flightPath, shared_ptr<Arrow> arrow, HitInfo hit,
	weak_ptr<HittableBoardEntity> entityToIgnore)
	: AbstractProjectile(flightPath, entityToIgnore), arrow(arrow), hit(std::move(hit)), lastPos(flightPath->getPosition())
{
	shadowDrawable = make_shared<Drawable>(flightPath->getPosition().grounded(),
		TextureLoader::makeUniColorTexture(4, 20, { 0, 0, 0, 110 }),
		Drawable::DrawableLayer::SHADOWS, make_pair(.06, .5), 0.);
	drawables.push_back(shadowDrawable);

	drawable = make_shared<Drawable>(flightPath->getPosition(),
		TextureLoader::getTexturePtr(arrow->getFilePath()),
		Drawable::DrawableLayer::ENTITIES, make_pair(.8, .8), 0.); // square texture, arrow along its middle
	drawables.push_back(drawable);
}

bool ArrowProjectile::rollBreak()
{
	broken = Calculator::calculateChance(arrow->getBreakChance());
	return broken;
}

void ArrowProjectile::onWallHit(Position hitPos, shared_ptr<Board>& board)
{
	isReadyToBeRemoved = true;
	if (rollBreak()) {
		return;
	}
	// Leave the arrow in front of the wall. Wall hitboxes are smaller than their tile,
	// so the last position can still be inside the (unwalkable) wall tile: step back
	// towards the shooter until the spot can be reached.
	Position spot = lastPos.grounded();
	Position back = (lastPos - hitPos).grounded();
	double length = back.getNorm();
	back = length > 1e-9 ? back * (0.1 / length) : Position(0, 0, 0);
	for (int i = 0; i < 20 && !board->isStepablePosition(spot); i++) {
		spot = spot + back;
	}
	dropPosition = spot;
}

void ArrowProjectile::onCharacterHit(shared_ptr<AbstractCharacter> character, shared_ptr<Board>& board)
{
	isReadyToBeRemoved = true;
	auto outcome = character->receiveHit(hit);
	if (rollBreak()) {
		return;
	}
	if (outcome.landed) {
		stuckIn = character;
	}
	else {
		dropPosition = lastPos.grounded(); // dodged: the arrow falls where it was
	}
}

void ArrowProjectile::onGroundHit(Position hitPos, shared_ptr<Board>& board)
{
	isReadyToBeRemoved = true;
	if (!rollBreak()) {
		Position ground = hitPos.grounded();
		dropPosition = board->isStepablePosition(ground) ? ground : lastPos.grounded();
	}
}

void ArrowProjectile::onRemoved(shared_ptr<Board>& board)
{
	if (auto character = stuckIn.lock()) {
		if (character->isDead()) {
			board->dropItem(character->getPosition(), arrow); // killed by this very arrow
		}
		else {
			character->addStuckArrow(arrow);
		}
	}
	else if (dropPosition.has_value()) {
		board->dropItem(dropPosition.value(), arrow);
	}
}

void ArrowProjectile::updateRotation(Position diff)
{
	// The texture points up; SDL rotates clockwise. On screen y grows downwards and
	// height (z) moves things up, so the on-screen direction is (dx, dy - dz).
	drawable->setAngle((float)(atan2(diff.getX(), -(diff.getY() - diff.getZ())) * RAD_TO_DEG));
	shadowDrawable->setAngle((float)(atan2(diff.getX(), -diff.getY()) * RAD_TO_DEG));
}

void ArrowProjectile::move(Time& timeDiff, shared_ptr<Board>& board)
{
	Position prevPos = flightPath->getPosition();
	Position currPos = flightPath->updatePosition(timeDiff);
	lastPos = prevPos;

	if (currPos != prevPos) {
		notifyDrawableObservers(DrawableEntityObserver::Change::REMOVED);
		updateRotation(currPos - prevPos);
		updateDrawables();
		notifyDrawableObservers(DrawableEntityObserver::Change::ADDED);
	}
	// "!(inside)" so that NaN also counts as outside; arrows leaving the map are lost.
	if (!(currPos.getX() >= 0 && currPos.getX() < board->getMap()->getSizeX() &&
		currPos.getY() >= 0 && currPos.getY() < board->getMap()->getSizeY())) {
		isReadyToBeRemoved = true;
		return;
	}

	auto hitResult = board->calculateHit(LineSegment(prevPos, currPos), entityToIgnore.lock());
	if (!hitResult.has_value()) {
		return;
	}
	if (hitResult->character.has_value()) {
		onCharacterHit(hitResult->character.value(), board);
		return;
	}
	if (hitResult->mapHit.has_value()) {
		if (hitResult->mapHit->getZ() <= 0.) {
			onGroundHit(hitResult->mapHit.value(), board);
		}
		else {
			onWallHit(hitResult->mapHit.value(), board);
		}
	}
}

void ArrowProjectile::updateDrawables()
{
	shadowDrawable->setPos(flightPath->getPosition().grounded());
	drawable->setPos(flightPath->getPosition());
}

double ArrowProjectile::getDamage() const
{
	return hit.damage;
}

const HitInfo& ArrowProjectile::getHit() const
{
	return hit;
}

shared_ptr<Arrow> ArrowProjectile::getArrow() const
{
	return arrow;
}

bool ArrowProjectile::isBroken() const
{
	return broken;
}
