#pragma once
enum class ModifiersTypes {
	DAMAGE,
	ARMOR_PIERCING,
	ATTACK_SPEED,
	CRIT_CHANCE,
	CRIT_DAMAGE,
	MOVEMENT_SPEED,
	DODGE_SPEED,
	COUNT_MODIFIERS,
	VIEW_RANGE,
	SPELL_COOLDOWN,
	ARMOR,
	//...,
	COUNT_TOTAL = ARMOR //always set it to ("normal count"-1) cause of COUNT_MODIFIERS
};