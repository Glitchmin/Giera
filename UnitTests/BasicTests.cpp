#include "pch.h"
#include "CppUnitTest.h"
#include "../Giera/AbstractTimer.h"
#include "../Giera/GeneralTimer.h"
#include "../Giera/SubTimer.h"
#include "../Giera/GameTime.h"
#include "../Giera/Calculator.h"
#include "../Giera/GeneralTimer.cpp"
#include "../Giera/SubTimer.cpp"
#include "../Giera/AbstractTimer.cpp"
#include "../Giera/GameTime.cpp"
#include "../Giera/Calculator.cpp"
#include "../Giera/Coordinates.h"
#include "../Giera/Coordinates.cpp"
#include "../Giera/Map.h"
#include "../Giera/Map.cpp"
#include "../Giera/MapTile.cpp"
#include "../Giera/Directions.h"
#include "../Giera/LandscapeTypes.h"
#include "../Giera/MapTypes.h"
#include "../Giera/ValuesRange.h"
#include "../Giera/ValuesRange.cpp"
#include "../Giera/GrasslandsGenerator.h"
#include "../Giera/GrasslandsGenerator.cpp"
#include "../Giera/Logger.h"
#include "../Giera/AbstractMapGenerator.h"
#include "../Giera/AbstractMapGenerator.cpp"
#include "../Giera/FileHandler.cpp"
#include "../Giera/FileHandler.h"
#include "../Giera/MapFileHandler.h"
#include "../Giera/MapFileHandlerSaving.cpp"
#include "../Giera/MapFileHandlerReading.cpp"
#include "../Giera/MapElementsHandler.h"
#include "../Giera/MapElementsHandler.cpp"
#include "../Giera/MapElementsFileHandler.h"
#include "../Giera/MapElementsFileHandler.cpp"
#include "../Giera/AbstractMapElement.h"
#include "../Giera/AbstractMapElement.cpp"
#include "../Giera/Wall.h"
#include "../Giera/Wall.cpp"
#include "../Giera/Terrain.h"
#include "../Giera/Terrain.cpp"
#include "../Giera/Foreground.h"
#include "../Giera/Foreground.cpp"
#include "../Giera/Background.h"
#include "../Giera/Background.cpp"

#include "../Giera/SpawningDetails.h"
#include "../Giera/SpawningDetails.cpp"
#include "../Giera/ItemSpawner.h"
#include "../Giera/ItemSpawner.cpp"
#include "../Giera/Position.h"
#include "../Giera/Position.cpp"
#include "../Giera/AbstractGeometryFigure.h"
#include "../Giera/LineSegment.h"
#include "../Giera/LineSegment.cpp"
#include "../Giera/Cuboid.h"
#include "../Giera/Cuboid.cpp"
#include "../Giera/Damage.cpp"
#include "../Giera/Damage.h"
#include "../Giera/Lifesteal.h"
#include "../Giera/Lifesteal.cpp"
#include "../Giera/DamageTypes.h"
#include "../Giera/DamageEffect.h"
#include "../Giera/DamageEffect.cpp"
#include "../Giera/AbstractEffect.cpp"
#include "../Giera/AbstractEffect.h"
#include "../Giera/AbstractNPC.h"
#include "../Giera/AbstractNPC.cpp"
#include "../Giera/NPC_AttributeTypes.h"
#include "../Giera/NPCObserver.h"
#include "../Giera/NPCObserver.cpp"
#include "../Giera/NPCTypes.h"
#include "../Giera/Player.h"
#include "../Giera/Player.cpp"
#include "../Giera/StatChangingEffect.h"
#include "../Giera/StatChangingEffect.cpp"
#include "../Giera/BaseItemHandler.h"
#include "../Giera/BaseItemHandler.cpp"
#include "../Giera/BaseItemFileHandler.h"
#include "../Giera/BaseItemFileHandler.cpp"

#include "../Giera/NormalItems.h"
#include "../Giera/AbstractItem.cpp"
#include "../Giera/AbstractGearItem.cpp"
#include "../Giera/AbstractWeapon.cpp"
#include "../Giera/Armor.cpp"
#include "../Giera/Arrow.cpp"
#include "../Giera/Food.cpp"
#include "../Giera/MeleeWeapon.cpp"
#include "../Giera/RangedWeapon.cpp"
#include "../Giera/Misc.cpp"
#include "../Giera/Readable.cpp"
#include "../Giera/Shield.cpp"

#include "../Giera/AbstractEntityFileLoader.h"
#include "../Giera/BaseItems.h"
#include "../Giera/AbstractBaseItem.cpp"
#include "../Giera/AbstractBaseGearItem.cpp"
#include "../Giera/AbstractBaseWeapon.cpp"
#include "../Giera/BaseArmor.cpp"
#include "../Giera/BaseArrow.cpp"
#include "../Giera/BaseFood.cpp"
#include "../Giera/BaseMeleeWeapon.cpp"
#include "../Giera/BaseRangedWeapon.cpp"
#include "../Giera/BaseMisc.cpp"
#include "../Giera/BaseReadable.cpp"
#include "../Giera/BaseShield.cpp"

#include "../Giera/EffectsFileHandler.h"
#include "../Giera/EffectsFileHandler.cpp"
#include "../Giera/EffectsHandler.h"
#include "../Giera/EffectsHandler.cpp"

#include "../Giera/Board.h"
#include "../Giera/Board.cpp"
#include "../Giera/BoardRenderer.h"
#include "../Giera/BoardRenderer.cpp"
#include "../Giera/Texture.h"
#include "../Giera/Texture.cpp"
#include "../Giera/TextureLoader.h"
#include "../Giera/TextureLoader.cpp"
#include "../Giera/Drawable.h"
#include "../Giera/Drawable.cpp"
#include "../Giera/DrawableBoardEntity.h"
#include "../Giera/DrawableBoardEntity.cpp"
#include "../Giera/Window.h"
#include "../Giera/Window.cpp"
#include "../Giera/Camera.h"
#include "../Giera/Camera.cpp"
#include "../Giera/FlightPath.h"
#include "../Giera/FlightPath.cpp"
#include "../Giera/AbstractProjectile.h"
#include "../Giera/AbstractProjectile.cpp"
#include "../Giera/ArrowProjectile.h"
#include "../Giera/ArrowProjectile.cpp"
#include "../Giera/SpellProjectile.h"
#include "../Giera/SpellProjectile.cpp"
#include "../Giera/AbstractSpell.h"
#include "../Giera/AbstractSpell.cpp"
#include "../Giera/PointSpell.h"
#include "../Giera/PointSpell.cpp"
#include "../Giera/SelfSpell.h"
#include "../Giera/SelfSpell.cpp"
#include "../Giera/ThrownSpell.h"
#include "../Giera/ThrownSpell.cpp"
#include "../Giera/AbstractGeometryFigure.h"
#include "../Giera/BoardTile.h"
#include "../Giera/BoardTile.cpp"
#include "../Giera/Hitbox.h"
#include "../Giera/Hitbox.cpp"
#include "../Giera/NPCHitbox.h"
#include "../Giera/NPCHitbox.cpp"
#include "../Giera/HittableBoardEntity.h"
#include "../Giera/HittableBoardEntity.cpp"
#include "../Giera/ProjectilesEngine.h"
#include "../Giera/ProjectilesEngine.cpp"

#include "../Giera/AbstractEqSlot.h"
#include "../Giera/AbstractEqSlot.cpp"
#include "../Giera/Dropslots.h"
#include "../Giera/Dropslots.cpp"
#include "../Giera/SingleEqSlot.h"
#include "../Giera/SingleEqSlot.cpp"
#include "../Giera/MultipleEqSlot.h"
#include "../Giera/MultipleEqSlot.cpp"
#include "../Giera/QuiverEqSlot.h"
#include "../Giera/QuiverEqSlot.cpp"

#include "../Giera/EqSlotTypes.h"
#include "../Giera/Inventory.h"
#include "../Giera/Inventory.cpp"

#include "../Giera/BoardLoop.h"
#include "../Giera/BoardLoop.cpp"
#include "../Giera/InputConfig.h"
#include "../Giera/InputConfig.cpp"
#include "../Giera/InventoryInputHandler.h"
#include "../Giera/InventoryInputHandler.cpp"
#include "../Giera/MouseButtonStateTypes.h"
#include "../Giera/MouseButtonTypes.h"
#include "../Giera/PlayerActionTypes.h"

#include "../Giera/ButtonUI.h"
#include "../Giera/ButtonUI.cpp"
#include "../Giera/ColumnLayout.h"
#include "../Giera/ColumnLayout.cpp"
#include "../Giera/HorizontalAlignmentTypes.h"
#include "../Giera/ImageResizeTypes.h"
#include "../Giera/InventoryButtonUI.h"
#include "../Giera/InventoryButtonUI.cpp"
#include "../Giera/InventoryUI.h"
#include "../Giera/InventoryUI.cpp"
#include "../Giera/RowLayout.h"
#include "../Giera/RowLayout.cpp"
#include "../Giera/UIElement.h"
#include "../Giera/UIElement.cpp"
#include "../Giera/VerticalAlignmentTypes.h"

#include "../Giera/AbstractEqSlotUIElement.h"
#include "../Giera/AbstractEqSlotUIElement.cpp"
#include "../Giera/MultipleEqSlotUI.h"
#include "../Giera/MultipleEqSlotUI.cpp"
#include "../Giera/QuiverEqSlotUI.h"
#include "../Giera/QuiverEqSlotUI.cpp"
#include "../Giera/SingleEqSlotUI.h"
#include "../Giera/SingleEqSlotUI.cpp"

#include <iostream>
#include <string>
#include <SDL.h>
#include <Windows.h>
#include <sstream>
#include <filesystem>
using Microsoft::VisualStudio::CppUnitTestFramework::Assert;
using namespace Microsoft::VisualStudio;

namespace ItemsTests {
	TEST_CLASS(BaseItemsInputTest)
	{
	public:
		TEST_METHOD(BaseArmorInputTest)
		{
			Logger::setHandler(0, 1);
			BaseItemHandler::initialize();
			auto arm = BaseItemHandler::generate<Armor>(ItemTypes::ARMOR, (int)ArmorTypes::ARMOR_1);
			Assert::AreEqual("body_block", arm->getName().c_str());
			Assert::AreEqual("blocks_your_body", arm->getDescription().c_str());
			Assert::IsTrue(arm->getValue()>=1.0 && arm->getValue()<=2.0);
			Assert::AreEqual((item_size_t)2,arm->getWidth());
			Assert::AreEqual((item_size_t)5,arm->getHeight());
			for (int i = 0;i < (int)ModifiersTypes::COUNT_MODIFIERS;i++) 
			{
				Assert::AreEqual((item_mod_t)0,arm->getModifier((ModifiersTypes) i));
			}
			Assert::IsTrue(arm->getArmor() >= 1.5 && arm->getArmor() <= 2.0);
		}

		TEST_METHOD(BaseMeleeInputTest) {
			Logger::setHandler(0, 1);
			auto mel = BaseItemHandler::generate<MeleeWeapon>(ItemTypes::MELEE_WEAPON, (int)MeleeWeaponTypes::LUFREWOP_SWORD);
			Assert::AreEqual("world_destroyer", mel->getName().c_str());
			Assert::AreEqual("destroys_your_world", mel->getDescription().c_str());
			Assert::IsTrue(mel->getValue() >= 10 && mel->getValue() <= 30);
			Assert::AreEqual((item_size_t)6, mel->getWidth());
			Assert::AreEqual((item_size_t)5, mel->getHeight());
			for (int i = 0; i < (int)ModifiersTypes::COUNT_MODIFIERS; i++) {
				Assert::AreEqual((item_mod_t)i, mel->getModifier((ModifiersTypes)i));
			}
			Assert::IsTrue(mel->getDamage()->getValue() >= 1.0 && mel->getDamage()->getValue() <= 2.0);
			Assert::AreEqual((int)DamageTypes::SLASHING, (int)mel->getDamage()->getDamageType());
			Assert::IsTrue(mel->getDamage()->getAp() >= 1 && mel->getDamage()->getAp() <= 1.5);
			Assert::IsFalse(mel->getDamage()->isLifesteal());
			Assert::IsTrue(mel->getAttackSpeed().getTimeMs() >= 69 && mel->getAttackSpeed().getTimeMs() <= 420);
		}
		TEST_METHOD(BaseMeleeLifestealInputTest) {
			Logger::setHandler(0, 1);
			auto mel = BaseItemHandler::generate<MeleeWeapon>(ItemTypes::MELEE_WEAPON, (int)MeleeWeaponTypes::SWORD_2);
			Assert::AreEqual("lifestealer", mel->getName().c_str());
			Assert::AreEqual("steals_your_life", mel->getDescription().c_str());
			Assert::IsTrue(mel->getDamage()->isLifesteal());
			auto& lfst = dynamic_cast<Lifesteal&> (*mel->getDamage());
			Assert::IsTrue(lfst.getProbability()>=70 && lfst.getProbability()<=80);
			Assert::IsTrue(lfst.getPercentStolen()>=50 && lfst.getPercentStolen()<=75);
		}
		TEST_METHOD(BaseRangedLifestealInputTest) {
			Logger::setHandler(0, 1);
			auto mel = BaseItemHandler::generate<RangedWeapon>(ItemTypes::RANGED_WEAPON, (int)RangedWeaponTypes::BOW_2);
			Assert::AreEqual("bow_of_lifesteal", mel->getName().c_str());
			Assert::AreEqual("yeets_lifesteal", mel->getDescription().c_str());
			Assert::IsTrue(mel->getDamage()->isLifesteal());
			auto& lfst = dynamic_cast<Lifesteal&> (*mel->getDamage());
			Assert::IsTrue(lfst.getProbability() >= 10 && lfst.getProbability() <= 20);
			Assert::IsTrue(lfst.getPercentStolen() >= 80 && lfst.getPercentStolen() <= 90);
		}
		TEST_METHOD(BaseArrowInputTest) {
			auto arr = BaseItemHandler::generate<Arrow>(ItemTypes::ARROW, (int)ArrowTypes::ARROW_1);
			Assert::AreEqual("penetrator", arr->getName().c_str());
			Assert::AreEqual("penetrates_your_soul", arr->getDescription().c_str());
			Assert::IsTrue(arr->getValue() >= 21 && arr->getValue() <= 37);
			Assert::AreEqual((item_size_t)1, arr->getWidth());
			Assert::AreEqual((item_size_t)2, arr->getHeight());
			Assert::IsTrue(arr->getDamage()->getValue() >= 1.5 && arr->getDamage()->getValue() <= 2.5);
			Assert::AreEqual((int)DamageTypes::PIERCING, (int)arr->getDamage()->getDamageType());
			Assert::IsTrue(arr->getDamage()->getAp() >= 0.5 && arr->getDamage()->getAp() <= 1.5);
			Assert::IsTrue(arr->getEffects().size() == 1);
			auto eff = dynamic_pointer_cast<DamageEffect>(arr->getEffects()[0]);
			
			Assert::AreEqual(false, eff->IsBuff());
			Assert::AreEqual((unsigned int)1000, eff->getDuration().getTimeMs());
			Assert::AreEqual((short)3, eff->getLevel());
			Assert::AreEqual((double)1.5, eff->getDamage()->getValue());
			Assert::AreEqual(2, (int)eff->getDamage()->getDamageType());
			Assert::AreEqual((unsigned int)100, eff->getTickrate().getTimeMs());
			Assert::AreEqual(1.2, eff->getDamageIncrease());
		}
		TEST_METHOD(BaseFoodTests) {
			auto food = BaseItemHandler::generate<Food>(ItemTypes::FOOD, (int)FoodTypes::BERRIES);
			Assert::AreEqual("borowkis", food->getName().c_str());
			Assert::AreEqual("smakowite_boroweczki", food->getDescription().c_str());
			Assert::IsTrue(food->getValue() >= 1 && food->getValue() <= 2);
			Assert::AreEqual((item_size_t)1, food->getWidth());
			Assert::AreEqual((item_size_t)1, food->getHeight());
			Assert::IsTrue(food->getEffects().size()==1);
			
			auto eff = dynamic_pointer_cast<DamageEffect>(food->getEffects()[0]);

			Assert::AreEqual(true, eff->IsBuff());
			Assert::AreEqual((unsigned int)1500, eff->getDuration().getTimeMs());
			Assert::AreEqual((short)4, eff->getLevel());
			Assert::AreEqual((double)1.5, eff->getDamage()->getValue());
			Assert::AreEqual((int)DamageTypes::HEALING, (int)eff->getDamage()->getDamageType());
			Assert::AreEqual((unsigned int)200, eff->getTickrate().getTimeMs());
			Assert::AreEqual(1.5, eff->getDamageIncrease());
		}
		TEST_METHOD(BaseMiscTests) {
			auto misc = BaseItemHandler::generate<Misc>(ItemTypes::MISC, (int)MiscTypes::MISC_1);
			Assert::AreEqual("a_stone", misc->getName().c_str());
			Assert::AreEqual("low_value", misc->getDescription().c_str());
			Assert::IsTrue(misc->getValue() >= 1 && misc->getValue() <= 2);
			Assert::AreEqual((item_size_t)1, misc->getWidth());
			Assert::AreEqual((item_size_t)1, misc->getHeight());
		}
		TEST_METHOD(RangedWeaponTests) {
			auto rang = BaseItemHandler::generate<RangedWeapon>(ItemTypes::RANGED_WEAPON, (int)RangedWeaponTypes::BOW_1);
			Assert::AreEqual("THE_ARROW_YEETER", rang->getName().c_str());
			Assert::AreEqual("yeets_arrows", rang->getDescription().c_str());
			Assert::IsTrue(rang->getValue() >= 4 && rang->getValue() <= 12);
			Assert::AreEqual((item_size_t)2, rang->getWidth());
			Assert::AreEqual((item_size_t)5, rang->getHeight());
			for (int i = 0; i < (int)ModifiersTypes::COUNT_MODIFIERS; i++) {
				Assert::AreEqual((item_mod_t)i, rang->getModifier((ModifiersTypes)i));
			}
			Assert::IsTrue(rang->getDamage()->getValue() >= 0.5 && rang->getDamage()->getValue() <= 2.5);
			Assert::IsTrue(rang->getDamage()->getAp() >= 1.0 && rang->getDamage()->getAp() <= 1.5);
			Assert::IsTrue(rang->getDamage()->isLifesteal()==false);
			Assert::AreEqual(0,(int)rang->getDamage()->getDamageType());
			Assert::IsTrue(rang->getDrawTime().getTimeMs() >= 1000 && rang->getDrawTime().getTimeMs() <= 1500);
			Assert::IsTrue(rang->getArrowVelocity() >= 2.0 && rang->getArrowVelocity() <= 4.0);
		}
		TEST_METHOD(ReadableTests) {
			auto readable = BaseItemHandler::generate<Readable>(ItemTypes::READABLE, (int)ReadableTypes::BUUK);
			Assert::AreEqual("a_book", readable->getName().c_str());
			Assert::AreEqual("lol", readable->getDescription().c_str());
			Assert::IsTrue(readable->getValue() >= 1.0 && readable->getValue() <= 2.0);
			Assert::AreEqual((item_size_t)1, readable->getWidth());
			Assert::AreEqual((item_size_t)2, readable->getHeight());
			Assert::AreEqual("witaj_podrozny", readable->getText().c_str());
		}
		TEST_METHOD(ShieldTests) {
			auto shield = BaseItemHandler::generate<Shield>(ItemTypes::SHIELD, (int)ShieldTypes::SHIELD_1);
			Assert::AreEqual("cock_block", shield->getName().c_str());
			Assert::AreEqual("blocks_your_cock", shield->getDescription().c_str());
			Assert::IsTrue(shield->getValue() >= 1.0 && shield->getValue() <= 1.0);
			Assert::AreEqual((item_size_t)2, shield->getWidth());
			Assert::AreEqual((item_size_t)5, shield->getHeight());
			for (int i = 0; i < (int)ModifiersTypes::COUNT_MODIFIERS; i++) {
				Assert::AreEqual((item_mod_t)0, shield->getModifier((ModifiersTypes)i));
			}
			Assert::IsTrue(shield->getArmor() >= 1.0 && shield->getArmor() <= 2.0);
			Assert::IsTrue(shield->getTimeToRaise().getTimeMs() >= 1500 && shield->getTimeToRaise().getTimeMs() <= 2000);
		}

	};
}
/*
namespace EqTests {
	TEST_CLASS(BaseEqTests)
	{
	public:
		TEST_METHOD(SingleEqSlotTest)
		{
			vector tmp = { ItemTypes::MELEE_WEAPON };
			SingleEqSlot slot(tmp);
			auto shield = BaseItemHandler::generate<Shield>(ItemTypes::SHIELD, (int)ShieldTypes::SHIELD_1);
			auto mel = BaseItemHandler::generate<MeleeWeapon>(ItemTypes::MELEE_WEAPON, (int)MeleeWeaponTypes::SWORD_2);
			Assert::IsTrue(slot.getItem(0,0)== nullopt);
			slot.insertAcceptedItem(0, 0, mel);
			Assert::IsTrue(slot.getItem(0,0)!=nullopt);
			slot.removeItem(0, 0);
			Assert::IsTrue(slot.getItem(0,0)== nullopt);
			Assert::IsTrue(slot.isAccepted(0, 0, mel));
			slot.insertAcceptedItem(0, 0, mel);
			Assert::IsFalse(slot.isAccepted(0, 0, shield));
		}
		TEST_METHOD(MultipleEqSlotTest)
		{
			vector tmp = { ItemTypes::MELEE_WEAPON };
			MultipleEqSlot slot(tmp, 22);
			auto shield = BaseItemHandler::generate<Shield>(ItemTypes::SHIELD, (int)ShieldTypes::SHIELD_1);
			auto mel = BaseItemHandler::generate<MeleeWeapon>(ItemTypes::MELEE_WEAPON, (int)MeleeWeaponTypes::SWORD_2);
			Assert::IsTrue(slot.getItem(0, 0) == nullopt);
			slot.insertAcceptedItem(0, 0, mel);
			Assert::IsTrue(slot.getItem(0, 0) != nullopt);
			slot.removeItem(0, 0);
			Assert::IsTrue(slot.getItem(0, 0) == nullopt);
			Assert::IsTrue(slot.isAccepted(0, 0, mel));
			slot.insertAcceptedItem(0, 0, mel);
			Assert::IsFalse(slot.isAccepted(1, 3, mel));
			slot.insertAcceptedItem(0.5, 0, mel);
			Assert::IsFalse(slot.isAccepted(0, 0, shield));
		}
		TEST_METHOD(QuiverEqSlotTest)
		{
			QuiverEqSlot slot(5);
			auto arr = BaseItemHandler::generate<Arrow>(ItemTypes::ARROW, (int)ArrowTypes::ARROW_1);
			auto arr2 = BaseItemHandler::generate<Arrow>(ItemTypes::ARROW, (int)ArrowTypes::THE_PENETRATOR);
			auto mel = BaseItemHandler::generate<MeleeWeapon>(ItemTypes::MELEE_WEAPON, (int)MeleeWeaponTypes::SWORD_2);
			Assert::IsTrue(slot.getItem(0, 0) == nullopt);
			slot.insertAcceptedItem(0, 0, arr);
			Assert::IsTrue(slot.getItem(0, 0) != nullopt);
			slot.removeItem(0, 0);
			Assert::IsTrue(slot.getItem(0, 0) == nullopt);
			slot.insertAcceptedItem(0, 0, arr);
			Assert::IsTrue(slot.isAccepted(0, 0, arr));
			slot.insertAcceptedItem(0, 0, arr);
			Assert::IsTrue(arr->getName() != arr2->getName());
			Assert::IsFalse(slot.isAccepted(0, 0, arr2));
			Assert::IsTrue(slot.isAccepted(0.7, 0, arr2));
		}
	};
}
*/
namespace MapTests
{
	TEST_CLASS(CoordinatesTests)
	{
	public:
		TEST_METHOD(CoordinatesOperatorsOverloadTest)
		{
			Coordinates c1(1, 2);
			Coordinates c2(3, 5);
			Coordinates c3 = c1 + c2;
			Assert::AreEqual(c3.getX(), (unsigned int)4);
			Assert::AreEqual(c3.getY(), (unsigned int)7);
		}
	};
	TEST_CLASS(MapClassTest)
	{
	public:
		TEST_METHOD(MapFileHandlerTestsTilebyTile)
		{

			stringstream ss;
			ss << std::filesystem::current_path();
			CppUnitTestFramework::Logger::WriteMessage(ss.str().c_str());
			Logger::setHandler(0, 1);
			Map map = Map(LandscapeTypes::GRASSLAND, MapTypes::GIERA,
				Directions::NORTH, 10, 15, SDL_GetTicks());
			MapFileHandler mapFileHandler;
			mapFileHandler.saveMap(map);
			auto map2 = mapFileHandler.readMap(MapTypes::GIERA);
			for (int x=0; x<map.getSizeX();x++)
			{
				for (int y=0;y< map.getSizeY();y++)
				{
					std::stringstream ss;
					ss << *map.getMapTile(Coordinates(x, y));
					std::stringstream ss2;
					ss2 << *map2.getMapTile(Coordinates(x, y));
					Assert::AreEqual(ss.str(), ss2.str());
 				}
			}
		}
		TEST_METHOD(MapFileHandlerTestsSeed)
		{
			stringstream ss;
			ss << std::filesystem::current_path();
			CppUnitTestFramework::Logger::WriteMessage(ss.str().c_str());
			Map map = Map(LandscapeTypes::GRASSLAND, MapTypes::QUEST_MAP,
				Directions::NORTH, 10, 15, SDL_GetTicks());
			map.setMapTile(Coordinates(3, 3),
				make_shared<MapTile>(Position(3,3,0),TerrainTypes::SAND, Rotations::RIGHT, ForegroundTypes::TALL_GRASS, BackgroundTypes::GRASS, WallTypes::BUSH));
			MapFileHandler mapFileHandler;
			mapFileHandler.saveMap(map);
			auto map2 = mapFileHandler.readMap(MapTypes::QUEST_MAP);
			for (int x = 0; x < map.getSizeX();x++)
			{
				for (int y = 0;y < map.getSizeY();y++)
				{
					std::stringstream ss;
					ss << *map.getMapTile(Coordinates(x, y));
					std::stringstream ss2;
					ss2 << *map2.getMapTile(Coordinates(x, y));
					Assert::AreEqual(ss.str(), ss2.str());
				}
			}
		}
		TEST_METHOD(ConstructorTest)
		{
			unsigned int sizeX = 10;
			unsigned int sizeY = 15;
			int seed = 10;
			std::shared_ptr<Map> map = std::make_shared<Map>(LandscapeTypes::GRASSLAND, MapTypes::QUEST_MAP,
				Directions::NORTH, sizeX, sizeY, seed);
			Assert::AreEqual(seed, map->getSeed());
			Assert::AreEqual(sizeX, map->getSizeX());
			Assert::AreEqual(sizeY, map->getSizeY());
			Assert::IsTrue(map->getMapChanges().empty());
			Assert::AreEqual((int)map->getStartDirection(), (int)Directions::NORTH);
			Assert::AreEqual((int)map->getMapType(), (int)MapTypes::QUEST_MAP);
			Assert::AreEqual((int)map->getLandscapeType(), (int)LandscapeTypes::GRASSLAND);
		}
	};
	TEST_CLASS(GeneratorTests)
	{
	public:
		TEST_METHOD(GrasslandsTest1) {
			for (int i = 0; i < 100; i++) {
				auto map1 = std::make_shared<Map>(LandscapeTypes::GRASSLAND, MapTypes::QUEST_MAP, Directions::NORTH, 15, 10, i * SDL_GetTicks());
				int rocksNumber = 0;
				int bushesNumber = 0;
				for (int x = 0; x < map1->getSizeX(); x++) {
					for (int y = 0; y < map1->getSizeY(); y++) {
						rocksNumber += (map1->getMapTile(Coordinates(x, y))->getWallType() == WallTypes::ROCK);
						bushesNumber += (map1->getMapTile(Coordinates(x, y))->getWallType() == WallTypes::BUSH);
					}
				}

				Assert::IsTrue(rocksNumber >= (int)(GrasslandsGenerator::getRockRatio().getMin() * map1->getSizeX() * map1->getSizeY()));
				Assert::IsTrue(rocksNumber <= (int)(GrasslandsGenerator::getRockRatio().getMax() * map1->getSizeX() * map1->getSizeY()));
				Assert::IsTrue(bushesNumber >= (int)(GrasslandsGenerator::getBushRatio().getMin() * map1->getSizeX() * map1->getSizeY()));
				Assert::IsTrue(bushesNumber <= (int)(GrasslandsGenerator::getBushRatio().getMax() * map1->getSizeX() * map1->getSizeY()));
			}
		}
	};
}

namespace DamageAndEffectsTests
{
	TEST_CLASS(DamageClassTest) {
		TEST_METHOD(ConstructorTest)
		{
			auto damage = std::make_unique<Damage>(2.0,-1, DamageTypes::POISON);
			Assert::AreEqual(2.0, damage->getValue());
		}
		TEST_METHOD(MultiplyTest) {
			auto damage = std::make_unique<Damage>(2.5,-1, DamageTypes::POISON);
			damage->multiply(3);
			Assert::AreEqual(7.5, damage->getValue());
		}
	};
	TEST_CLASS(DamageEffectTest) {
		TEST_METHOD(ConstructorTest) {
			auto damage = std::make_unique<Damage>(2.0,-1, DamageTypes::POISON);
			DamageEffect damageEffect(move(damage), Time(1000),
				0, 1, std::shared_ptr<AbstractNPC>(nullptr), std::shared_ptr<AbstractNPC>(nullptr), Time(300));
			Assert::AreEqual(2.0, damageEffect.getDamage()->getValue());
			Assert::AreEqual((unsigned int)1000, damageEffect.getDuration().getTimeMs());
			Assert::AreEqual((unsigned int)1000, damageEffect.getTimeLeft().getTimeMs());
			Assert::AreEqual((unsigned int)300, damageEffect.getTimeUntilTick().getTimeMs());
			Assert::AreEqual((unsigned int)300, damageEffect.getTickrate().getTimeMs());
			Assert::AreEqual(1.0, damageEffect.getDamageIncrease());
		}
		TEST_METHOD(TotalDamageTest) {
			auto damage = std::make_unique<Damage>(2.0,-1, DamageTypes::POISON);
			DamageEffect damageEffect(move(damage),Time(1000),
				0,1, std::shared_ptr<AbstractNPC>(nullptr), std::shared_ptr<AbstractNPC>(nullptr),Time(300));
			damage = std::make_unique<Damage>(1.0,-1, DamageTypes::POISON);
			Assert::AreEqual(6.0,damageEffect.calculateTotalDamage());
			damageEffect = DamageEffect(move(damage), Time(1000), 
				0, 1, std::shared_ptr<AbstractNPC>(nullptr), std::shared_ptr<AbstractNPC>(nullptr), Time(300),2);
			Assert::AreEqual(7.0, damageEffect.calculateTotalDamage());
		}
		TEST_METHOD(RealDamageTest) {
			auto damage = std::make_unique<Damage>(1.0,-1, DamageTypes::POISON);
			auto damageEffect = DamageEffect(move(damage), Time(1000),
				0, 1, std::shared_ptr<AbstractNPC>(nullptr), std::shared_ptr<AbstractNPC>(nullptr), Time(300), 2);
			GeneralTimer generalTimer;
			Time lastTimeCalc = generalTimer.getTime();
			Time lastTimeUntilTick = damageEffect.getTimeUntilTick();
			double sum = 0.0;
			while (damageEffect.subtractFromTimeLeft(generalTimer.getTime() - lastTimeCalc)==0) {
				lastTimeCalc = generalTimer.getTime();
				Sleep(Calculator::getRandomInt(5,10));
				if (lastTimeUntilTick < damageEffect.getTimeUntilTick()) {
					sum += damageEffect.getDamage()->getValue()/damageEffect.getDamageIncrease();
				}
				lastTimeUntilTick = damageEffect.getTimeUntilTick();
				generalTimer.updateTime();
			}
			Assert::AreEqual(7.0, sum);
		}
	};
	TEST_CLASS(StatChangingEffectTest) {
		TEST_METHOD(ConstructorTest) {
			StatChangingEffect statChangingEffect(Time(1000), 1, 1, std::shared_ptr<AbstractNPC>(nullptr),
				std::shared_ptr<AbstractNPC>(nullptr), 10, 15, Time(500), NPC_AttributeTypes::HP);
			Assert::AreEqual(10.0, statChangingEffect.getCurrentValue());
			Assert::AreEqual(Time(1000).getTimeMs(), statChangingEffect.getTimeLeft().getTimeMs());
			Assert::AreEqual((int)NPC_AttributeTypes::HP, (int)statChangingEffect.getAttributeType());
		}
		TEST_METHOD(ValueChangeTest) {
			StatChangingEffect statChangingEffect(Time(1000), 1, 1, std::shared_ptr<AbstractNPC>(nullptr),
				std::shared_ptr<AbstractNPC>(nullptr), 10, 15, Time(500), NPC_AttributeTypes::HP);
			GeneralTimer generalTimer;
			Time lastTimeCalc = generalTimer.getTime();
			double sum = 0.0;
			double past_value = 9.9;
			while (statChangingEffect.subtractFromTimeLeft(generalTimer.getTime() - lastTimeCalc) == 0) {
				lastTimeCalc = generalTimer.getTime();
				Sleep(Calculator::getRandomInt(5, 10));
				if (generalTimer.getTime() < Time(500)) {
					Assert::IsTrue(statChangingEffect.getCurrentValue() > past_value);
				}
				Assert::IsTrue(statChangingEffect.getCurrentValue() >= past_value);
				generalTimer.updateTime();
			}
			
		}
	};
	TEST_CLASS(EffectsHandlerTest) {
		TEST_METHOD(DamageEffectTest) {
			auto eff = EffectsHandler::getEffect<DamageEffect>(EffectTypes::DAMAGE_EFFECT, 0);
			Assert::AreEqual(false, eff->IsBuff());
			Assert::AreEqual((unsigned int) 1000, eff->getDuration().getTimeMs());
			Assert::AreEqual((short)3, eff->getLevel());
			Assert::AreEqual((double)1.5, eff->getDamage()->getValue());
			Assert::AreEqual(2, (int)eff->getDamage()->getDamageType());
			Assert::AreEqual((unsigned int)100, eff->getTickrate().getTimeMs());
			Assert::AreEqual(1.2, eff->getDamageIncrease());

		}
		TEST_METHOD(StatChangingEffectTest) {
			auto eff = EffectsHandler::getEffect<StatChangingEffect>(EffectTypes::STAT_CHANGING_EFFECT, 0);
			Assert::AreEqual(true, eff->IsBuff());
			Assert::AreEqual((unsigned int)800, eff->getDuration().getTimeMs());
			Assert::AreEqual((short)1, eff->getLevel());
			Assert::AreEqual((double)50, eff->getInitialPercentValue());
			Assert::AreEqual((double)80, eff->getFinalPercentValue());
			Assert::AreEqual((unsigned int)200, eff->getTimeToReachFinal().getTimeMs());
			Assert::AreEqual(0, (int)eff->getAttributeType());
		}
	};
}

namespace UtilityTests
{
	TEST_CLASS(FileHandlerTest) {
		TEST_METHOD(AppendAndReadTest)
		{
			stringstream ss;
			ss << std::filesystem::current_path();
			CppUnitTestFramework::Logger::WriteMessage(ss.str().c_str());
			Logger::setLevel(LoggingLevels::DEBUG);
			Logger::setHandler(0, 1);
			FileHandler fileHandler("test1", FileModeTypes::WRITE);
			char tmp = 'h';
			fileHandler.saveToFile(tmp);
			fileHandler.closeFile();
			fileHandler.openFile("test1", FileModeTypes::APPEND);
			tmp = 'a';
			fileHandler.saveToFile(tmp);
			fileHandler.saveToFile(tmp);
			fileHandler.closeFile();
			std::string str;
			
			fileHandler.openFile("test1", FileModeTypes::READ);
			for (int i = 0; i < 3;i++) {
				string tmp = "b";
				fileHandler.readFromFile(tmp);
				str += tmp;
			}
			fileHandler.closeFile();
			Assert::AreEqual(str.c_str(), "haa");
		}
		TEST_METHOD(IntSaveTest)
		{
			Logger::setHandler(0, 1);
			FileHandler fileHandler("test1", FileModeTypes::WRITE);
			int tmp = 15;
			fileHandler.saveToFile(tmp);
			fileHandler.closeFile();
			fileHandler.openFile("test1", FileModeTypes::APPEND);
			tmp = 10;
			fileHandler.saveToFile(tmp);
			fileHandler.saveToFile(tmp);
			fileHandler.closeFile();
			int ans = 0;
			fileHandler.openFile("test1", FileModeTypes::READ);
			for (int i = 0; i < 3;i++) {
				int tmp = 0;
				fileHandler.readFromFile(tmp);
				ans += tmp;
			}
			fileHandler.closeFile();
			Assert::AreEqual(ans, 10+10+15);
		}
	};
	TEST_CLASS(TimeTests)
	{
	public:
		//those tests might not be working if launched on a VERY slow (or occupied with something else) device. Just rerun them before drawing conclusions
		TEST_METHOD(TimeOperatorsOverloadTest)
		{
			Time time1(100);
			Time time2(250);
			Time time3 = time1 + time2;
			Assert::AreEqual(time3.getTimeMs(), (unsigned int)350);
			Assert::AreEqual(time1.getTimeMs(), (unsigned int)100);
			Assert::AreEqual(time2.getTimeMs(), (unsigned int)250);

			Time time4 = time2 - time1;
			Assert::AreEqual(time4.getTimeMs(), (unsigned int)150);
			Assert::AreEqual(time1.getTimeMs(), (unsigned int)100);
			Assert::AreEqual(time2.getTimeMs(), (unsigned int)250);

			Time time5(4);
			time5 += time1;
			Assert::AreEqual(time5.getTimeMs(), (unsigned int)104);
			Assert::AreEqual(time1.getTimeMs(), (unsigned int)100);

			Time time6(104);
			time6 -= time1;
			Assert::AreEqual(time6.getTimeMs(), (unsigned int)4);
			Assert::AreEqual(time1.getTimeMs(), (unsigned int)100);

			Time time7 = time1 * 1.5;
			Assert::AreEqual(time7.getTimeMs(), (unsigned int)150);
			Assert::AreEqual(time1.getTimeMs(), (unsigned int)100);

			Time time8(100);
			Time time9(150);
			time8 -= time9;
			Assert::AreEqual(time8.getTimeMs(), (unsigned int)0);
			Assert::AreEqual(time9.getTimeMs(), (unsigned int)150);

			Assert::AreEqual(time1.getTimeS(),0.1);
		}
		TEST_METHOD(GeneralTimerTestTempo1_0)
		{
			GeneralTimer generalTimer;
			Assert::IsTrue(generalTimer.getTime().getTimeMs() < 5);
			unsigned int time = SDL_GetTicks();
			while (SDL_GetTicks() < time + 200)
			{
				generalTimer.updateTime();
			}
			Assert::IsTrue(generalTimer.getTime().getTimeMs() < 210);
			Assert::IsTrue(generalTimer.getTime().getTimeMs() > 190);
		}
		TEST_METHOD(GeneralTimerTestTempo1_5)
		{
			GeneralTimer generalTimer;
			Assert::IsTrue(generalTimer.getTime().getTimeMs() < 5);
			unsigned int time = SDL_GetTicks();
			generalTimer.setTempo(1.5);
			while (SDL_GetTicks() < time + 200)
			{
				generalTimer.updateTime();
				Sleep(7);
			}
			Assert::IsTrue(generalTimer.getTime().getTimeMs() < 315);
			Assert::IsTrue(generalTimer.getTime().getTimeMs() > 285);
		}
		TEST_METHOD(GeneralTimerTestTempo0_5)
		{
			GeneralTimer generalTimer;
			Time answer = generalTimer.getTime();
			Assert::IsTrue(answer.getTimeMs() < 5);
			unsigned int time = SDL_GetTicks();
			generalTimer.setTempo(0.5);
			while (SDL_GetTicks() < time + 200)
			{
				generalTimer.updateTime();
			}
			answer = generalTimer.getTime();

			Assert::IsTrue(answer.getTimeMs() < 105);
			Assert::IsTrue(answer.getTimeMs() > 95);
		}
		TEST_METHOD(GeneralTimerTestPause)
		{
			GeneralTimer generalTimer;
			Assert::IsTrue(generalTimer.getTime().getTimeMs() < 5);
			unsigned int time = SDL_GetTicks();
			generalTimer.setTempo(2.5);
			generalTimer.pause();
			while (SDL_GetTicks() < time + 100)
			{
				generalTimer.updateTime();
			}
			generalTimer.unpause();
			time = SDL_GetTicks();
			while (SDL_GetTicks() < time + 100)
			{
				generalTimer.updateTime();
			}
			Assert::IsTrue(generalTimer.getTime().getTimeMs() < 2.5 * 105);
			Assert::IsTrue(generalTimer.getTime().getTimeMs() > 2.5 * 95);
		}
		TEST_METHOD(SubTimerTest)
		{
			auto generalTimer = std::make_shared<GeneralTimer>();
			Assert::IsTrue(generalTimer->getTime().getTimeMs() < 5);
			unsigned int time = SDL_GetTicks();
			auto subTimer = std::make_shared<SubTimer>(generalTimer);
			while (SDL_GetTicks() < time + 200)
			{
				generalTimer->updateTime();
				subTimer->updateTime();
			}
			Assert::IsTrue(subTimer->getTime().getTimeMs() < 210);
			Assert::IsTrue(subTimer->getTime().getTimeMs() > 190);
		}
		TEST_METHOD(SubTimerTestDiffTempos)
		{
			std::shared_ptr <GeneralTimer> generalTimer(new GeneralTimer);
			Assert::IsTrue(generalTimer->getTime().getTimeMs() < 5);
			unsigned int time = SDL_GetTicks();
			std::shared_ptr<SubTimer> subTimer(new SubTimer(generalTimer));
			generalTimer->setTempo(2.0);
			subTimer->setTempo(0.5);
			while (SDL_GetTicks() < time + 200)
			{
				generalTimer->updateTime();
				subTimer->updateTime();
			}
			Assert::IsTrue(subTimer->getTime().getTimeMs() < 210);
			Assert::IsTrue(subTimer->getTime().getTimeMs() > 190);
		}
		TEST_METHOD(SubTimerPauseTest)
		{
			std::shared_ptr <GeneralTimer> generalTimer(new GeneralTimer);
			Assert::IsTrue(generalTimer->getTime().getTimeMs() < 5);
			unsigned int time = SDL_GetTicks();
			std::shared_ptr<SubTimer> subTimer(new SubTimer(generalTimer));
			generalTimer->setTempo(2.0);
			subTimer->pause();
			while (SDL_GetTicks() < time + 100)
			{
				generalTimer->updateTime();
				subTimer->updateTime();
			}
			time = SDL_GetTicks();
			subTimer->unpause();
			while (SDL_GetTicks() < time + 100)
			{
				generalTimer->updateTime();
				subTimer->updateTime();
			}
			Assert::IsTrue(subTimer->getTime().getTimeMs() < 210);
			Assert::IsTrue(subTimer->getTime().getTimeMs() > 190);
		}
	};
	TEST_CLASS(CalculatorTests)
	{
	public:
		//those tests might not be working if launched on a VERY slow (or occupied with something else) device. Just rerun them before drawing conclusions
		TEST_METHOD(getIntFromDoubleWithProbTest) {
			srand(time(NULL));

			int wyn, sum = 0;

			for (int i = 0; i < 1000; i++) {
				wyn = Calculator::getIntFromDoubleWithProb(6.9);
				Assert::IsTrue(wyn == 6 || wyn == 7);
				sum += wyn;
			}

			Assert::IsTrue(sum >= 6800 && sum <= 7000);
		}
		TEST_METHOD(calculateChanceTest) {
			srand(time(NULL));

			int sum = 0;
			for (int i = 0; i < 1000; i++)
				sum += Calculator::calculateChance(0.25);

			Assert::IsTrue(sum >= 200 && sum <= 300);

			sum = 0;
			for (int i = 0; i < 1000; i++)
				sum += Calculator::calculateChance(0.5);

			Assert::IsTrue(sum >= 450 && sum <= 550);

			sum = 0;
			for (int i = 0; i < 1000; i++)
				sum += Calculator::calculateChance(0.75);

			Assert::IsTrue(sum >= 700 && sum <= 800);
		}
		TEST_METHOD(getRandomDoubleTest) {
			srand(time(NULL));

			double wyn = 0;
			int _min = 0, _max = 0, wow = 0;
			unsigned int sum[25] = {};

			for (int i = 0; i < 1000; i++) {
				wyn = Calculator::getRandomDouble(0, 25);
				Assert::IsTrue(wyn >= 0 && wyn <= 25);
				if ((int)wyn >= 25) {
					wow++;  // how many times the double got really close to 25
					continue;
				}
				sum[int(wyn)]++;
				_min = min(_min, sum[int(wyn)]);
				_max = max(_max, sum[int(wyn)]);
			}

			Assert::IsTrue(_max - _min <= 75);
			Assert::IsTrue(wow < 2); // very not likely to happen even once
		}
		TEST_METHOD(getRandomIntTest) {
			srand(time(NULL));

			int wyn = 0, _min = 0, _max = 0;
			unsigned int sum[26] = {};

			for (int i = 0; i < 1000; i++) {
				wyn = Calculator::getRandomInt(0, 25);
				Assert::IsTrue(wyn >= 0 && wyn <= 25);
				sum[wyn]++;
				_min = min(_min, sum[wyn]);
				_max = max(_max, sum[wyn]);
			}
			Assert::IsTrue(_max - _min <= 75);
		}
	};
	TEST_CLASS(PositionTest)
	{
	public:
		TEST_METHOD(PositionOperatorsOverloadTest) {

			Position p1(100.0, 50.0, 10.0);
			Position p2(10.0, 50.0, 100.0);
			Position p3 = p1 + p2;
			Assert::IsTrue(p3 == Position(110.0, 100.0, 110.0));
			Assert::IsTrue(p1 == Position(100.0, 50.0, 10.0));
			Assert::IsTrue(p2 == Position(10.0, 50.0, 100.0));

			Position p4 = p2 - p1;
			Assert::IsTrue(p4 == Position(-90.0, 0.0, 90.0));
			Assert::IsTrue(p1 == Position(100.0, 50.0, 10.0));
			Assert::IsTrue(p2 == Position(10.0, 50.0, 100.0));

			Position p5(10.0, 20.0, 30.0);
			p5 += p1;
			Assert::IsTrue(p5 == Position(110.0, 70.0, 40.0));
			Assert::IsTrue(p1 == Position(100.0, 50.0, 10.0));

			Position p6(20.0, 30.0, 50.0);
			p6 -= p1;
			Assert::IsTrue(p6 == Position(-80.0, -20.0, 40.0));
			Assert::IsTrue(p1 == Position(100.0, 50.0, 10.0));

			Position p7 = p1 * 1.5;
			Assert::IsTrue(p7 == Position(150.0, 75.0, 15.0));
			Assert::IsTrue(p1 == Position(100.0, 50.0, 10.0));

			Position p8(1.0, 2.0, 3.0);
			Position p9(-1.0, 2.0, -3.0);
			p8 -= p9;
			Assert::IsTrue(p8 == Position(2.0, 0.0, 6.0));
			Assert::IsTrue(p9 == Position(-1.0, 2.0, -3.0));

			Assert::IsTrue(p1.getX() == 100.0);
			Assert::IsTrue(p1.getY() == 50.0);
			Assert::IsTrue(p1.getZ() == 10.0);

			Position p10(3.0, 4.0, 0.0);
			Assert::IsTrue(p10.getNormSq() == 25.0);
			Assert::IsTrue(p10.getNorm() == 5.0);
		}
	};
	TEST_CLASS(RectangleTest)
	{
	public:
		TEST_METHOD(IntersectTest) {
			Cuboid rec = Cuboid(Position(0.0, 0.0, 0.0), Position(3.0, 4.0, 12.0));

			LineSegment line1 = LineSegment(Position(-1.0, -1.0, -1.0), Position(1.0, 1.0, 1.0));
			Assert::IsTrue(rec.getLineSegmentIntersect(line1).has_value());
			Assert::IsTrue(rec.getLineSegmentIntersect(line1).value() == Position(0.0, 0.0, 0.0));


			LineSegment line2 = LineSegment(Position(-2.0, -2.0, -2.0), Position(-1.0, -1.0, -1.0));
			Assert::IsFalse(rec.getLineSegmentIntersect(line2).has_value());
			Assert::IsFalse(rec.getLineSegmentIntersect(line2).has_value());

			LineSegment line3 = LineSegment(Position(-1.0, -1.0, -1.0), Position(0.0, 0.0, 0.0));
			Assert::IsTrue(rec.getLineSegmentIntersect(line3).has_value());
			Assert::IsTrue(rec.getLineSegmentIntersect(line3).value() == Position(0.0, 0.0, 0.0));

		}
	};
	TEST_CLASS(FlightPathTest) {
	public:
		TEST_METHOD(SimpleThrowTest) {
			FlightPath path(Position(0, 0, 0.5), Position(120, 0, 0), 1, 40);
			Position pos(0, 0, 0);
			GeneralTimer generalTimer;
			Time currentTime(generalTimer.updateTime());
			generalTimer.setTempo(7.5);

			while (pos.getZ() >= 0) {
				Time timeDiff = generalTimer.getTime() - currentTime;
				currentTime = generalTimer.getTime();
				pos = path.updatePosition(timeDiff);
				Assert::IsTrue(pos.getX() >= 0);
				Sleep(2);
				generalTimer.updateTime();

			}
			Assert::IsTrue(pos.getX() >= 119 && pos.getX() <= 121);
			Assert::IsTrue(pos.getY() >= -0.1 && pos.getY() <= 0.1);
		}
		TEST_METHOD(SimpleThrowTest2) {
			FlightPath path(Position(0, 5, 0.5), Position(120, 5, 0), 1, 40);
			Position pos(0, 0, 0);
			GeneralTimer generalTimer;
			Time currentTime(generalTimer.updateTime());
			generalTimer.setTempo(7.5);

			while (pos.getZ() >= 0) {
				Time timeDiff = generalTimer.getTime() - currentTime;
				currentTime = generalTimer.getTime();
				pos = path.updatePosition(timeDiff);
				Assert::IsTrue(pos.getX() >= 0);
				Sleep(2);
				generalTimer.updateTime();

			}
			Assert::IsTrue(pos.getX() >= 119 && pos.getX() <= 121);
			Assert::IsTrue(pos.getY() >= 4.9 && pos.getY() <= 5.1);
		}
		TEST_METHOD(SimpleThrowTest3) {
			FlightPath path(Position(120, 5, 0.5), Position(0, 5, 0), 1, 40);
			Position pos(0, 0, 0);
			GeneralTimer generalTimer;
			Time currentTime(generalTimer.updateTime());
			generalTimer.setTempo(7.5);

			while (pos.getZ() >= 0) {
				Time timeDiff = generalTimer.getTime() - currentTime;
				currentTime = generalTimer.getTime();
				pos = path.updatePosition(timeDiff);
				Sleep(2);
				generalTimer.updateTime();

			}
			Assert::IsTrue(pos.getX() >= -1 && pos.getX() <= 1);
			Assert::IsTrue(pos.getY() >= 4.9 && pos.getY() <= 5.1);
		}
		TEST_METHOD(DiagonalThrowTest) {
			FlightPath path(Position(0, 0, 0.5), Position(120, 120, 0), 1, 50);
			Position pos(0,0,0);
			while (pos.getZ() >= 0) {
				Time timeDiff(2);
				pos = path.updatePosition(timeDiff);
				Assert::IsTrue(pos.getX() >= 0);
				Assert::IsTrue(pos.getY() >= 0);
			}
			Assert::IsTrue(path.willReachTarget());
			Assert::IsTrue(pos.getX() >= 119.9 && pos.getX() <= 121.1);
			Assert::IsTrue(pos.getY() >= 119.9 && pos.getY() <= 121.1);
		}

		TEST_METHOD(UnreachableThrowTest) {
			FlightPath path(Position(0, 0, 0.5), Position(120, 120, 0), 1, 10);
			Position pos(0, 0, 0);

			while (pos.getZ() >= 0) {
				Time timeDiff(10);
				pos = path.updatePosition(timeDiff);
				Assert::IsTrue(pos.getX() >= 0);
			}
			Assert::IsFalse(path.willReachTarget());
		}
	};
}
