#include "EntityEquipmentSlot.h"

#include <stdexcept>

EntityEquipmentSlot EntityEquipmentSlot::MAINHAND(EquipmentType::HAND, 0, 0, "mainhand");
EntityEquipmentSlot EntityEquipmentSlot::OFFHAND(EquipmentType::HAND, 1, 5, "offhand");
EntityEquipmentSlot EntityEquipmentSlot::FEET(EquipmentType::ARMOR, 0, 1, "feet");
EntityEquipmentSlot EntityEquipmentSlot::LEGS(EquipmentType::ARMOR, 1, 2, "legs");
EntityEquipmentSlot EntityEquipmentSlot::CHEST(EquipmentType::ARMOR, 2, 3, "chest");
EntityEquipmentSlot EntityEquipmentSlot::HEAD(EquipmentType::ARMOR, 3, 4, "head");

EntityEquipmentSlot::EntityEquipmentSlot(EquipmentType slotTypeIn, int32_t indexIn, int32_t slotIndexIn,
    std::string_view nameIn)
        :slotType(slotTypeIn),index(indexIn),slotIndex(slotIndexIn),name(nameIn)
{
    value.emplace_back(this);
}

EntityEquipmentSlot EntityEquipmentSlot::fromString(std::string_view targetName)
{
    for(auto& entityequipmentslot : values()) 
    {
        if (entityequipmentslot->getName() == targetName) 
        {
            return *entityequipmentslot;
        }
    }

    throw std::logic_error("Invalid slot '" + targetName + "'");
}

EquipmentType EntityEquipmentSlot::getSlotType() const
{
    return slotType;
}

int32_t EntityEquipmentSlot::getIndex() const
{
    return index;
}

int32_t EntityEquipmentSlot::getSlotIndex() const
{
    return slotIndex;
}

std::string EntityEquipmentSlot::getName() const
{
    return name;
}

std::vector<EntityEquipmentSlot *> & EntityEquipmentSlot::values()
{
    return value;
}
