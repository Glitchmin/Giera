#include "SpellProjectile.h"

SpellProjectile::SpellProjectile(shared_ptr<AbstractFlightPath> flightPath, shared_ptr<ThrownSpell> spell, Position startPos)
	:AbstractProjectile(flightPath, startPos)
{
	this->spell = spell;
	drawable = make_shared<Drawable>(startPos, TextureLoader::makeUniColorTexture(20, 20, { 255,0,0,255 }),
		Drawable::DrawableLayer::ENTITIES, make_pair(.3, .3), 0.1);
	drawables.push_back(drawable);
}

void SpellProjectile::onWallHit(shared_ptr<MapTile> tile)
{
	Logger::logDebug("projectile hit the wall");
}

void SpellProjectile::onNPCHit(shared_ptr<AbstractNPC> npc)
{
	Logger::logDebug("projectile hit the NPC");
}

void SpellProjectile::onGroundHit(shared_ptr<MapTile> tile)
{
	isReadyToBeRemoved = true;
}

void SpellProjectile::move(Time& timeDiff)
{
	pos += flightPath->posDiff(timeDiff);
	notifyObservers(DrawableEntityObserver::Change::REMOVED);
	updateDrawables();
	notifyObservers(DrawableEntityObserver::Change::ADDED);
}

void SpellProjectile::updateDrawables()
{
	drawable->setPos(pos);	
}
