#pragma once
#include "LineSegment.h"
#include "Drawable.h"
#include <optional>

class AttackInfo
{
public:
	AttackInfo() = default;
	AttackInfo(Time timeToAttack, Time cooldownAfterAttack, LineSegment attackLine, shared_ptr<Drawable> attackShadowDrawable, shared_ptr<Drawable> attackDrawable);

	LineSegment attackLine;
	shared_ptr<Drawable> attackShadowDrawable;
	shared_ptr<Drawable> attackDrawable; //unused for now
	Time timeToAttack; // time until attack will hit
	Time cooldownAfterAttack; // time after hitting until able to attack again or move
	bool hasHit = false; // true if already checked if attack hit
};