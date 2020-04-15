#pragma once
#include "Item.h"
#include "../potion/PotionEffect.h"

class ItemFood :public Item
{
public:

    ItemFood(int32_t amount, float saturation, bool isWolfFood);
    ItemFood(int32_t amount, bool isWolfFood);
    ItemStack onItemUseFinish(ItemStack stack, World* worldIn, EntityLivingBase* entityLiving) override;
    int32_t getMaxItemUseDuration(ItemStack stack) override;
    EnumAction getItemUseAction(ItemStack stack) override;
    ActionResult onItemRightClick(World* worldIn, EntityPlayer* playerIn, EnumHand handIn) override;
    virtual int32_t getHealAmount(ItemStack stack) const;
    virtual float getSaturationModifier(ItemStack stack) const;
    bool isWolfsFavoriteMeat();
    ItemFood* setPotionEffect(PotionEffect effect, float probability);
    ItemFood* setAlwaysEdible();


    int32_t itemUseDuration;
protected:
    virtual void onFoodEaten(ItemStack stack, World* worldIn, EntityPlayer* player) const;
private:
    int healAmount;
    float saturationModifier;
    bool isWolfsFavoriteMeat;
    bool alwaysEdible;
    PotionEffect* potionId;
    float potionEffectProbability;
};
