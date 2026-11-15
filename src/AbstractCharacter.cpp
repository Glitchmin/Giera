#include "AbstractCharacter.h"
#include "Board.h"
#include "Calculator.h"
#include "Cuboid.h"
#include "CharacterObserver.h"
#include "HpBarDrawable.h"
#include "Damage.h"
#include <algorithm>
#include <cmath>
#include <memory>
#include "AbstractWeapon.h"
#include "ArrowProjectile.h"
#include "CombatRules.h"
#include "NormalItems.h"

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
	// (Used to generate a brand new random sword on every call.)
	return nullptr; // bare hands
}

shared_ptr<RangedWeapon> AbstractCharacter::getSelectedBow() const
{
	return nullptr;
}

shared_ptr<Arrow> AbstractCharacter::peekArrow() const
{
	return nullptr;
}

shared_ptr<Arrow> AbstractCharacter::takeArrow()
{
	return nullptr;
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
 * @brief Attack button pressed.
 *
 * In bow mode with a bow and an arrow this starts drawing the bow (the arrow flies on
 * releaseAttack()); otherwise it is a melee swing - bare-handed if no weapon is equipped.
 * (The old "ranged" mode cast a spell projectile without needing a bow.)
 *
 * @param target The position to target for the attack.
 */
void AbstractCharacter::startAttack(Position target)
{
	if (!canAttack()) {
		Logger::logDebug("cannot attack");
		return;
	}
	if (!meleeMode && canShoot()) {
		startDrawing(target);
	}
	else {
		startMelee(target);
	}
}

bool AbstractCharacter::canShoot() const
{
	return getSelectedBow() != nullptr && peekArrow() != nullptr;
}

void AbstractCharacter::startDrawing(Position target)
{
	if (!canAttack() || !canShoot()) {
		return;
	}
	cancelParry(); // both hands are needed for the bow
	bowDraw = BowDrawInfo{ 0., target };
}

void AbstractCharacter::aimAt(Position target)
{
	if (bowDraw.has_value()) {
		bowDraw->target = target;
	}
}

void AbstractCharacter::cancelDrawing()
{
	bowDraw.reset();
	clearAimLine(!dead);
}

/// Where an arrow released right now would land (on the ground), following the same
/// flight path as the real arrow: a weak draw falls short, and a wall stops the line.
std::vector<Position> AbstractCharacter::getArrowTrajectory() const
{
	std::vector<Position> points;
	auto bow = getSelectedBow();
	auto boardSp = board.lock();
	if (!bowDraw.has_value() || !bow || !boardSp) {
		return points;
	}
	double speed = bow->getArrowVelocity() * bowDraw->progress;
	if (!(speed > 0.)) {
		return points;
	}
	Position start = position + Position(0, 0, CombatRules::get().arrowSpawnHeight);
	FlightPath path(start, bowDraw->target, 1, speed);
	points.push_back(start);
	// the shooter is ignored, like the real arrow does
	auto self = std::const_pointer_cast<AbstractCharacter>(shared_from_this());
	for (int i = 0; i < 600; i++) {
		Position pos = path.updatePosition(Time(10));
		if (!std::isfinite(pos.getX()) || !std::isfinite(pos.getY()) || !std::isfinite(pos.getZ())) {
			break;
		}
		if (pos.getZ() < 1.0 && !boardSp->isStepablePosition(pos.grounded())) {
			break; // a wall (or the map edge) stops the arrow
		}
		// the first character in the way stops it too
		auto hit = boardSp->calculateHit(LineSegment(points.back(), pos), self);
		if (hit.has_value() && hit->character.has_value()) {
			// the arc ends where it meets the character (the ground line ends under that point)
			Position prev = points.back();
			points.push_back(Position((prev.getX() + pos.getX()) / 2, (prev.getY() + pos.getY()) / 2,
				std::max(0., (prev.getZ() + pos.getZ()) / 2)));
			break;
		}
		if (pos.getZ() <= 0.) {
			points.push_back(Position(pos.getX(), pos.getY(), 0.));
			break;
		}
		points.push_back(pos);
	}
	return points;
}

std::optional<Position> AbstractCharacter::getArrowLandingPoint() const
{
	auto points = getArrowTrajectory();
	if (points.empty()) {
		return std::nullopt;
	}
	return points.back().grounded();
}

bool AbstractCharacter::canReleaseArrow() const
{
	return bowDraw.has_value() && bowDraw->progress >= CombatRules::get().minDrawToShoot;
}

size_t AbstractCharacter::getAimDotCount() const
{
	return aimDots.size();
}

void AbstractCharacter::clearAimLine(bool notify)
{
	if (!aimLineDrawable && aimDots.empty()) {
		return;
	}
	if (notify) {
		notifyDrawableObservers(DrawableEntityObserver::Change::REMOVED);
	}
	drawables.erase(std::remove_if(drawables.begin(), drawables.end(), [this](const shared_ptr<Drawable>& d) {
		return d == aimLineDrawable || std::find(aimDots.begin(), aimDots.end(), d) != aimDots.end();
		}), drawables.end());
	aimLineDrawable = nullptr;
	aimDots.clear();
	if (notify) {
		notifyDrawableObservers(DrawableEntityObserver::Change::ADDED);
	}
}

/**
 * Aiming aid while the bow is drawn:
 *  - a dotted arc along the arrow's real flight (Angry Birds style), green once releasing
 *    would shoot (draw >= minDrawToShoot), red before that;
 *  - a dark line on the ground from the character to where the arrow would land.
 */
void AbstractCharacter::updateAimLine()
{
	auto points = getArrowTrajectory();
	if (points.size() < 2 || dead) {
		clearAimLine(!dead);
		return;
	}
	Position from = position.grounded();
	Position diff = points.back().grounded() - from;
	double length = diff.getNorm();

	// REMOVED before changing the list, ADDED after (the renderer is a multimap).
	notifyDrawableObservers(DrawableEntityObserver::Change::REMOVED);
	drawables.erase(std::remove_if(drawables.begin(), drawables.end(), [this](const shared_ptr<Drawable>& d) {
		return d == aimLineDrawable || std::find(aimDots.begin(), aimDots.end(), d) != aimDots.end();
		}), drawables.end());
	aimLineDrawable = nullptr;
	aimDots.clear();

	if (!aimLineTexture) {
		aimLineTexture = TextureLoader::makeUniColorTexture(4, 4, { 0, 0, 0, 140 });
		aimDotReadyTexture = TextureLoader::makeCircleTexture(24, { 90, 230, 90, 235 });
		aimDotNotReadyTexture = TextureLoader::makeCircleTexture(24, { 230, 60, 50, 235 });
	}

	// ground track: a thin strip centred between the two points, rotated along the line
	if (length > 0.05) {
		aimLineDrawable = make_shared<Drawable>(from + diff * 0.5, aimLineTexture, Drawable::DrawableLayer::SHADOWS,
			make_pair(0.04, length), 0.);
		aimLineDrawable->setAngle((float)(std::atan2(diff.getX(), -diff.getY()) * 57.2957795130823));
		drawables.push_back(aimLineDrawable);
	}

	// dots every dotSpacing metres along the arc, shrinking towards the end
	const double dotSpacing = 0.45;
	const double firstDot = 0.5; // keep the first dot off the character
	auto texture = canReleaseArrow() ? aimDotReadyTexture : aimDotNotReadyTexture;
	double travelled = 0., nextDot = firstDot;
	double total = 0.;
	for (size_t i = 1; i < points.size(); i++) {
		total += (points[i] - points[i - 1]).getNorm();
	}
	for (size_t i = 1; i < points.size() && aimDots.size() < 60; i++) {
		double step = (points[i] - points[i - 1]).getNorm();
		while (step > 0. && travelled + step >= nextDot) {
			double t = (nextDot - travelled) / step;
			Position dotPos = points[i - 1] + (points[i] - points[i - 1]) * t;
			double size = 0.16 - 0.08 * std::min(1., nextDot / std::max(total, 0.01));
			auto dot = make_shared<Drawable>(dotPos, texture, Drawable::DrawableLayer::ENTITIES,
				make_pair(size, size), 0.);
			aimDots.push_back(dot);
			drawables.push_back(dot);
			nextDot += dotSpacing;
		}
		travelled += step;
	}
	notifyDrawableObservers(DrawableEntityObserver::Change::ADDED);
}

bool AbstractCharacter::isDrawing() const
{
	return bowDraw.has_value();
}

double AbstractCharacter::getDrawProgress() const
{
	return bowDraw.has_value() ? bowDraw->progress : 0.;
}

bool AbstractCharacter::isAttacking() const
{
	return meleeAttackInfo.has_value() || bowDraw.has_value();
}

/**
 * @brief Shoots the drawn bow towards @p target.
 *
 * Arrow speed and damage scale with the draw progress:
 *   speed  = bow arrow speed * progress
 *   damage = (bow damage + arrow damage) * progress
 * Releasing before CombatRules::minDrawToShoot lowers the bow and keeps the arrow.
 */
void AbstractCharacter::releaseAttack(Position target)
{
	if (!bowDraw.has_value()) {
		return;
	}
	double progress = bowDraw->progress;
	bowDraw.reset();
	clearAimLine(!dead);
	const auto& rules = CombatRules::get();
	if (dead || progress < rules.minDrawToShoot) {
		Logger::logInfo("bow released too early, no shot (draw", progress, ")");
		return;
	}
	auto boardSp = board.lock();
	auto bow = getSelectedBow();
	if (!boardSp || !bow || !peekArrow()) {
		return;
	}
	auto arrow = takeArrow();
	double speed = bow->getArrowVelocity() * progress;
	HitInfo hit = makeArrowHit(bow, arrow, progress);
	Position start = position + Position(0, 0, rules.arrowSpawnHeight);
	Logger::logInfo("arrow shot: draw", progress, "speed", speed, "damage", hit.damage, hit.critical ? "CRIT" : "");
	boardSp->addProjectile(make_shared<ArrowProjectile>(
		make_shared<FlightPath>(start, target, 1, speed),
		arrow, hit,
		std::enable_shared_from_this<AbstractCharacter>::weak_from_this()));
}

bool AbstractCharacter::setMeleeMode(bool melee)
{
	if (!melee && !getSelectedBow()) {
		Logger::logInfo("no bow equipped");
		melee = true;
	}
	if (melee != meleeMode) {
		cancelDrawing();
	}
	meleeMode = melee;
	return meleeMode;
}

bool AbstractCharacter::toggleWeaponMode()
{
	bool melee = setMeleeMode(!meleeMode);
	Logger::logInfo("weapon mode:", melee ? "melee" : "bow");
	return melee;
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
	Position direction = (target - position).grounded();
	if (direction.getNorm() < 1e-6) {
		// Clicking exactly on the character gave a zero-length direction (NaN attack line).
		direction = Position(1, 0, 0);
	}
	const auto& rules = CombatRules::get();
	cancelParry();
	meleeAttackInfo = MeleeAttackInfo();
	notifyDrawableObservers(DrawableEntityObserver::Change::REMOVED);
	Position attackPosition = direction * (rules.meleeReach / direction.getNorm()) + position;
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
	auto timings = getMeleeTimings();
	meleeAttackInfo->timeToAttack = timings.first;
	meleeAttackInfo->cooldownAfterAttack = timings.second;
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
	bool moved = movedSinceLastUpdate;
	movedSinceLastUpdate = false;
	const auto& rules = CombatRules::get();

	staggerLeft -= timeDiff;
	parryCooldownLeft -= timeDiff;
	dashCooldownLeft -= timeDiff;
	if (parryWindowLeft.getTimeMs() > 0) {
		parryWindowLeft -= timeDiff;
		if (parryWindowLeft.getTimeMs() == 0) {
			// the window closed without a hit: a missed parry can't be retried at once
			parryCooldownLeft = Time(rules.parryMissCooldownMs);
		}
	}
	if (dashLeft.getTimeMs() > 0) {
		Time step = timeDiff < dashLeft ? timeDiff : dashLeft;
		double distance = rules.dashDistance * step.getTimeMs() / std::max(1u, rules.dashDurationMs);
		// small steps, so a long frame can't jump over a wall tile
		int steps = std::max(1, (int)std::ceil(distance / 0.2));
		for (int i = 0; i < steps; i++) {
			if (!performMove(dashDirection * (distance / steps))) {
				dashLeft = Time(0); // hit a wall: the dash ends there
				break;
			}
		}
		dashLeft -= timeDiff;
	}
	updateEffects(timeDiff);
	if (dead) {
		return;
	}

	if (bowDraw.has_value()) {
		auto bow = getSelectedBow();
		if (dead || !bow || !peekArrow()) {
			cancelDrawing(); // bow unequipped / quiver emptied while drawing
		}
		else {
			double rate = (double)timeDiff.getTimeMs() / std::max(1u, getDrawTime().getTimeMs());
			if (moved) {
				rate *= CombatRules::get().actionSpeedWhileMoving;
			}
			bowDraw->progress = std::min(1.0, bowDraw->progress + rate);
		}
	}
	if (bowDraw.has_value()) {
		updateAimLine(); // follows the cursor / joystick and the draw strength
	}
	// Always finish an ongoing melee swing, even if the weapon mode was switched
	// mid-swing (before, the character stayed frozen forever in that case).
	if (meleeAttackInfo.has_value()) {
		bool retFlag;
		updateMelee(timeDiff, retFlag);
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
			// Last use of meleeAttackInfo in this call: the target may stagger us (shield
			// block / riposte), which ends this swing.
			HitInfo hit = makeMeleeHit();
			auto outcome = enemy->receiveHit(hit);
			Logger::logInfo("melee hit:", hit.damage, hit.critical ? "CRIT" : "", "-> dealt", outcome.dealt,
				"(armor", (int)(outcome.reduction * 100), "%)", outcome.parried ? "PARRIED" : "",
				outcome.shieldBlocked ? "shield block" : "", outcome.killed ? "killed" : "");
		}
		retFlag = false;
		return;
	}
	else {
		meleeAttackInfo->cooldownAfterAttack -= timeDiff;
		if (meleeAttackInfo->cooldownAfterAttack.getTimeMs() <= 0) {
			endMeleeSwing();
		}
	}
	retFlag = false;
}

void AbstractCharacter::endMeleeSwing()
{
	if (!meleeAttackInfo.has_value()) {
		return;
	}
	notifyDrawableObservers(DrawableEntityObserver::Change::REMOVED);
	drawables.erase(std::remove(drawables.begin(), drawables.end(), meleeAttackInfo->attackShadowDrawable), drawables.end());
	meleeAttackInfo.reset();
	if (!dead) {
		notifyDrawableObservers(DrawableEntityObserver::Change::ADDED);
	}
}

/**
 * @brief Checks if the character can move.
 *
 * A character cannot move if it is stunned or performing a melee attack.
 *
 * @return True if the character can move, false otherwise.
 */
bool AbstractCharacter::canMove() const {
	return !dead && !meleeAttackInfo.has_value() && !isStaggered() && !isDashing();
}

void AbstractCharacter::addStuckArrow(shared_ptr<Arrow> arrow) {
	if (arrow) {
		stuckArrows.push_back(arrow);
	}
}

const vector<shared_ptr<Arrow>>& AbstractCharacter::getStuckArrows() const {
	return stuckArrows;
}

void AbstractCharacter::dropStuckArrows() {
	auto boardSp = board.lock();
	if (boardSp) {
		for (auto& arrow : stuckArrows) {
			// spread them a little around the body
			Position offset(Calculator::getRandomDouble(-.3, .3), Calculator::getRandomDouble(-.3, .3), 0);
			Position spot = position + offset;
			boardSp->dropItem(boardSp->isStepablePosition(spot) ? spot : position, arrow);
		}
	}
	stuckArrows.clear();
}

bool AbstractCharacter::takeDamage(character_hp_t damage) {
	if (dead) {
		return false;
	}
	hp = damage >= hp ? 0 : hp - damage;
	if (hp == 0) {
		die();
		return true;
	}
	return false;
}

bool AbstractCharacter::isDead() const {
	return dead;
}

character_hp_t AbstractCharacter::getHp() const {
	return hp;
}

character_hp_t AbstractCharacter::getMaxHp() const {
	return maxHp;
}

double AbstractCharacter::getParryCompleteness() const {
	return parryCompleteness;
}

/**
 * @brief Checks if the character can attack.
 *
 * A character cannot attack if it is stunned or performing a melee attack.
 *
 * @return True if the character can attack, false otherwise.
 */
bool AbstractCharacter::canAttack() const {
	return !dead && !meleeAttackInfo.has_value() && !bowDraw.has_value() && !isStaggered() && !isDashing()
		&& !isOccupied();
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
	// REMOVED before changing the list and ADDED after, otherwise the renderer
	// (a multimap) ends up with duplicate "ghost" copies of every drawable.
	notifyDrawableObservers(DrawableEntityObserver::Change::REMOVED);
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
	if (dead) {
		return;
	}
	if (meleeAttackInfo.has_value() || isStaggered() || isDashing()) {
		Logger::logDebug("Parry not initialized because of ongoing attack");
		return;
	}
	bool justPressed = !parryHeld;
	parryHeld = true;
	auto shield = getSelectedShield();
	if (!shield) {
		// No shield: a timed parry. Only the press counts - holding the button does nothing.
		if (justPressed && parryCooldownLeft.getTimeMs() == 0 && parryWindowLeft.getTimeMs() == 0) {
			cancelDrawing();
			parryWindowLeft = Time(CombatRules::get().parryWindowMs);
			Logger::logDebug("parry window open");
		}
		return;
	}
	cancelDrawing(); // raising the shield lowers the bow
	double rate = (double)timeDiff.getTimeMs() / std::max(1u, shield->getTimeToRaise().getTimeMs());
	if (movedSinceLastUpdate) {
		rate *= CombatRules::get().actionSpeedWhileMoving;
	}
	auto prevParryCompleteness = parryCompleteness;
	parryCompleteness = std::min(1.0, parryCompleteness + rate);

	Logger::logDebug("parry started with timeDiff: ", timeDiff, "parryCompl: ", parryCompleteness);
	if (!(prevParryCompleteness != 1. && parryCompleteness == 1.)) {
		return; // Not fully raised yet, or already raised and shown.
	}

	Logger::logDebug("shield fully raised, armor", getArmorAgainst(DamageTypes::SLASHING));
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
	if (!dead) {
		// Re-register the remaining drawables; without this the character
		// vanished after releasing the shield until it moved again.
		notifyDrawableObservers(DrawableEntityObserver::Change::ADDED);
	}
}

/**
 * @brief Cancels the ongoing parry action.
 *
 * Resets parry completeness to 0 and removes the shield drawable if it exists.
 * Logs the cancellation of the parry.
 */
void AbstractCharacter::cancelParry() {
	parryHeld = false;
	if (!parryCompleteness) {
		return; // Parry already cancelled.
	}
	Logger::logDebug("Parry cancelled");
	parryCompleteness = 0;
	removeShieldDrawable();
}

void AbstractCharacter::die() {
	if (dead) {
		return; // die() used to run again on every further hit
	}
	dead = true;
	hp = 0;
	// Remove from the renderer first, while the drawables list still matches
	// what the renderer holds; then drop the temporary drawables.
	notifyDrawableObservers(DrawableEntityObserver::Change::REMOVED);
	parryCompleteness = 0;
	if (shieldDrawable) {
		drawables.erase(std::remove(drawables.begin(), drawables.end(), shieldDrawable), drawables.end());
		shieldDrawable = nullptr;
	}
	if (meleeAttackInfo.has_value()) {
		drawables.erase(std::remove(drawables.begin(), drawables.end(), meleeAttackInfo->attackShadowDrawable), drawables.end());
		meleeAttackInfo.reset();
	}
	bowDraw.reset();
	clearAimLine(false); // the renderer already dropped our drawables above
	parryHeld = false;
	parryWindowLeft = Time(0);
	parryCooldownLeft = Time(0);
	staggerLeft = Time(0);
	dashLeft = Time(0);
	activeEffects.clear();
	onInterrupted();
	dropStuckArrows(); // arrows that hit this character can be picked up from the body
	auto boardSp = board.lock();
	if (boardSp) {
		boardSp->getBoardTile(Coordinates(getPosition())).removeCharacter(shared_from_this());
	}
	notifyCharacterObservers(CharacterObserver::Change::REMOVED);
	notifyHittableObservers(HittableEntityObserver::Change::REMOVED);
}

void AbstractCharacter::revive(Position newPosition) {
	if (!dead) {
		return;
	}
	dead = false;
	hp = maxHp;
	staggerLeft = Time(0);
	dashLeft = Time(0);
	dashCooldownLeft = Time(0);
	position = newPosition;
	updateDrawables();
	updateHitboxes();
	notifyDrawableObservers(DrawableEntityObserver::Change::ADDED);
	notifyHittableObservers(HittableEntityObserver::Change::ADDED);
	notifyCharacterObservers(CharacterObserver::Change::ADDED);
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
	if (!std::isfinite(moveDifference.getX()) || !std::isfinite(moveDifference.getY()) || !std::isfinite(moveDifference.getZ())) {
		return;
	}
	if (!canMove()) {
		//Logger::logInfo("cannot move");
		return;
	}
	if (moveDifference.grounded().getNorm() > 1e-9) {
		lastMoveDirection = moveDifference.grounded() * (1. / moveDifference.grounded().getNorm());
	}
	moveDifference = moveDifference * getMoveSpeedMultiplier();
	performMove(moveDifference);
}

bool AbstractCharacter::performMove(Position moveDifference)
{
	auto board_sh = board.lock();
	if (board_sh == nullptr || dead || moveDifference.getNorm() == 0. ||
		!std::isfinite(moveDifference.getX()) || !std::isfinite(moveDifference.getY())) {
		return false;
	}
	if (board_sh->isStepablePosition(position + moveDifference - Position(sizeXY.first / 2, 0, 0)) &&
		board_sh->isStepablePosition(position + moveDifference + Position(sizeXY.first / 2, 0, 0))) {
		notifyDrawableObservers(DrawableEntityObserver::Change::REMOVED);
		notifyHittableObservers(HittableEntityObserver::Change::REMOVED);
		notifyCharacterObservers(CharacterObserver::Change::BEFORE_MOVE);
		position += moveDifference;
		movedSinceLastUpdate = true;
		updateDrawables();
		updateHitboxes();
		notifyDrawableObservers(DrawableEntityObserver::Change::ADDED);
		notifyHittableObservers(HittableEntityObserver::Change::ADDED);
		notifyCharacterObservers(CharacterObserver::Change::AFTER_MOVE);
		return true;
	}
	return false;
}

