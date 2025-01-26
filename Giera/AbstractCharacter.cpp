#include "AbstractCharacter.h"
#include "Board.h"
#include "Calculator.h"
#include "Cuboid.h"
#include "CharacterObserver.h"
#include "HpBarDrawable.h"
#include "Damage.h"
#include <algorithm>
#include <memory>
#include <AbstractWeapon.h>
#include <BaseItemHandler.h>

AbstractCharacter::AbstractCharacter()
{
	resitances.resize((int)DamageTypes::COUNT, 1);
	hpBarDrawable = make_shared<HpBarDrawable>(Position(position.getX(), position.getY(), position.getZ() + height + .1), this);
	drawables.push_back(hpBarDrawable);
}


string AbstractCharacter::getTextureFilePath()
{
	stringstream ss;
	ss << (int)characterType;
	return string(SAVE_FILES_PATH) +"/tx/npc/npc" + ss.str() + ".png";
}

string AbstractCharacter::getShadowFilePath()
{
	return string(SAVE_FILES_PATH) +"/tx/shadows/shadow_medium.png";
}

Position AbstractCharacter::getPosition() const
{
	return position;
}

void AbstractCharacter::updateDrawables()
{
	drawable->setPos(position);
	shadow_drawable->setPos(Position(position.getX(), position.getY(), -.01));
	hpBarDrawable->setPos(Position(position.getX(), position.getY(), position.getZ() + height + .1));
	if (shieldDrawable) {
		shieldDrawable->setPos(getShieldPos());
	}	
}

void AbstractCharacter::updateHitboxes()
{
	hitbox->setFigure(make_unique<Cuboid>(
		Position(position.getX() - sizeXY.first / 2, position.getY() - sizeXY.second / 2, 0),
		Position(position.getX() + sizeXY.first / 2, position.getY() + sizeXY.second / 2, height)));
	//Logger::logInfo("hitbox updated", hitbox->getFigure().get()->getBoundingBox().first,
	//	hitbox->getFigure().get()->getBoundingBox().second);
}

void AbstractCharacter::addCharacterObserver(weak_ptr<CharacterObserver> observer)
{
	characterObservers.push_back(observer);
	observer.lock()->notifyCharacterObservers(shared_from_this(), CharacterObserver::Change::ADDED);
}

void AbstractCharacter::setBoard(weak_ptr<Board> board)
{
	this->board = board;
}

void AbstractCharacter::notifyCharacterObservers(CharacterObserver::Change change)
{
	for (auto obs : characterObservers) {
		auto obsSp = obs.lock();
		if (obsSp) {
			obsSp->notifyCharacterObservers(shared_from_this(), change);
		}
	}
}

shared_ptr<Inventory> AbstractCharacter::getInventory() const
{
	return inventory;
}

shared_ptr<AbstractWeapon> AbstractCharacter::getSelectedWeapon() const
{
	//TODO probably AbstractCharacter should just have a random weapon
	return BaseItemHandler::generate<MeleeWeapon>(ItemTypes::MELEE_WEAPON, 0); 
}

character_hp_t* AbstractCharacter::getHpPtr()
{
	return &hp;
}

character_hp_t* AbstractCharacter::getMaxHpPtr()
{
	return &maxHp;
}

void AbstractCharacter::startAttack(Position target)
{
	if (!canAttack()) {
		Logger::logInfo("cannot attack");
		return;
	}
	attackInfo = AttackInfo();
	notifyDrawableObservers(DrawableEntityObserver::Change::REMOVED);
	Position attackPosition = (target - position) * (1. / (target - position).getNorm()) + position;
	attackInfo->attackLine = LineSegment(position, attackPosition);
	Logger::logInfo("attack started", target, position);
	int attackShadowSize = 100;
	auto attackShadowTexture = TextureLoader::makeUniColorTexture(attackShadowSize, attackShadowSize, { 0,0,0,0 });
	auto prev_target = SDL_GetRenderTarget(Texture::getRenderer());
	SDL_SetRenderDrawColor(Texture::getRenderer(), 0, 0, 0, 196);
	SDL_SetRenderTarget(Texture::getRenderer(), attackShadowTexture->getSDLTexture());
	SDL_RenderDrawLine(Texture::getRenderer(), attackShadowSize / 2, attackShadowSize / 2,
		attackShadowSize / 2 + attackShadowSize / 2 * (attackPosition - position).getX(),
		attackShadowSize / 2 + attackShadowSize / 2 * (attackPosition - position).getY());
	SDL_SetRenderTarget(Texture::getRenderer(), prev_target);
	attackInfo->attackShadowDrawable =
		make_shared<Drawable>(position, attackShadowTexture, Drawable::DrawableLayer::SHADOWS, make_pair(2, 2), 0.);
	drawables.push_back(attackInfo->attackShadowDrawable);
	attackInfo->timeToAttack = Time(300);
	attackInfo->cooldownAfterAttack = Time(700);
	notifyDrawableObservers(DrawableEntityObserver::Change::ADDED);

}

void AbstractCharacter::updateAttack(Time timeDiff)
{
	if (!attackInfo.has_value()) 
	{
		//no ongoing attack - nothing to update
		return;
	}
	//Logger::logInfo("updateAttack", attackInfo->timeToAttack, attackInfo->cooldownAfterAttack);
	cancelParry();
	attackInfo->timeToAttack -= timeDiff;
	if (attackInfo->timeToAttack.getTimeMs() > 0) {
		return;
	}
	if (!attackInfo->hasStruct) {
		attackInfo->hasStruct = true;
		auto lineStart = attackInfo->attackLine.getStart();
		auto lineEnd = attackInfo->attackLine.getEnd();
		lineStart.setZ(.1);
		lineEnd.setZ(.1);
		attackInfo->attackLine = LineSegment(lineStart, lineEnd);
		auto hitResult = board.lock()->calculateHit(attackInfo->attackLine, shared_from_this());
		Logger::logInfo("attackLine: ", attackInfo->attackLine.getStart(), attackInfo->attackLine.getEnd());
		Logger::logInfo("attack calculated", hitResult.has_value());
		if (hitResult.has_value()) {
			Logger::logInfo("attack hit", hitResult.value().character.has_value(), hitResult.value().mapHit.has_value());
		}
		if (hitResult.has_value() && hitResult.value().character.has_value()) {
			auto enemy = hitResult.value().character.value();
			
			//we need to deal dmg in int so we use calculator
			auto dealtDamage = Calculator::getIntFromDoubleWithProb(
				std::max(0., getSelectedWeapon()->getDamage()->getValue() - enemy->getTotalArmor())
			); 
			Logger::logInfo("updateAttack: dealing damage: ", dealtDamage,
				"(calc from dmg: ", getSelectedWeapon()->getDamage()->getValue(), " - arm: ", enemy->getTotalArmor(), ")");
			(*enemy->getHpPtr()) -= dealtDamage;

			if ((*enemy->getHpPtr()) <= 0) {
				Logger::logInfo("killed");
				enemy->die();
			}
			Logger::logInfo("attack hit");
		}
	}
	else {
		attackInfo->cooldownAfterAttack -= timeDiff;
		if (attackInfo->cooldownAfterAttack.getTimeMs() <= 0) {
			notifyDrawableObservers(DrawableEntityObserver::Change::REMOVED);
			drawables.erase((std::find(drawables.begin(), drawables.end(), attackInfo->attackShadowDrawable)));
			notifyDrawableObservers(DrawableEntityObserver::Change::ADDED);
			attackInfo.reset();
		}
	}
}

bool AbstractCharacter::canMove() const {
	return !attackInfo.has_value() && !isStunned;
}

bool AbstractCharacter::canAttack() const {
	return !attackInfo.has_value() && !isStunned;
}

shared_ptr<Shield> AbstractCharacter::getSelectedShield() const {
	return nullptr;
}

Position AbstractCharacter::getShieldPos() const {
	return Position(position.getX() + 0.1, position.getY() + 0.5, position.getZ() + 0.6);
}

double AbstractCharacter::getTotalArmor() const {
	// TODO armor part
	// TODO maybe without the shield you should also be able to parry (eg. with the sword)
	auto shield = getSelectedShield();
	return (shield ? shield->getArmor() : 0.) * parryCompleteness; 
}

void AbstractCharacter::showShieldDrawable() {
	if (!getSelectedShield()) {
		Logger::logDebug("No shield to show");
		return;
	}
	auto shieldTx = shared_ptr(getSelectedShield()->getTexture());
	if (!shieldTx) {
		return;
	}
	shieldDrawable = make_shared<Drawable>(getShieldPos(),
		shieldTx,
		Drawable::DrawableLayer::ENTITIES,
		make_pair(0.67, 0.4),
		0.35);
	drawables.push_back(shieldDrawable);
	notifyDrawableObservers(DrawableEntityObserver::Change::ADDED);
	Logger::logDebug("shieldDrawable set: ", shieldDrawable);
}

void AbstractCharacter::parry(Time timeDiff) {
	if (attackInfo.has_value()) {
		Logger::logDebug("Parry not initialized because of ongoing attack");
		return;
	}
	auto prevParryCompleteness = parryCompleteness;
	parryCompleteness = std::min(1.0, parryCompleteness + (timeDiff / timeToParry));

	if (prevParryCompleteness == 0.) {
		Logger::logDebug("parry started with timeDiff: ", timeDiff, "parryCompl: ", parryCompleteness);
		return;
	}
	if (!(prevParryCompleteness != 1. && parryCompleteness == 1.)) {
		return; //already completed parry and handled displaying the shield
	}

	Logger::logDebug("parry completed (1.) with timeDiff: ", timeDiff, "armor: ", getTotalArmor());
	showShieldDrawable();
}

void AbstractCharacter::removeShieldDrawable() {
	if (!shieldDrawable) {
		return; //no drawable to remove
	}
	notifyDrawableObservers(DrawableEntityObserver::Change::REMOVED);
	std::vector<shared_ptr<Drawable>>::iterator endAfterRemove =
		std::remove(drawables.begin(), drawables.end(), shieldDrawable);
	if (endAfterRemove != drawables.end()) {
		drawables.erase(endAfterRemove, drawables.end());
	}
	shieldDrawable = nullptr;
}

void AbstractCharacter::cancelParry() {
	if (!parryCompleteness) {
		return; //parry already cancelled
	}
	Logger::logDebug("Parry cancelled");
	parryCompleteness = 0;
	removeShieldDrawable();
}

void AbstractCharacter::die() {
	board.lock()->getBoardTile(Coordinates(getPosition())).removeCharacter(shared_from_this());
	notifyCharacterObservers(CharacterObserver::Change::REMOVED);
	notifyHittableObservers(HittableEntityObserver::Change::REMOVED);
	notifyDrawableObservers(DrawableEntityObserver::Change::REMOVED);
}

void AbstractCharacter::generateShadowTexture()
{
	using std::min;
	using std::max;
	Position shadowLowerLeft = position;
	Position shadowUpperRight = position;
	for (auto hitbox : hitboxes) {
		shadowLowerLeft.setX(min(shadowLowerLeft.getX(), hitbox->getFigure()->getBoundingBox().first.getX()));
		shadowLowerLeft.setY(min(shadowLowerLeft.getY(), hitbox->getFigure()->getBoundingBox().first.getY()));
		shadowUpperRight.setX(max(shadowUpperRight.getX(), hitbox->getFigure()->getBoundingBox().second.getX()));
		shadowUpperRight.setY(max(shadowUpperRight.getY(), hitbox->getFigure()->getBoundingBox().second.getY()));
	}
	shadow_drawable = make_shared<Drawable>(Position((shadowLowerLeft.getX() + shadowUpperRight.getX()) / 2,
		(shadowLowerLeft.getY() + shadowUpperRight.getY()) / 2, 0),
		TextureLoader::getTexturePtr(getShadowFilePath()),
		Drawable::DrawableLayer::SHADOWS, make_pair(shadowUpperRight.getX() - shadowLowerLeft.getX(),
			shadowUpperRight.getY() - shadowLowerLeft.getY()), 0);
	drawables.push_back(shadow_drawable);
}

void AbstractCharacter::move(Position moveDifference)
{
	auto board_sh = board.lock();
	if (board_sh == nullptr) {
		throw "board is null";
	}
	if (!canMove()) {
		//Logger::logInfo("cannot move");
		return;
	}
	if (board_sh->isStepablePosition(position + moveDifference - Position(sizeXY.first / 2, 0, 0)) &&
		board_sh->isStepablePosition(position + moveDifference + Position(sizeXY.first / 2, 0, 0))) {
		notifyDrawableObservers(DrawableEntityObserver::Change::REMOVED);
		notifyHittableObservers(HittableEntityObserver::Change::REMOVED);
		notifyCharacterObservers(CharacterObserver::Change::BEFORE_MOVE);
		position += moveDifference;
		updateDrawables();
		updateHitboxes();
		notifyDrawableObservers(DrawableEntityObserver::Change::ADDED);
		notifyHittableObservers(HittableEntityObserver::Change::ADDED);
		notifyCharacterObservers(CharacterObserver::Change::AFTER_MOVE);
	}
}

