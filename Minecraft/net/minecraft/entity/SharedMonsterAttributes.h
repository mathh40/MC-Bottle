#pragma once
#include "../../../../spdlog/include/spdlog/logger.h"
#include "../nbt/NBTTagList.h"
#include "ai/attributes/IAttribute.h"

#include <memory>
#include <optional>

class AttributeModifier;
class IAttributeInstance;
class AbstractAttributeMap;

class SharedMonsterAttributes {
public:
    static std::unique_ptr<IAttribute> MAX_HEALTH;
    static std::unique_ptr<IAttribute>FOLLOW_RANGE;
    static std::unique_ptr<IAttribute> KNOCKBACK_RESISTANCE;
    static std::unique_ptr<IAttribute> MOVEMENT_SPEED;
    static std::unique_ptr<IAttribute> FLYING_SPEED;
    static std::unique_ptr<IAttribute> ATTACK_DAMAGE;
    static std::unique_ptr<IAttribute> ATTACK_SPEED;
    static std::unique_ptr<IAttribute> ARMOR;
    static std::unique_ptr<IAttribute> ARMOR_TOUGHNESS;
    static std::unique_ptr<IAttribute> LUCK;

    static std::unique_ptr<NBTTagList> writeBaseAttributeMapToNBT(AbstractAttributeMap* map);
    static std::unique_ptr<NBTTagCompound> writeAttributeModifierToNBT(AttributeModifier modifier);
    static void setAttributeModifiers(AbstractAttributeMap* map, NBTTagList* list);
    static std::optional<AttributeModifier> readAttributeModifierFromNBT(NBTTagCompound* compound);


private:
    static std::unique_ptr<NBTTagCompound> writeAttributeInstanceToNBT(IAttributeInstance* instance);
    static void applyModifiersToAttributeInstance(IAttributeInstance* instance, NBTTagCompound* compound);


    static std::shared_ptr<spdlog::logger> LOGGER;

};
