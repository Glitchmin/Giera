#pragma once
#include "AbstractProjectile.h"
#include "Arrow.h"
#include "HitInfo.h"
#include <optional>

/**
 * @brief An arrow shot from a bow.
 *
 * Damage is decided when the arrow is released (bow + arrow damage, scaled by how far
 * the bow was drawn). On every impact the arrow may break (Arrow::getBreakChance()).
 * An arrow that survives either lies on the ground (ground / wall hit) or sticks in
 * the character it hit and drops when that character dies; both can be picked up.
 */
class ArrowProjectile : public AbstractProjectile
{
protected:
	shared_ptr<Arrow> arrow;
	HitInfo hit; ///< made when the bow was released (draw, modifiers, crit)
	Position lastPos;
	shared_ptr<Drawable> shadowDrawable;
	// What happens to the arrow item once the projectile is removed (main thread).
	std::optional<Position> dropPosition;
	weak_ptr<AbstractCharacter> stuckIn;
	bool broken = false;

	bool rollBreak();
	void updateRotation(Position diff);
public:
	ArrowProjectile(shared_ptr<FlightPath> flightPath, shared_ptr<Arrow> arrow, HitInfo hit,
		weak_ptr<HittableBoardEntity> entityToIgnore);

	virtual void onWallHit(Position hitPos, shared_ptr<Board>& board) override;
	virtual void onCharacterHit(shared_ptr<AbstractCharacter> character, shared_ptr<Board>& board) override;
	virtual void onGroundHit(Position hitPos, shared_ptr<Board>& board) override;
	virtual void move(Time& timeDiff, shared_ptr<Board>& board) override;
	virtual void onRemoved(shared_ptr<Board>& board) override;
	virtual void updateDrawables() override;

	double getDamage() const;
	const HitInfo& getHit() const;
	shared_ptr<Arrow> getArrow() const;
	bool isBroken() const;
};
