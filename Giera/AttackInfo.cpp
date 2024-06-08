#include "AttackInfo.h"

AttackInfo::AttackInfo(Time timeToAttack, Time cooldownAfterAttack, LineSegment attackLine, shared_ptr<Drawable> attackShadowDrawable, shared_ptr<Drawable> attackDrawable)
	: timeToAttack(timeToAttack), cooldownAfterAttack(cooldownAfterAttack), attackLine(attackLine), attackShadowDrawable(attackShadowDrawable), attackDrawable(attackDrawable)
{
}