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
#include <SpellProjectile.h>

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

/**
 * @brief Starts a ranged shooting attack targeting a specified position.
 *
 * Continuously adds projectiles to the board's projectile list until the maximum of 5 projectiles
 * is reached. Each projectile follows a flight path from the character's position to the target.
 *
 * @param target The position to target for the shooting attack.
 */
void AbstractCharacter::startShoot(Position& target)
{
	while (board.lock()->getProjectiles().size() < 5) {
		board.lock()->addProjectile(make_shared<SpellProjectile>(
			make_shared<FlightPath>(
				position,
				target,
				1,
				2 * Calculator::getRandomInt(15, 17)
			),
			make_shared<ThrownSpell>(),
			std::enable_shared_from_this<AbstractCharacter>::weak_from_this()
		));
	}
}

/**
 * @brief Initiates an attack based on the current combat mode (melee or ranged).
 *
 * If the character cannot attack, logs a message and exits. Otherwise, the function starts
 * either a melee or ranged attack depending on the character's combat mode.
 *
 * @param target The position to target for the attack.
 */
void AbstractCharacter::startAttack(Position target)
{
	if (!canAttack()) {
		Logger::logInfo("cannot attack");
		return;
	}
	if (meleeMode) {
		startMelee(target);
	}
	else {
		startShoot(target);
	}
}

/**
 * @brief Starts a melee attack targeting a specified position.
 *
 * Sets up melee attack details such as attack line, visual effects, and cooldown timers.
 * Notifies observers of drawable changes and initializes the shadow texture for the attack.
 *
 * @param target The position to target for the melee attack.
 */
void AbstractCharacter::startMelee(Position& target) {
	meleeAttackInfo = MeleeAttackInfo();
	notifyDrawableObservers(DrawableEntityObserver::Change::REMOVED);
	Position attackPosition = (target - position) * (1. / (target - position).getNorm()) + position;
	meleeAttackInfo->attackLine = LineSegment(position, attackPosition);
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
	meleeAttackInfo->attackShadowDrawable =
		make_shared<Drawable>(position, attackShadowTexture, Drawable::DrawableLayer::SHADOWS, make_pair(2, 2), 0.);
	drawables.push_back(meleeAttackInfo->attackShadowDrawable);
	meleeAttackInfo->timeToAttack = Time(300);
	meleeAttackInfo->cooldownAfterAttack = Time(700);
	notifyDrawableObservers(DrawableEntityObserver::Change::ADDED);
}

/**
 * @brief Updates the ongoing attack based on the time difference.
 *
 * If the attack is in melee mode, updates the melee attack logic. If the attack
 * is ranged, no updates are currently implemented.
 *
 * @param timeDiff The time elapsed since the last update.
 */
void AbstractCharacter::updateAttack(Time timeDiff) {
	if (meleeMode) {
		bool retFlag;
		updateMelee(timeDiff, retFlag);
		if (retFlag) return;
	}
	else {
		// TODO update shoot
	}
}

/**
 * @brief Visualizes the ongoing melee attack by modifying the attack line.
 *
 * Adjusts the Z-axis positions of the attack line's start and end points, ensuring
 * the attack's visual representation is updated.
 */
void AbstractCharacter::visualiseMeleeAttack()
{
	auto lineStart = meleeAttackInfo->attackLine.getStart();
	auto lineEnd = meleeAttackInfo->attackLine.getEnd();
	lineStart.setZ(.1);
	lineEnd.setZ(.1);
	meleeAttackInfo->attackLine = LineSegment(lineStart, lineEnd);
	Logger::logInfo("attackLine: ", meleeAttackInfo->attackLine.getStart(), meleeAttackInfo->attackLine.getEnd());
}

/**
 * @brief Updates the state of a melee attack, including damage calculation and cooldown handling.
 *
 * Cancels parry mode, processes attack timing, calculates damage, and applies it to the target.
 * If the attack's cooldown has elapsed, cleans up attack visuals and resets attack state.
 *
 * @param timeDiff The time elapsed since the last update.
 * @param retFlag A flag indicating whether the update resulted in the attack ending.
 */
void AbstractCharacter::updateMelee(Time& timeDiff, bool& retFlag)
{
	retFlag = true;
	if (!meleeAttackInfo.has_value()) {
		// No ongoing attack - nothing to update
		return;
	}
	Logger::logDebug("updateAttack, cancel parry");
	cancelParry();
	meleeAttackInfo->timeToAttack -= timeDiff;
	if (meleeAttackInfo->timeToAttack.getTimeMs() > 0) {
		return;
	}
	if (!meleeAttackInfo->hasStruct) {
		meleeAttackInfo->hasStruct = true;
		visualiseMeleeAttack();
		auto hitResult = board.lock()->calculateHit(meleeAttackInfo->attackLine, shared_from_this());
		Logger::logInfo("attack calculated", hitResult.has_value());
		if (hitResult.has_value()) {
			Logger::logInfo("attack hit", hitResult.value().character.has_value(), hitResult.value().mapHit.has_value());
		}
		if (hitResult.has_value() && hitResult.value().character.has_value()) {
			auto enemy = hitResult.value().character.value();
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
		meleeAttackInfo->cooldownAfterAttack -= timeDiff;
		if (meleeAttackInfo->cooldownAfterAttack.getTimeMs() <= 0) {
			notifyDrawableObservers(DrawableEntityObserver::Change::REMOVED);
			drawables.erase((std::find(drawables.begin(), drawables.end(), meleeAttackInfo->attackShadowDrawable)));
			notifyDrawableObservers(DrawableEntityObserver::Change::ADDED);
			meleeAttackInfo.reset();
		}
	}
	retFlag = false;
}

/**
 * @brief Checks if the character can move.
 *
 * A character cannot move if it is stunned or performing a melee attack.
 *
 * @return True if the character can move, false otherwise.
 */
bool AbstractCharacter::canMove() const {
	return !meleeAttackInfo.has_value() && !isStunned;
}

/**
 * @brief Checks if the character can attack.
 *
 * A character cannot attack if it is stunned or performing a melee attack.
 *
 * @return True if the character can attack, false otherwise.
 */
bool AbstractCharacter::canAttack() const {
	return !meleeAttackInfo.has_value() && !isStunned;
}


/**
 * @brief Retrieves the currently selected shield.
 *
 * @return A shared pointer to the selected shield, or nullptr if no shield is selected.
 */
shared_ptr<Shield> AbstractCharacter::getSelectedShield() const {
	return nullptr;
}

/**
 * @brief Gets the position of the shield relative to the character.
 *
 * @return The position where the shield is displayed (relative to the character).
 */
Position AbstractCharacter::getShieldPos() const {
	return Position(position.getX() + 0.1, position.getY() + 0.5, position.getZ() + 0.6);
}

/**
 * @brief Calculates the total armor value of the character, including the shield if applicable.
 *
 * The armor value depends on whether a shield is selected and the current parry completeness.
 *
 * @return The total armor value of the character.
 */
double AbstractCharacter::getTotalArmor() const {
	// TODO body armor part
	// TODO: Consider allowing parry without a shield (e.g., with a sword).
	auto shield = getSelectedShield();
	return (shield ? shield->getArmor() : 0.) * parryCompleteness;
}

/**
 * @brief Displays the shield as a drawable entity on the screen.
 *
 * This function creates a drawable representation of the selected shield and adds it
 * to the list of drawables. If no shield is selected or no shield texture is available,
 * the function exits early.
 */
void AbstractCharacter::showShieldDrawable() {
	Logger::logDebug("showing shield!");
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

/**
 * @brief Handles the parry action, gradually increasing parry completeness over time.
 *
 * If an attack is ongoing, parry is not initiated. When parry completeness reaches 1.0,
 * the shield drawable is displayed. Logs debug information throughout the process.
 *
 * @param timeDiff The time elapsed since the last parry update.
 */
void AbstractCharacter::parry(Time timeDiff) {
	if (meleeAttackInfo.has_value()) {
		Logger::logDebug("Parry not initialized because of ongoing attack");
		return;
	}
	auto prevParryCompleteness = parryCompleteness;
	parryCompleteness = std::min(1.0, parryCompleteness + (timeDiff / timeToParry));

	Logger::logDebug("parry started with timeDiff: ", timeDiff, "parryCompl: ", parryCompleteness);
	if (prevParryCompleteness == 0.) {
		return;
	}
	if (!(prevParryCompleteness != 1. && parryCompleteness == 1.)) {
		return; // Already completed parry and handled displaying the shield.
	}

	Logger::logDebug("parry completed (1.) with timeDiff: ", timeDiff, "armor: ", getTotalArmor());
	showShieldDrawable();
}

/**
 * @brief Removes the shield drawable from the list of drawables.
 *
 * Notifies observers that the shield drawable has been removed and erases it
 * from the drawable list. Resets the shield drawable pointer to nullptr.
 */
void AbstractCharacter::removeShieldDrawable() {
	if (!shieldDrawable) {
		return; // No drawable to remove.
	}
	notifyDrawableObservers(DrawableEntityObserver::Change::REMOVED);
	std::vector<shared_ptr<Drawable>>::iterator endAfterRemove =
		std::remove(drawables.begin(), drawables.end(), shieldDrawable);
	if (endAfterRemove != drawables.end()) {
		drawables.erase(endAfterRemove, drawables.end());
	}
	shieldDrawable = nullptr;
}

/**
 * @brief Cancels the ongoing parry action.
 *
 * Resets parry completeness to 0 and removes the shield drawable if it exists.
 * Logs the cancellation of the parry.
 */
void AbstractCharacter::cancelParry() {
	if (!parryCompleteness) {
		return; // Parry already cancelled.
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

