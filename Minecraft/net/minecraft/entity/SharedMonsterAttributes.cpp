#include "SharedMonsterAttributes.h"

#include "ai/attributes/AbstractAttributeMap.h"
#include "ai/attributes/IAttributeInstance.h"
#include "ai/attributes/RangedAttribute.h"

std::shared_ptr<spdlog::logger> SharedMonsterAttributes::LOGGER = spdlog::get("Minecraft")->clone("SharedMonsterAttributes");

std::unique_ptr<IAttribute> SharedMonsterAttributes::MAX_HEALTH = (std::make_unique<RangedAttribute>(nullptr, "generic.maxHealth", 20.0, 0.0, 1024.0)).setDescription("Max Health").setShouldWatch(true);
std::unique_ptr<IAttribute> SharedMonsterAttributes::FOLLOW_RANGE = (std::make_unique<RangedAttribute>(nullptr, "generic.followRange", 32.0, 0.0, 2048.0)).setDescription("Follow Range");
std::unique_ptr<IAttribute> SharedMonsterAttributes::KNOCKBACK_RESISTANCE = std::make_unique<RangedAttribute>(nullptr, "generic.knockbackResistance", 0.0, 0.0, 1.0)).setDescription("Knockback Resistance");
std::unique_ptr<IAttribute> SharedMonsterAttributes::MOVEMENT_SPEED = std::make_unique<RangedAttribute>(nullptr, "generic.movementSpeed", 0.699999988079071, 0.0, 1024.0)).setDescription("Movement Speed").setShouldWatch(true);
std::unique_ptr<IAttribute> SharedMonsterAttributes::FLYING_SPEED = std::make_unique<RangedAttribute>(nullptr, "generic.flyingSpeed", 0.4000000059604645, 0.0, 1024.0)).setDescription("Flying Speed").setShouldWatch(true);
std::unique_ptr<IAttribute> SharedMonsterAttributes::ATTACK_DAMAGE = std::make_unique<RangedAttribute>(nullptr, "generic.attackDamage", 2.0, 0.0, 2048.0);
std::unique_ptr<IAttribute> SharedMonsterAttributes::ATTACK_SPEED = std::make_unique<RangedAttribute>(nullptr, "generic.attackSpeed", 4.0, 0.0, 1024.0)).setShouldWatch(true);
std::unique_ptr<IAttribute> SharedMonsterAttributes::ARMOR = std::make_unique<RangedAttribute>(nullptr, "generic.armor", 0.0, 0.0, 30.0)).setShouldWatch(true);
std::unique_ptr<IAttribute> SharedMonsterAttributes::ARMOR_TOUGHNESS = std::make_unique<RangedAttribute>(nullptr, "generic.armorToughness", 0.0, 0.0, 20.0)).setShouldWatch(true);
std::unique_ptr<IAttribute> SharedMonsterAttributes::LUCK = std::make_unique<RangedAttribute>(nullptr, "generic.luck", 0.0, -1024.0, 1024.0)).setShouldWatch(true);

std::unique_ptr<NBTTagList> SharedMonsterAttributes::writeBaseAttributeMapToNBT(AbstractAttributeMap* map) {
     auto nbttaglist = std::make_unique<NBTTagList>();

    for(auto iattributeinstance : map->getAllAttributes()){
         nbttaglist->appendTag(writeAttributeInstanceToNBT(iattributeinstance));
      }

      return nbttaglist;
}

std::unique_ptr<NBTTagCompound> SharedMonsterAttributes::writeAttributeModifierToNBT(AttributeModifier modifier) {
    auto nbttagcompound = std::make_unique<NBTTagCompound>();
      nbttagcompound->setString("Name", modifier.getName());
      nbttagcompound->setDouble("Amount", modifier.getAmount());
      nbttagcompound->setInteger("Operation", modifier.getOperation());
      nbttagcompound->setUniqueId("UUID", modifier.getID());
      return nbttagcompound;
}

void SharedMonsterAttributes::setAttributeModifiers(AbstractAttributeMap* map, NBTTagList *list) {
    for(auto i = 0; i < list->tagCount(); ++i){
        auto nbttagcompound = list->getCompoundTagAt(i);
         IAttributeInstance* iattributeinstance = map->getAttributeInstanceByName(nbttagcompound->getString("Name"));
         if (iattributeinstance == nullptr) {
            LOGGER->warn("Ignoring unknown attribute '{}'", nbttagcompound->getString("Name"));
         } else {
            applyModifiersToAttributeInstance(iattributeinstance, nbttagcompound.get());
         }
      }
}

std::optional<AttributeModifier> SharedMonsterAttributes::readAttributeModifierFromNBT(NBTTagCompound *compound) {
    auto uuid = compound->getUniqueId("UUID");

      try {
         return AttributeModifier(uuid, compound->getString("Name"), compound->getDouble("Amount"), compound->getInteger("Operation"));
      } catch (std::exception &var3) {
         LOGGER->warn("Unable to create attribute: {}", var3.what());
         return std::nullopt;
        }
}

std::unique_ptr<NBTTagCompound> SharedMonsterAttributes::writeAttributeInstanceToNBT(IAttributeInstance *instance) {
    auto nbttagcompound = std::make_unique<NBTTagCompound>();
    auto *const iattribute = instance->getAttribute();
      nbttagcompound->setString("Name", iattribute->getName());
      nbttagcompound->setDouble("Base", instance->getBaseValue());
      auto collection = instance->getModifiers();
      if (collection != nullptr && !collection.empty()) {
           auto nbttaglist = std::make_unique<NBTTagList>();

        for(auto attributemodifier : collection){
            if (attributemodifier.isSaved()) {
               nbttaglist->appendTag(writeAttributeModifierToNBT(attributemodifier));
            }
         }

         nbttagcompound->setTag("Modifiers", nbttaglist);
      }

      return nbttagcompound;
}

void SharedMonsterAttributes::
applyModifiersToAttributeInstance(IAttributeInstance *instance, NBTTagCompound *compound) {
    instance->setBaseValue(compound->getDouble("Base"));
      if (compound->hasKey("Modifiers", 9)) {
         NBTTagList nbttaglist = compound->getTagList("Modifiers", 10);

         for(auto i = 0; i < nbttaglist.tagCount(); ++i) {
            AttributeModifier attributemodifier = readAttributeModifierFromNBT(nbttaglist.getCompoundTagAt(i));
            if (attributemodifier != nullptr) {
               AttributeModifier attributemodifier1 = instance->getModifier(attributemodifier.getID());
               if (attributemodifier1 != nullptr) {
                  instance->removeModifier(attributemodifier1);
               }

               instance->applyModifier(attributemodifier);
            }
         }
      }
}
