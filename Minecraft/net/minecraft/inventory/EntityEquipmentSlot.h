#pragma once
#include <string>
#include <vector>

enum class EquipmentType
{
    HAND,
    ARMOR
};

class EntityEquipmentSlot
{
  public:
    static EntityEquipmentSlot MAINHAND;
    static EntityEquipmentSlot OFFHAND;
    static EntityEquipmentSlot FEET;
    static EntityEquipmentSlot LEGS;
    static EntityEquipmentSlot CHEST;
    static EntityEquipmentSlot HEAD;
    EntityEquipmentSlot(EquipmentType slotTypeIn, int32_t indexIn, int32_t slotIndexIn, std::string_view nameIn);
    static EntityEquipmentSlot fromString(std::string_view targetName);
    friend auto operator==(const EntityEquipmentSlot &lhs, const EntityEquipmentSlot &rhs) -> bool;
    friend auto operator!=(const EntityEquipmentSlot &lhs, const EntityEquipmentSlot &rhs) -> bool;

    EquipmentType getSlotType() const;
    int32_t getIndex() const;
    int32_t getSlotIndex() const;
    std::string getName() const;

    static std::vector<EntityEquipmentSlot *> &values();

  private:
    EquipmentType slotType;
    int32_t index;
    int32_t slotIndex;
    std::string name;

    static std::vector<EntityEquipmentSlot *> value;
};
