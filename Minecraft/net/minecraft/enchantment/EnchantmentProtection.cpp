#include "EnchantmentProtection.h"

#include "EnchantmentHelper.h"
#include "math/MathHelper.h"

EnchantmentProtection::Type ALL = EnchantmentProtection::Type("all", 1, 11, 20);
EnchantmentProtection::Type FIRE = EnchantmentProtection::Type("fire", 10, 8, 12);
EnchantmentProtection::Type FALL = EnchantmentProtection::Type("fall", 5, 6, 10);
EnchantmentProtection::Type EXPLOSION = EnchantmentProtection::Type("explosion", 5, 8, 12);
EnchantmentProtection::Type PROJECTILE = EnchantmentProtection::Type("projectile", 3, 6, 15);

EnchantmentProtection::Type::Type(std::string_view name, int32_t minimal, int32_t perLevelEnchantability,
                                  int32_t p_i47051_6_):
    typeName(name), minEnchantability(minimal), levelCost(perLevelEnchantability), levelCostSpan(p_i47051_6_) {
}

std::string_view EnchantmentProtection::Type::getTypeName() const {
    return typeName;
}

int32_t EnchantmentProtection::Type::getMinimalEnchantability() const {
    return minEnchantability;
}

int32_t EnchantmentProtection::Type::getEnchantIncreasePerLevel() const {
    return levelCost;
}

EnchantmentProtection::EnchantmentProtection(Enchantment::Rarity rarityIn, EnchantmentProtection::Type protectionTypeIn,
                                             std::span<const EntityEquipmentSlot> slots):
    Enchantment(rarityIn, EnumEnchantmentType::ARMOR, slots), protectionType(protectionTypeIn) {

    if (protectionTypeIn == EnchantmentProtection::Type::FALL) {
        type = EnumEnchantmentType::ARMOR_FEET;
    }

}

int32_t EnchantmentProtection::getMinEnchantability(int32_t enchantmentLevel) const {
    return protectionType.getMinimalEnchantability() + (enchantmentLevel - 1) * protectionType.
           getEnchantIncreasePerLevel();
}

int32_t EnchantmentProtection::getMaxEnchantability(int32_t enchantmentLevel) const {
    return getMinEnchantability(enchantmentLevel) + protectionType.getEnchantIncreasePerLevel();
}

int32_t EnchantmentProtection::getMaxLevel() const {
    return 4;
}

int32_t EnchantmentProtection::calcModifierDamage(int32_t level, const DamageSource::DamageSource &source) const {
    if (source.canHarmInCreative()) {
        return 0;
    } else if (protectionType == EnchantmentProtection::Type::ALL) {
        return level;
    } else if (protectionType == EnchantmentProtection::Type::FIRE && source.isFireDamage()) {
        return level * 2;
    } else if (protectionType == EnchantmentProtection::Type::FALL && source == DamageSource::FALL) {
        return level * 3;
    } else if (protectionType == EnchantmentProtection::Type::EXPLOSION && source.isExplosion()) {
        return level * 2;
    } else {
        return protectionType == EnchantmentProtection::Type::PROJECTILE && source.isProjectile() ? level * 2 : 0;
    }
}

std::string EnchantmentProtection::getName() const {
    return "enchantment.protect." + std::string(protectionType.getTypeName());
}

bool EnchantmentProtection::canApplyTogether(const Enchantment &ench) const {
    const Enchantment *enchent = &ench;
    if (Util::instanceof<EnchantmentProtection>(enchent)) {
        EnchantmentProtection *enchantmentprotection = (EnchantmentProtection *)enchent;
        if (protectionType == enchantmentprotection->protectionType) {
            return false;
        } else {
            return protectionType == EnchantmentProtection::Type::FALL || enchantmentprotection->protectionType ==
                   EnchantmentProtection::Type::FALL;
        }
    } else {
        return Enchantment::canApplyTogether(ench);
    }
}

int32_t EnchantmentProtection::getFireTimeForEntity(EntityLivingBase *p_92093_0_, int32_t p_92093_1_) {
    int i = EnchantmentHelper::getMaxEnchantmentLevel(Enchantments::FIRE_PROTECTION, p_92093_0_);
    if (i > 0) {
        p_92093_1_ -= MathHelper::floor((float)p_92093_1_ * (float)i * 0.15F);
    }

    return p_92093_1_;
}

double EnchantmentProtection::getBlastDamageReduction(EntityLivingBase *entityLivingBaseIn, double damage) {
    int i = EnchantmentHelper::getMaxEnchantmentLevel(Enchantments::BLAST_PROTECTION, entityLivingBaseIn);
    if (i > 0) {
        damage -= (double)MathHelper::floor(damage * (double)((float)i * 0.15F));
    }

    return damage;
}

bool operator==(const EnchantmentProtection::Type &lhs, const EnchantmentProtection::Type &rhs) {
    return lhs.typeName == rhs.typeName;
}

bool operator!=(const EnchantmentProtection::Type &lhs, const EnchantmentProtection::Type &rhs) {
    return !(lhs == rhs);
}
