#pragma once
#include "AbstractCharacter.h"

class AbstractItem;
class Food;

class Player :
    public AbstractCharacter
{
public:
    /// Starts fully equipped: sword, bow, shield, armor, a quiver of arrows and food.
    Player();
    virtual void updateBehaviour(Time timeDiff);
    shared_ptr<AbstractWeapon> getSelectedWeapon() const override;
    shared_ptr<Shield> getSelectedShield() const override;
    shared_ptr<Armor> getSelectedArmor() const override;
    shared_ptr<RangedWeapon> getSelectedBow() const override;
    /// Primary quiver stack first, then the secondary one.
    shared_ptr<Arrow> peekArrow() const override;
    shared_ptr<Arrow> takeArrow() override;
    /// Number of arrows in both quiver stacks.
    int getArrowCount() const;

    /// Puts an item into the inventory: arrows go to a matching quiver stack first,
    /// anything else (or arrows when the quiver is full) to the first free backpack space.
    /// @return false if there was no room.
    bool storeItem(shared_ptr<AbstractItem> item);
    /// Same check as storeItem() without changing the inventory.
    bool canStoreItem(shared_ptr<AbstractItem> item);
    /// Picks up arrows lying within CombatRules::arrowPickupRadius. @return how many.
    int pickUpArrows();

    /// Starts eating the food in satchel cell @p index (0-3, row by row). Takes
    /// CombatRules::eatTimeMs, then the food is used up and its effects (healing) start.
    /// @return false if there is no food there or the hands are busy.
    bool eatFromSatchel(int index);
    /// Eats the first food found in the satchel.
    bool eatAnyFood();
    bool isEating() const;
    /// 0..1 progress of the current meal.
    double getEatProgress() const;
    int getFoodCount() const;

    void updateAttack(Time timeDiff) override;

protected:
    bool isOccupied() const override;
    void onInterrupted() override;

private:
    bool placeItem(shared_ptr<AbstractItem> item, bool insert);
    struct EatingInfo {
        int x = 0, y = 0;
        shared_ptr<Food> food;
        Time left;
    };
    std::optional<EatingInfo> eating;
};
