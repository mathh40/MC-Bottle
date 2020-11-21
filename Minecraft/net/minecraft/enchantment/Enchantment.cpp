#include "Enchantment.h"



#include "EnchantmentArrowDamage.h"
#include "EnchantmentArrowFire.h"
#include "EnchantmentArrowInfinite.h"
#include "EnchantmentArrowKnockback.h"
#include "EnchantmentBindingCurse.h"
#include "EnchantmentDamage.h"
#include "EnchantmentDigging.h"
#include "EnchantmentDurability.h"
#include "EnchantmentFireAspect.h"
#include "EnchantmentFishingSpeed.h"
#include "EnchantmentFrostWalker.h"
#include "EnchantmentKnockback.h"
#include "EnchantmentLootBonus.h"
#include "EnchantmentMending.h"
#include "EnchantmentOxygen.h"
#include "EnchantmentProtection.h"
#include "EnchantmentSweepingEdge.h"
#include "EnchantmentThorns.h"
#include "EnchantmentUntouching.h"
#include "EnchantmentVanishingCurse.h"
#include "EnchantmentWaterWalker.h"
#include "EnchantmentWaterWorker.h"
#include "ResourceLocation.h"
#include "../entity/EntityLivingBase.h"
#include "../item/ItemStack.h"
#include "text/translation/I18n.h"

Enchantment *Enchantment::getEnchantmentByID(int32_t id) {
    return REGISTRY.getObjectById(id).value();
}

int32_t Enchantment::getEnchantmentID(Enchantment *enchantmentIn) {
    return REGISTRY.getIDForObject(enchantmentIn);
}

Enchantment * Enchantment::getEnchantmentByLocation(std::string_view location) {
    return REGISTRY.getObject(ResourceLocation(location)).value();
}

std::vector<ItemStack> & Enchantment::getEntityEquipment(EntityLivingBase *entityIn) const{
    std::vector<ItemStack> list;

    for (auto entityequipmentslot : applicableEquipmentTypes) {
         ItemStack itemstack = entityIn->getItemStackFromSlot(entityequipmentslot);
         if (!itemstack.isEmpty()) {
            list.emplace_back(itemstack);
         }
      }

      return list;
}

Enchantment::Rarity Enchantment::getRarity() const {
    return rarity;
}

int32_t Enchantment::getMinLevel() const{
    return 1;
}

int32_t Enchantment::getMaxLevel() const{
    return 1;
}

int32_t Enchantment::getMinEnchantability(int32_t enchantmentLevel) const{
    return 1 + enchantmentLevel * 10;
}

int32_t Enchantment::getMaxEnchantability(int32_t enchantmentLevel) const{
    return getMinEnchantability(enchantmentLevel) + 5;
}

int32_t Enchantment::calcModifierDamage(int32_t level, const DamageSource::DamageSource &source) const{
    return 0;
}

float Enchantment::calcDamageByCreature(int32_t level, const EnumCreatureAttribute &creatureType) const{
    return 0.0F;
}

bool Enchantment::isCompatibleWith(const Enchantment &p_191560_1_) const{
    return canApplyTogether(p_191560_1_) && p_191560_1_.canApplyTogether(this);
}

Enchantment & Enchantment::setName(std::string_view enchName) {
    name = enchName;
    return *this;
}

std::string Enchantment::getName() const {
    return "enchantment." + name;
}

std::string Enchantment::getTranslatedName(int32_t level) const{
    auto s = I18n::translateToLocal(getName());
      if (isCurse()) {
         s = TextFormatting::RED + s;
      }

      return level == 1 && getMaxLevel() == 1 ? s : s + " " + I18n::translateToLocal("enchantment.level." + level);
}

bool Enchantment::canApply(const ItemStack &stack) const{
    return type.canEnchantItem(stack.getItem());
}

void Enchantment::onEntityDamaged(EntityLivingBase *user, Entity *target, int32_t level) const{
}

void Enchantment::onUserHurt(EntityLivingBase *user, Entity *attacker, int32_t level) const{
}

bool Enchantment::isTreasureEnchantment() const{
    return false;
}

bool Enchantment::isCurse() const{
    return false;
}

void Enchantment::registerEnchantments() {
    const std::array<EntityEquipmentSlot,4> aentityequipmentslot = {EntityEquipmentSlot::HEAD, EntityEquipmentSlot::CHEST, EntityEquipmentSlot::LEGS, EntityEquipmentSlot::FEET};
    const std::array<EntityEquipmentSlot,1> headequipmentslot = {EntityEquipmentSlot::HEAD};
    const std::array<EntityEquipmentSlot,1> chestequipmentslot = {EntityEquipmentSlot::CHEST};
    const std::array<EntityEquipmentSlot,1> legsequipmentslot = {EntityEquipmentSlot::LEGS};
    const std::array<EntityEquipmentSlot,1> feetequipmentslot = {EntityEquipmentSlot::FEET};
    const std::array<EntityEquipmentSlot,1> mainhandequipmentslot = {EntityEquipmentSlot::MAINHAND};

      REGISTRY.registe(0, ResourceLocation("protection"), new EnchantmentProtection(Rarity::COMMON, EnchantmentProtection::Type::ALL, aentityequipmentslot));
      REGISTRY.registe(1, ResourceLocation("fire_protection"), new EnchantmentProtection(Rarity::UNCOMMON, EnchantmentProtection::Type::FIRE, aentityequipmentslot));
      REGISTRY.registe(2, ResourceLocation("feather_falling"), new EnchantmentProtection(Rarity::UNCOMMON, EnchantmentProtection::Type::FALL, aentityequipmentslot));
      REGISTRY.registe(3, ResourceLocation("blast_protection"), new EnchantmentProtection(Rarity::RARE, EnchantmentProtection::Type::EXPLOSION, aentityequipmentslot));
      REGISTRY.registe(4, ResourceLocation("projectile_protection"), new EnchantmentProtection(Rarity::UNCOMMON, EnchantmentProtection::Type::PROJECTILE, aentityequipmentslot));
      REGISTRY.registe(5, ResourceLocation("respiration"), new EnchantmentOxygen(Rarity::RARE, aentityequipmentslot));
      REGISTRY.registe(6, ResourceLocation("aqua_affinity"), new EnchantmentWaterWorker(Rarity::RARE, aentityequipmentslot));
      REGISTRY.registe(7, ResourceLocation("thorns"), new EnchantmentThorns(Rarity::VERY_RARE, aentityequipmentslot));
      REGISTRY.registe(8, ResourceLocation("depth_strider"), new EnchantmentWaterWalker(Rarity::RARE, aentityequipmentslot));
      REGISTRY.registe(9, ResourceLocation("frost_walker"), new EnchantmentFrostWalker(Rarity::RARE, feetequipmentslot));
      REGISTRY.registe(10, ResourceLocation("binding_curse"), new EnchantmentBindingCurse(Rarity::VERY_RARE, aentityequipmentslot));
      REGISTRY.registe(16, ResourceLocation("sharpness"), new EnchantmentDamage(Rarity::COMMON, 0, mainhandequipmentslot));
      REGISTRY.registe(17, ResourceLocation("smite"), new EnchantmentDamage(Rarity::UNCOMMON, 1, mainhandequipmentslot));
      REGISTRY.registe(18, ResourceLocation("bane_of_arthropods"), new EnchantmentDamage(Rarity::UNCOMMON, 2, mainhandequipmentslot));
      REGISTRY.registe(19, ResourceLocation("knockback"), new EnchantmentKnockback(Rarity::UNCOMMON, mainhandequipmentslot));
      REGISTRY.registe(20, ResourceLocation("fire_aspect"), new EnchantmentFireAspect(Rarity::RARE, mainhandequipmentslot));
      REGISTRY.registe(21, ResourceLocation("looting"), new EnchantmentLootBonus(Rarity::RARE, EnumEnchantmentType::WEAPON, mainhandequipmentslot));
      REGISTRY.registe(22, ResourceLocation("sweeping"), new EnchantmentSweepingEdge(Rarity::RARE, mainhandequipmentslot));
      REGISTRY.registe(32, ResourceLocation("efficiency"), new EnchantmentDigging(Rarity::COMMON, mainhandequipmentslot));
      REGISTRY.registe(33, ResourceLocation("silk_touch"), new EnchantmentUntouching(Rarity::VERY_RARE, mainhandequipmentslot));
      REGISTRY.registe(34, ResourceLocation("unbreaking"), new EnchantmentDurability(Rarity::UNCOMMON, mainhandequipmentslot));
      REGISTRY.registe(35, ResourceLocation("fortune"), new EnchantmentLootBonus(Rarity::RARE, EnumEnchantmentType::DIGGER, mainhandequipmentslot));
      REGISTRY.registe(48, ResourceLocation("power"), new EnchantmentArrowDamage(Rarity::COMMON,mainhandequipmentslot));
      REGISTRY.registe(49, ResourceLocation("punch"), new EnchantmentArrowKnockback(Rarity::RARE, mainhandequipmentslot));
      REGISTRY.registe(50, ResourceLocation("flame"), new EnchantmentArrowFire(Rarity::RARE, mainhandequipmentslot));
      REGISTRY.registe(51, ResourceLocation("infinity"), new EnchantmentArrowInfinite(Rarity::VERY_RARE, mainhandequipmentslot));
      REGISTRY.registe(61, ResourceLocation("luck_of_the_sea"), new EnchantmentLootBonus(Rarity::RARE, EnumEnchantmentType::FISHING_ROD, mainhandequipmentslot));
      REGISTRY.registe(62, ResourceLocation("lure"), new EnchantmentFishingSpeed(Rarity::RARE, EnumEnchantmentType::FISHING_ROD, mainhandequipmentslot));
      REGISTRY.registe(70, ResourceLocation("mending"), new EnchantmentMending(Rarity::RARE, EntityEquipmentSlot::values()));
      REGISTRY.registe(71, ResourceLocation("vanishing_curse"), new EnchantmentVanishingCurse(Rarity::VERY_RARE, EntityEquipmentSlot::values()));
}

Enchantment::Enchantment(Rarity rarityIn, EnumEnchantmentType typeIn,
                         std::span<const EntityEquipmentSlot> slots)
        :rarity(rarityIn),type(typeIn),applicableEquipmentTypes(slots.begin(),slots.end()){
}

bool Enchantment::canApplyTogether(const Enchantment &ench) const{
    return *this != ench;
}

bool operator==(const Enchantment &lhs, const Enchantment &rhs) {
    return lhs.getName() == rhs.getName();
}

bool operator!=(const Enchantment &lhs, const Enchantment &rhs) {
    return !(lhs == rhs);
}
