#include "SpellProjectile.h"
#include "Drawable.h"

SpellProjectile::SpellProjectile(shared_ptr<FlightPath> flightPath, shared_ptr<ThrownSpell> spell)
	:AbstractProjectile(flightPath)
{
	this->spell = spell;
	drawable = make_shared<Drawable>(flightPath->getPosition(), 
		TextureLoader::makeUniColorTexture(20, 20, { 255,0,0,255 }),
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
	flightPath->updatePosition(timeDiff);
	notifyDrawableObservers(DrawableEntityObserver::Change::REMOVED);
	updateDrawables();
	notifyDrawableObservers(DrawableEntityObserver::Change::ADDED);
}

void SpellProjectile::updateDrawables()
{
	drawable->setPos(flightPath->getPosition());
}
