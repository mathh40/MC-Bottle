#include "EnchantmentHelper.h"

#include "../entity/EntityLivingBase.h"
#include "../item/ItemEnchantedBook.h"
#include "../item/ItemStack.h"
#include "../nbt/NBTTagList.h"
#include "Enchantment.h"
#include "EnchantmentData.h"

namespace EnchantmentHelper
{
ModifierDamage ENCHANTMENT_MODIFIER_DAMAGE;
ModifierLiving ENCHANTMENT_MODIFIER_LIVING;
HurtIterator ENCHANTMENT_ITERATOR_HURT;
DamageIterator ENCHANTMENT_ITERATOR_DAMAGE;

void applyEnchantmentModifier(IModifier *modifier, const ItemStack &stack)
{
    if (!stack.isEmpty())
    {
        NBTTagList *nbttaglist = stack.getEnchantmentTagList();

        for (auto i = 0; i < nbttaglist->tagCount(); ++i)
        {
            auto j = nbttaglist->getCompoundTagAt(i)->getShort("id");
            auto k = nbttaglist->getCompoundTagAt(i)->getShort("lvl");
            if (Enchantment::getEnchantmentByID(j) != nullptr)
            {
                modifier->calculateModifier(*Enchantment::getEnchantmentByID(j), k);
            }
        }
    }
}

void applyEnchantmentModifierArray(IModifier *modifier, std::span<ItemStack> stacks)
{
    for (auto itemstack : stacks)
    {
        applyEnchantmentModifier(modifier, itemstack);
    }
}

void ModifierLiving::calculateModifier(const Enchantment &enchantmentIn, int32_t enchantmentLevel)
{
    livingModifier += enchantmentIn.calcDamageByCreature(enchantmentLevel, entityLiving);
}

void ModifierDamage::calculateModifier(const Enchantment &enchantmentIn, int32_t enchantmentLevel)
{
    damageModifier += enchantmentIn.calcModifierDamage(enchantmentLevel, source.value());
}

void HurtIterator::calculateModifier(const Enchantment &enchantmentIn, int32_t enchantmentLevel)
{
    enchantmentIn.onUserHurt(user, attacker, enchantmentLevel);
}

void DamageIterator::calculateModifier(const Enchantment &enchantmentIn, int32_t enchantmentLevel)
{
    enchantmentIn.onEntityDamaged(user, target, enchantmentLevel);
}

int32_t getEnchantmentLevel(const Enchantment &enchID, const ItemStack &stack)
{
    if (stack.isEmpty())
    {
        return 0;
    }
    else
    {
        NBTTagList *nbttaglist = stack.getEnchantmentTagList();

        for (auto i = 0; i < nbttaglist->tagCount(); ++i)
        {
            const auto nbttagcompound = nbttaglist->getCompoundTagAt(i);
            const auto enchantment    = Enchantment::getEnchantmentByID(nbttagcompound->getShort("id"));
            int j                     = nbttagcompound->getShort("lvl");
            if (*enchantment == enchID)
            {
                return j;
            }
        }

        return 0;
    }
}

std::unordered_map<Enchantment *, int16_t> getEnchantments(const ItemStack &stack)
{
    std::unordered_map<Enchantment *, int16_t> map;
    NBTTagList *nbttaglist = stack.getItem() == Items::ENCHANTED_BOOK ? ItemEnchantedBook::getEnchantments(stack)
                                                                      : stack.getEnchantmentTagList();

    for (auto i = 0; i < nbttaglist->tagCount(); ++i)
    {
        NBTTagCompound *nbttagcompound = nbttaglist->getCompoundTagAt(i);
        Enchantment *enchantment       = Enchantment::getEnchantmentByID(nbttagcompound.getShort("id"));
        auto j                         = nbttagcompound->getShort("lvl");
        map.emplace(enchantment, j);
    }

    return map;
}

void setEnchantments(std::unordered_map<Enchantment *, int16_t> enchMap, ItemStack &stack)
{
    NBTTagList *nbttaglist = new NBTTagList();

    for (auto entry : enchMap)
    {
        auto enchantment = entry.first;
        if (enchantment != nullptr)
        {
            auto i                         = entry.second;
            NBTTagCompound *nbttagcompound = new NBTTagCompound();
            nbttagcompound->setShort("id", Enchantment::getEnchantmentID(enchantment));
            nbttagcompound->setShort("lvl", i);
            nbttaglist->appendTag(nbttagcompound);
            if (stack.getItem() == Items::ENCHANTED_BOOK)
            {
                ItemEnchantedBook::addEnchantment(stack, EnchantmentData(enchantment, i));
            }
        }
    }

    if (nbttaglist->isEmpty())
    {
        if (stack.hasTagCompound())
        {
            stack.getTagCompound()->removeTag("ench");
        }
    }
    else if (stack.getItem() != Items::ENCHANTED_BOOK)
    {
        stack.setTagInfo("ench", nbttaglist);
    }
}

int32_t getEnchantmentModifierDamage(std::span<ItemStack> stacks, DamageSource::DamageSource source)
{
    ENCHANTMENT_MODIFIER_DAMAGE.damageModifier = 0;
    ENCHANTMENT_MODIFIER_DAMAGE.source         = source;
    applyEnchantmentModifierArray(&ENCHANTMENT_MODIFIER_DAMAGE, stacks);
    return ENCHANTMENT_MODIFIER_DAMAGE.damageModifier;
}

float getModifierForCreature(const ItemStack &stack, const EnumCreatureAttribute &creatureAttribute)
{
    ENCHANTMENT_MODIFIER_LIVING.livingModifier = 0.0F;
    ENCHANTMENT_MODIFIER_LIVING.entityLiving   = creatureAttribute;
    applyEnchantmentModifier(&ENCHANTMENT_MODIFIER_LIVING, stack);
    return ENCHANTMENT_MODIFIER_LIVING.livingModifier;
}

float getSweepingDamageRatio(EntityLivingBase *p_191527_0_)
{
    int32_t i = getMaxEnchantmentLevel(Enchantments::SWEEPING, p_191527_0_);
    return i > 0 ? EnchantmentSweepingEdge::getSweepingDamageRatio(i) : 0.0F;
}

void applyThornEnchantments(EntityLivingBase *p_151384_0_, Entity *p_151384_1_)
{
    ENCHANTMENT_ITERATOR_HURT.attacker = p_151384_1_;
    ENCHANTMENT_ITERATOR_HURT.user     = p_151384_0_;
    if (p_151384_0_ != nullptr)
    {
        applyEnchantmentModifierArray(&ENCHANTMENT_ITERATOR_HURT, p_151384_0_->getEquipmentAndArmor());
    }

    if (Util:: instanceof <EntityPlayer>(p_151384_1_))
    {
        applyEnchantmentModifier(&ENCHANTMENT_ITERATOR_HURT, p_151384_0_->getHeldItemMainhand());
    }
}

void applyArthropodEnchantments(EntityLivingBase *p_151385_0_, Entity *p_151385_1_)
{
    ENCHANTMENT_ITERATOR_DAMAGE.user   = p_151385_0_;
    ENCHANTMENT_ITERATOR_DAMAGE.target = p_151385_1_;
    if (p_151385_0_ != nullptr)
    {
        applyEnchantmentModifierArray(&ENCHANTMENT_ITERATOR_DAMAGE, p_151385_0_->getEquipmentAndArmor());
    }

    if (Util:: instanceof <EntityPlayer>(p_151385_0_))
    {
        applyEnchantmentModifier(&ENCHANTMENT_ITERATOR_DAMAGE, p_151385_0_->getHeldItemMainhand());
    }
}

int32_t getMaxEnchantmentLevel(const Enchantment &p_185284_0_, EntityLivingBase *p_185284_1_)
{
    auto iterable = p_185284_0_.getEntityEquipment(p_185284_1_);
    if (iterable == nullptr)
    {
        return 0;
    }
    else
    {
        int32_t i = 0;

        for (auto itemstack : iterable)
        {
            int32_t j = getEnchantmentLevel(p_185284_0_, itemstack);
            if (j > i)
            {
                i = j;
            }
        }

        return i;
    }
}

int32_t getKnockbackModifier(EntityLivingBase *player)
{
    return getMaxEnchantmentLevel(Enchantments::KNOCKBACK, player);
}

int32_t getFireAspectModifier(EntityLivingBase *player)
{
    return getMaxEnchantmentLevel(Enchantments::FIRE_ASPECT, player);
}

int32_t getRespirationModifier(EntityLivingBase *p_185292_0_)
{
    return getMaxEnchantmentLevel(Enchantments::RESPIRATION, p_185292_0_);
}

int32_t getDepthStriderModifier(EntityLivingBase *p_185294_0_)
{
    return getMaxEnchantmentLevel(Enchantments::DEPTH_STRIDER, p_185294_0_);
}

int32_t getEfficiencyModifier(EntityLivingBase *p_185293_0_)
{
    return getMaxEnchantmentLevel(Enchantments::EFFICIENCY, p_185293_0_);
}

int32_t getFishingLuckBonus(const ItemStack &p_191529_0_)
{
    return getEnchantmentLevel(Enchantments::LUCK_OF_THE_SEA, p_191529_0_);
}

int32_t getFishingSpeedBonus(const ItemStack &p_191528_0_)
{
    return getEnchantmentLevel(Enchantments::LURE, p_191528_0_);
}

int32_t getLootingModifier(EntityLivingBase *p_185283_0_)
{
    return getMaxEnchantmentLevel(Enchantments::LOOTING, p_185283_0_);
}

bool getAquaAffinityModifier(EntityLivingBase *p_185287_0_)
{
    return getMaxEnchantmentLevel(Enchantments::AQUA_AFFINITY, p_185287_0_) > 0;
}

bool hasFrostWalkerEnchantment(EntityLivingBase *player)
{
    return getMaxEnchantmentLevel(Enchantments::FROST_WALKER, player) > 0;
}

bool hasBindingCurse(const ItemStack &p_190938_0_)
{
    return getEnchantmentLevel(Enchantments::BINDING_CURSE, p_190938_0_) > 0;
}

bool hasVanishingCurse(const ItemStack &p_190939_0_)
{
    return getEnchantmentLevel(Enchantments::VANISHING_CURSE, p_190939_0_) > 0;
}

ItemStack getEnchantedItem(const Enchantment &p_92099_0_, EntityLivingBase *p_92099_1_)
{
    auto list = p_92099_0_.getEntityEquipment(p_92099_1_);
    if (list.empty())
    {
        return ItemStack::EMPTY;
    }
    else
    {
        std::vector<ItemStack> list1;

        for (auto itemstack : list)
        {
            if (!itemstack.isEmpty() && getEnchantmentLevel(p_92099_0_, itemstack) > 0)
            {
                list1.emplace_back(itemstack);
            }
        }

        return list1.empty() ? ItemStack::EMPTY : list1[p_92099_1_->getRNG()(list1.size())];
    }
}

int32_t calcItemStackEnchantability(pcg32 &rand, int32_t enchantNum, int32_t power, ItemStack stack)
{
    Item *item = stack.getItem();
    auto i     = item->getItemEnchantability();
    if (i <= 0)
    {
        return 0;
    }
    else
    {
        if (power > 15)
        {
            power = 15;
        }

        int32_t j = rand(8) + 1 + (power >> 1) + rand(power + 1);
        if (enchantNum == 0)
        {
            return MathHelper::max(j / 3, 1);
        }
        else
        {
            return enchantNum == 1 ? j * 2 / 3 + 1 : MathHelper::max(j, power * 2);
        }
    }
}

ItemStack addRandomEnchantment(pcg32 &random, ItemStack &stack, int32_t level, bool allowTreasure)
{
    auto list = buildEnchantmentList(random, stack, level, allowTreasure);
    bool flag = stack.getItem() == Items::BOOK;
    if (flag)
    {
        stack = ItemStack(Items::ENCHANTED_BOOK);
    }

    for (auto enchantmentdata : list)
    {
        if (flag)
        {
            ItemEnchantedBook::addEnchantment(stack, enchantmentdata);
        }
        else
        {
            stack.addEnchantment(enchantmentdata.enchantment, enchantmentdata.enchantmentLevel);
        }
    }

    return stack;
}

std::vector<EnchantmenRandom::EnchantmentData> buildEnchantmentList(pcg32 &randomIn, ItemStack &itemStackIn,
                                                                    int32_t level, bool allowTreasure)
{
    std::vector<EnchantmenRandom::EnchantmentData> list;
    Item *item = itemStackIn.getItem();
    auto i     = item->getItemEnchantability();
    if (i <= 0)
    {
        return list;
    }
    else
    {
        level   = level + 1 + randomIn(i / 4 + 1) + randomIn(i / 4 + 1);
        float f = (MathHelper::nextFloat(randomIn) + MathHelper::nextFloat(randomIn) - 1.0F) * 0.15F;
        level =
            MathHelper::clamp<int32_t>(MathHelper::round(level + level * f), 1, std::numeric_limits<int32_t>::max());
        auto list1 = getEnchantmentDatas(level, itemStackIn, allowTreasure);
        if (!list1.empty())
        {
            list.emplace_back(EnchantmenRandom::getRandomItem(randomIn, list1));

            while (randomIn(50) <= level)
            {
                removeIncompatible(list1, list.back());
                if (list1.empty())
                {
                    break;
                }

                list.emplace_back(EnchantmenRandom::getRandomItem(randomIn, list1));
                level /= 2;
            }
        }

        return list;
    }
}

void removeIncompatible(std::vector<EnchantmenRandom::EnchantmentData> &p_185282_0_,
                        const EnchantmenRandom::EnchantmentData &p_185282_1_)
{

    for (auto enchdata : p_185282_0_)
    {
        if (!p_185282_1_.enchantment.isCompatibleWith(enchdata.enchantment))
        {
            p_185282_0_.erase(enchdata);
        }
    }
}

std::vector<EnchantmenRandom::EnchantmentData> getEnchantmentDatas(int32_t p_185291_0_, ItemStack &p_185291_1_,
                                                                   bool allowTreasure)
{
    std::vector<EnchantmenRandom::EnchantmentData> list;
    Item *item = p_185291_1_.getItem();
    bool flag  = p_185291_1_.getItem() == Items::BOOK;
    auto var6  = Enchantment::REGISTRY.begin();

    while (true)
    {
        while (true)
        {
            Enchantment *enchantment;
            do
            {
                do
                {
                    if (var6 == Enchantment::REGISTRY.end())
                    {
                        return list;
                    }

                    enchantment = var6->second;
                } while (enchantment->isTreasureEnchantment() && !allowTreasure);
            } while (!enchantment->type.canEnchantItem(item) && !flag);

            for (auto i = enchantment->getMaxLevel(); i > enchantment->getMinLevel() - 1; --i)
            {
                if (p_185291_0_ >= enchantment->getMinEnchantability(i) &&
                    p_185291_0_ <= enchantment->getMaxEnchantability(i))
                {
                    list.emplace_back(EnchantmenRandom::EnchantmentData(enchantment, i));
                    break;
                }
            }
        }
    }
}
} // namespace EnchantmentHelper
