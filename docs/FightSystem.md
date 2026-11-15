# Fight system rulebook

How fighting works in Giera, and where every number lives so you can change it.

There are two places to edit, and neither needs a recompile:

| What | File | Read |
|---|---|---|
| Rules of the fight system (armor formula, timings, parry window, dash, crits...) | [`assets/rules/Combat.txt`](../assets/rules/Combat.txt) | once at game start |
| Numbers of each item (damage, armor, swing / draw time, crit, penalties, break chance...) | [`assets/baseItems/*.csv`](../assets/baseItems), [`assets/effects/*.csv`](../assets/effects) | once at game start |

On Windows the build copies `assets/` next to the executable, so after editing either rebuild
(or copy the file to `build/windows-debug/bin/assets/...`). On Android the files are packed into the APK.

Items roll their numbers **once, when the item is created**, somewhere between the CSV's
`_MIN` and `_MAX`. Two swords of the same row can therefore hit for different amounts, but one
sword always hits for the same amount.

---

## 1. The damage pipeline

Every hit goes through these steps in order. Steps 1-4 happen at the attacker, 5-11 at the target.

| # | Step | Rule |
|---|---|---|
| 1 | **Base damage** | Melee: weapon `DMG`. Bare hands: `bareHandsDamage`. Bow: (bow `DMG` + arrow `DMG`) × draw. |
| 2 | **Damage modifiers** | × (1 + `DAMAGE` %). The % comes from worn gear plus the weapon used. Stat effects on damage also apply here. |
| 3 | **Crit roll** | Chance = `baseCritChancePct` + `CRIT_CHANCE` %. On a crit: × weapon `CRIT_DAMAGE` % (or `defaultCritDamagePct` if the weapon has 0). |
| 4 | **Armor piercing** | The weapon's `AP` (bow + arrow for arrows) + `ARMOR_PIERCING` modifiers. |
| 5 | **Dodge** | Target is dashing: nothing happens. |
| 6 | **Parry** | Melee hit, target has no shield and its parry window is open: deflected. The target ripostes (section 7). |
| 7 | **Armor** | Target armor vs this damage type (section 2), minus armor piercing (never below 0). |
| 8 | **Reduction** | `reduction = armor / (armor + armorK)`, then damage × (1 − reduction). |
| 9 | **Rounding** | Fractions round randomly: 4.6 becomes 5 in 60% of hits, 4 in 40%. |
| 10 | **Reactions** | Melee hit on a fully raised shield: the attacker is staggered. Any damage taken lowers a drawn bow. |
| 11 | **After the hit** | Lifesteal heals the attacker. On-hit effects (bleeding...) are applied to the target. |

### Worked example
The player's sword deals 20 slashing damage with AP 2.5. The enemy has 15 armor. 15 − 2.5 = 12.5 armor,
so the reduction is 12.5 / (12.5 + 50) = 20%, and **16 damage** gets through.

The other way round: the enemy's sword deals 16 with AP 5. The player has 20 body armor
(100% vs slashing) and a fully raised 50-armor shield: 70 − 5 = 65 armor, a 56% reduction, so
**7 damage**. With the shield down: 20 − 5 = 15 armor, a 23% reduction, so **12 damage**.

## 2. Armor

**Formula**: `reduction = armor / (armor + armorK)`, with `armorK` = 50.

| Armor | 0 | 25 | 50 | 75 | 100 | 150 | 200 |
|---|---|---|---|---|---|---|---|
| Reduction | 0% | 33% | 50% | 60% | 67% | 75% | 80% |

Each point of armor adds the same amount of *effective health* (+2% per point at K = 50). There is
no hard cap, but each point removes a smaller share of a hit than the last. Lowering `armorK` makes all
armor stronger.

**Where armor comes from** (added together):

- **Body armor**: `Armor.csv` `ARMOR` × effectiveness against the hit's damage type. The `EFF_SLASH`,
  `EFF_PIERCE` and `EFF_BLUNT` columns are 0-1. body_block: 100% vs slashing, 60% vs piercing,
  50% vs bludgeoning.
- **Shield**: `Shield.csv` `ARMOR` × how far it is raised (0-1). It counts fully against every
  physical type.
- **Enemies**: `enemyArmor` (15) from the rules file; enemies don't wear armor items yet.

**Damage types**: slashing, piercing and bludgeoning are *physical* and meet armor.
Elemental types (fire, frost, lightning...) ignore armor completely.
Melee weapons slash, arrows pierce, and bare hands bludgeon.

**Armor piercing** (`AP` columns, plus `ARMOR_PIERCING` modifiers) is a flat amount removed from the
target's armor before the formula. Penetrator arrows (AP 15-25) go straight through enemy armor.

## 3. Gear modifiers (`m0`-`m6` columns)

Every gear item (weapons, shields, armor) has seven modifier columns. They are **plain percentages**,
and negative values are penalties.

| Column | Modifier | Effect |
|---|---|---|
| `m0` | DAMAGE | damage × (1 + value %) |
| `m1` | ARMOR_PIERCING | flat armor piercing added |
| `m2` | ATTACK_SPEED | swing and draw time × 100 / (100 + value): +25 = 20% shorter, −15 = 18% longer |
| `m3` | CRIT_CHANCE | percentage points added to the crit chance |
| `m4` | CRIT_DAMAGE | on a weapon: its crit multiplier in % (150 = ×1.5; 0 = use the default). On worn gear: added to it |
| `m5` | MOVEMENT_SPEED | walking speed × (1 + value %) |
| `m6` | DODGE_SPEED | dash cooldown × 100 / (100 + value) |

**Which items count**:
- **Worn gear** (shield and body armor) affects *everything* you do.
- A **weapon** affects only its own use: the sword's modifiers apply to swings, the bow's to arrows.
  For walking and dashing, the weapon of the current setup counts.

**Shields have penalties**: that's the price of their armor. The cock_block slows attacks
(and bow draws) by 15% and walking by 10%. Fighting without a shield is a playstyle: faster and
more mobile, it defends with the timed parry instead (section 7).

## 4. Critical hits

- Chance = `baseCritChancePct` (5%) + `CRIT_CHANCE` of the worn gear and the weapon used.
- A crit multiplies the damage by the weapon's `CRIT_DAMAGE` % (150 = ×1.5). Weapons with 0 use
  `defaultCritDamagePct` (150), and worn gear's `CRIT_DAMAGE` adds to it.
- A crit is applied before armor. The log marks crits with `CRIT`.

| | Crit chance | Crit multiplier |
|---|---|---|
| world_destroyer (player's sword) | 5% + 5% = 10% | ×1.5 |
| THE_ARROW_YEETER (player's bow) | 5% + 5% = 10% | ×2.0 |
| lifestealer, bow_of_lifesteal | 5% | ×1.5 (default) |

## 5. Lifesteal

Weapons whose damage has `LIFESTEAL` columns (lifestealer, bow_of_lifesteal): on a hit, with
`LFSTL_PROB` % chance, the attacker heals `LIFESTEAL` % of the damage **actually dealt** (after armor).
It never heals above max health.

| | Chance | Heals |
|---|---|---|
| lifestealer (enemies' sword) | 70-80% | 50-75% of damage dealt |
| bow_of_lifesteal | 10-20% | 80-90% of damage dealt |

## 6. Effects (bleeding, healing, stat changes)

- **On-hit effects**: arrows with effects apply them to the character they hit. The normal arrow
  causes bleeding: 1 damage every 250 ms for 2 s, 8 damage total.
- **Damage over time ignores armor.** It doesn't interrupt the bow either.
- **Healing** effects (damage type HEALING) restore health instead, never above max.
- **Stat effects** (`StatChangingEffect.csv`) change MOVEMENT_SPEED or DMG by a percentage
  (100 = normal) that moves from the initial to the final value.
- **The same effect again refreshes** its duration instead of stacking. "The same" means the same kind,
  level and buff/debuff.
- Death removes all effects.
- **Shown** as small squares above the HP bar: red for harmful, green for helpful. The HUD names
  the player's ("bleeding", "healing").

Effect numbers are in [`assets/effects/DamageEffect.csv`](../assets/effects/DamageEffect.csv):
row 0 is bleeding and row 1 is the berries' healing. The columns are DURATION, DMG_VAL, TICKRATE and
DMG_INCREASE (a multiplier applied after every tick; 1.0 = constant).

## 7. Blocking: shield or parry

The block button (`Left Ctrl`, or the shield button on touch screens) does different things depending on
whether a shield is equipped.

### With a shield
- **Hold** to raise it over its **raise time** (`Shield.csv` `RAISE_TIME`). Its armor counts in
  proportion to how far it's raised (section 2).
- A **melee hit on a fully raised shield staggers the attacker** for `shieldBlockStaggerMs` (600 ms),
  on top of the rest of its swing. That's your counter-attack window. Damage still comes through,
  reduced by the armor.
- The shield blocks from every direction and stops arrows too.
- While raised, you walk at half speed (`moveSpeedWhileBusy`), and walking raises it at half speed
  (`actionSpeedWhileMoving`). Attacking lowers it.
- The shield's penalties (section 3) apply all the time while it's equipped.

### Without a shield: timed parry
- **Press** block just before an enemy's melee hit lands. The enemy's swing shows as a dark line,
  and its wind-up is about 30% of its swing time.
- The window lasts `parryWindowMs` (250 ms) from the press. Holding the button does nothing.
- **Success**: the hit is deflected (0 damage), the attacker is staggered for `riposteStaggerMs`
  (1 s, on top of the rest of its swing), and you **instantly counter-hit** with your melee weapon.
  The counter can crit and lifesteal, but can't itself be parried.
- **Miss** (no hit within the window): you can't parry again for `parryMissCooldownMs` (600 ms).
- Arrows can't be parried.

## 8. Hit reactions

| Event | Reaction |
|---|---|
| You take damage while drawing the bow | the bow is lowered; the arrow stays in the quiver |
| A melee hit lands on your fully raised shield | the attacker is staggered 600 ms + the rest of its swing |
| You parry a melee hit (no shield) | the attacker is staggered 1000 ms + the rest of its swing, and you counter-hit |

**Staggered** characters can't move, attack, block, dash or eat. A stagger cancels their current swing,
bow draw and meal. The HUD shows `STAGGERED` for the player.

## 9. Melee

1. **Wind-up**: `meleeWindupShare` (30%) of the swing time. A dark line shows where the swing will land.
2. **Hit**: the first character on the line within `meleeReach` (1 m) takes the hit. One target only.
3. **Recovery**: the remaining 70% of the swing time.

**Swing time** is the weapon's `AT_SPD` column in ms (bare hands: `bareHandsSwingMs`), scaled by
`ATTACK_SPEED` modifiers. You can't move or start another attack while swinging. Starting a swing
lowers the shield.

| Melee.csv row | Name | Damage | AP | Swing time | Notes |
|---|---|---|---|---|---|
| 0 | world_destroyer (player) | 15-25 slashing | 0-5 | 1000-1200 ms | crit 5%, ×1.5 |
| 1 | lifestealer (enemies; spare in the backpack) | 12-20 slashing | 2-8 | 800-1000 ms | lifesteal |

## 10. Bow

**Hold** the attack button to draw and **release** to shoot. With the mouse, the arrow flies towards the
cursor. On touch screens it flies in the direction you push the attack joystick.

- **Aiming aid** while drawing, following the same flight as the real arrow:
  - a **dotted arc** (Angry Birds style) along the flight in the air;
  - a **dark line on the ground** from you to where the arrow would land.
  
  The arc grows as the bow draws, since a weak draw falls short. It stops at walls and at the first
  character in the way.
- **Dot colour**: **red** while releasing would *not* shoot (draw below `minDrawToShoot`), **green**
  once it would. There is no draw bar; the arc is the draw indicator.
- **Draw time**: `Ranged.csv` `DRAW_TIME` (ms), scaled by `ATTACK_SPEED` modifiers. The shield's −15%
  slows drawing too.
- **Releasing early**: below `minDrawToShoot` (25%) nothing is shot and the arrow stays in the quiver.
- **Arrow speed** = bow `ARROW_SPD` × draw. **Damage** = (bow `DMG` + arrow `DMG`) × draw, then
  modifiers and crit of the **bow**. Piercing damage.
- Arrows leave the bow at `arrowSpawnHeight` (1 m) and fly a real ballistic arc to the aimed point.
- While drawing you walk at half speed, and walking draws at half speed. Raising the shield or
  taking damage lowers the bow.
- Arrows are taken from the quiver's first stack, then the second.
- **Setup**: `Q` (or the weapon button) switches melee / bow; bow mode needs a bow. In bow mode with an
  empty quiver, the attack button swings the melee weapon.

| | Damage | AP | Draw time | Arrow speed | Notes |
|---|---|---|---|---|---|
| Bow 0, THE_ARROW_YEETER (equipped) | 8-12 | 5-10 | 0.8-1.2 s | 25-30 m/s | crit 5%, ×2 |
| Bow 1, bow_of_lifesteal (backpack) | 6-10 | 2-6 | 0.9-1.3 s | 22-28 m/s | lifesteal |
| Arrow 0, arrow | +10-15 | +0-5 | | | bleeding, breaks 25% |
| Arrow 1, penetrator | +15-20 | +15-25 | | | breaks 10% |

## 11. Arrows: breaking and picking up

- On every impact (ground, wall or character) an arrow **may break**, with the arrow's `BREAK_CHANCE`
  (normal 25%, penetrator 10%).
- An arrow that survives:
  - **missed** - it lies where it hit the ground;
  - **hit a wall** - it lies in front of the wall;
  - **hit a character** - it sticks in them and falls around the body when they die. An arrow that
    kills falls straight to the ground. An arrow that meets a dashing character falls where it was.
- **Walk within `arrowPickupRadius` (0.6 m)** of an arrow to pick it up. It goes into a quiver stack of
  the same type, else the backpack. If there's no room it stays on the ground.
- Arrows that fly off the map are lost.

## 12. Dash

- `Space`, or the DASH button on touch screens: a quick dash of `dashDistance` (2.5 m) over
  `dashDurationMs` (200 ms) in the direction you last walked.
- **Invulnerable** while dashing: hits don't land and effects aren't applied.
- **Cooldown** `dashCooldownMs` (1 s) after the dash, scaled by `DODGE_SPEED` modifiers.
- A dash lowers the bow and the shield, and interrupts eating. You can't dash while swinging or
  staggered. A wall ends the dash.

## 13. Healing: eating

- Keys `1`-`4` eat the food in satchel cells 1-4, row by row. On touch screens the food button eats
  the first food found.
- Eating takes `eatTimeMs` (500 ms); the HUD bar fills green. Your hands are busy: no attacking or
  blocking, and you walk at half speed.
- When finished, the food is used up and its effects start. Berries heal 2 health every 300 ms
  for 3 s, 20 in total.
- A dash, stagger or death interrupts eating and the food stays in the satchel.
- You start with `startingFood` (2) berries. There is no passive regeneration.

## 14. Moving while busy

Walking speed = `playerWalkSpeed` × busy factor × `MOVEMENT_SPEED` modifiers × stat effects.

- **Busy** means drawing, shield raised, or eating: × `moveSpeedWhileBusy` (0.5).
- Walking makes the bow draw and the shield rise at × `actionSpeedWhileMoving` (0.5).

## 15. Enemies

- Enemies fight in melee only, with `Melee.csv` row `enemyMeleeWeaponId` (lifestealer: lifesteal
  included).
- Health `enemyMaxHp`, armor `enemyArmor`.
- They walk towards the player at `enemyWalkSpeed`, stop at `enemyStopDistance` and swing when closer
  than `enemyAttackRange`. They ignore a dead player.
- They don't block, parry or dash, and they can be staggered.
- A killed enemy is replaced 2 s later; the player respawns after 3 s (`src/BoardLoop.h`).

**How long fights take** with the shipped numbers (no crits):

| | Per hit | Hits to kill 100 hp |
|---|---|---|
| player's sword vs enemy | ~16 | ~7 |
| player's full-draw arrow vs enemy | ~20 + 8 bleeding | ~4 |
| enemy's sword vs player, shield down | ~12 | ~9 |
| enemy's sword vs player, shield up | ~7 (+ the enemy is staggered) | ~15 |

## 16. Starting equipment

| Slot | Item |
|---|---|
| Melee | world_destroyer (Melee row 0) |
| Bow | THE_ARROW_YEETER (Ranged row 0) |
| Shield | cock_block (Shield row 0): 40-60 armor, attacks −15%, walking −10% |
| Armor | body_block (Armor row 0): 15-25 armor |
| Quiver | `startingArrows` (20) normal arrows; each stack holds `quiverCapacity` (20) |
| Satchel | `startingFood` (2) berries |
| Backpack | lifestealer, bow_of_lifesteal, 5 penetrator arrows - to try out in the inventory (`I`) |

Take the shield off in the inventory to try the shieldless playstyle. The loadout itself is in
`Player::Player()` in `src/Player.cpp`.

## 17. Controls

| | Keyboard and mouse | Touch |
|---|---|---|
| Move | `W A S D` | left joystick |
| Attack / draw bow | hold left mouse button, release to shoot | hold right joystick, lift finger to shoot |
| Aim | mouse cursor | right joystick direction |
| Block (shield) / parry (no shield) | `Left Ctrl` | shield button |
| Dash | `Space` | DASH button |
| Eat | `1`-`4` | food button |
| Melee / bow | `Q` | weapon button |
| Inventory | `I` (`Esc` closes) | - |

Opening the inventory lowers the bow and the shield.

## 18. Not in the fight system yet

- **Facing**: shields block from every direction.
- **Elemental resistances** have data structures but no values; all characters take normal elemental damage.
- **Enemy variety**: enemies don't block, dash or use bows, and don't wear armor items.
- **Stamina**: none by design; pacing comes from swing, draw, raise and cooldown times.

## 19. Rules file reference (`assets/rules/Combat.txt`)

One `key value` pair per line; everything after `#` is a comment. Unknown keys and bad values
are reported in the log and ignored. A key that is missing keeps its default.

| Key | Default | Meaning |
|---|---|---|
| `playerMaxHp` | 100 | player health |
| `enemyMaxHp` | 100 | enemy health |
| `playerWalkSpeed` | 4.0 | m/s |
| `enemyWalkSpeed` | 1.0 | m/s |
| `enemyAttackRange` | 1.0 | m, enemy swings when closer |
| `enemyStopDistance` | 0.8 | m, enemy stops walking when closer |
| `enemyMeleeWeaponId` | 1 | `Melee.csv` row (0 = first) for enemies |
| `enemyArmor` | 15 | enemies' armor |
| `armorK` | 50 | armor formula constant: armor = K blocks 50% |
| `baseCritChancePct` | 5 | % added to gear crit chance |
| `defaultCritDamagePct` | 150 | crit multiplier of weapons with `CRIT_DAMAGE` 0 |
| `bareHandsDamage` | 5 | damage without a melee weapon |
| `bareHandsSwingMs` | 700 | swing time without a melee weapon |
| `meleeReach` | 1.0 | m |
| `meleeWindupShare` | 0.3 | part of the swing before the hit lands |
| `minDrawToShoot` | 0.25 | weakest draw that shoots |
| `arrowSpawnHeight` | 1.0 | m |
| `shieldBlockStaggerMs` | 600 | stagger for hitting a fully raised shield |
| `parryWindowMs` | 250 | timed parry window (no shield) |
| `parryMissCooldownMs` | 600 | after a missed parry |
| `riposteStaggerMs` | 1000 | stagger of a parried attacker |
| `moveSpeedWhileBusy` | 0.5 | walking speed while drawing, blocking or eating |
| `actionSpeedWhileMoving` | 0.5 | draw / raise speed while walking |
| `dashDistance` | 2.5 | m |
| `dashDurationMs` | 200 | dash time, invulnerable |
| `dashCooldownMs` | 1000 | after a dash |
| `arrowPickupRadius` | 0.6 | m |
| `quiverCapacity` | 20 | arrows per quiver stack |
| `startingArrows` | 20 | arrows in the quiver at start |
| `eatTimeMs` | 500 | eating time |
| `startingFood` | 2 | berries in the satchel at start |

## 20. Item CSV columns used by the fight system

Rows are read in order: row 0 is the first item line under the header.
If you add or remove a column, also change the `BLANK_SPACES_IN_LINE` number in the first line.
The loader skips `2 × N + 6` header words.

| File | Column | Used for |
|---|---|---|
| all gear | `m0`-`m6` | modifiers in % (section 3) |
| `Melee.csv` | `DMG_MIN/MAX`, `DMG_TYPE` | damage and type (0 slashing, 1 piercing, 2 bludgeoning, 3+ elemental) |
| | `AP_MIN/MAX` | armor piercing |
| | `LIFESTEAL_MIN/MAX`, `LFSTL_PROB_MIN/MAX` | % of damage healed, % chance |
| | `AT_SPD_MIN/MAX` | swing time in ms |
| `Ranged.csv` | `DMG`, `AP`, `LIFESTEAL...` | bow part of arrow hits |
| | `DRAW_TIME_MIN/MAX` | ms to full draw |
| | `ARROW_SPD_MIN/MAX` | m/s at full draw |
| `Arrow.csv` | `DMG`, `DMG_TYPE`, `AP` | arrow part of arrow hits |
| | `BREAK_CHANCE` | 0-1, rolled on every impact |
| | `EFFECTS_NR` + rows below | on-hit effects (type, id in `assets/effects`) |
| `Shield.csv` | `ARMOR_MIN/MAX` | armor when fully raised |
| | `RAISE_TIME_MIN/MAX` | ms to raise fully |
| `Armor.csv` | `ARMOR_MIN/MAX` | body armor |
| | `EFF_SLASH`, `EFF_PIERCE`, `EFF_BLUNT` | 0-1 share of the armor counted vs each physical type |
| `Food.csv` | effects | applied when eaten |

## 21. Where it is in the code

| Topic | Code |
|---|---|
| formulas (armor, piercing, modifiers) | `src/CombatMath.cpp` |
| damage pipeline, modifiers, crits, lifesteal, stagger, parry / riposte, dash, effects | `src/AbstractCharacterCombat.cpp` |
| what a hit carries / what happened | `src/HitInfo.h` |
| attack input, melee swing, bow draw / release, shield raise, movement | `src/AbstractCharacter.cpp` |
| arrow flight, hits, breaking, leaving arrows behind | `src/ArrowProjectile.cpp`, `src/FlightPath.cpp` |
| equipment, quiver, eating, picking arrows up, starting loadout | `src/Player.cpp` |
| enemy behaviour | `src/AiCharacter.cpp` |
| input, autoplay, respawns | `src/BoardLoop.cpp` |
| rules file | `src/CombatRules.h/.cpp` |
| HUD, touch buttons, HP bar effect markers | `src/CombatHudUI.cpp`, `src/ActionButton.cpp`, `src/HpBarDrawable.cpp` |
| tests of every rule above | `tests/TestDamage.cpp`, `tests/TestReactions.cpp`, `tests/TestCombat.cpp`, `tests/TestBow.cpp` |
