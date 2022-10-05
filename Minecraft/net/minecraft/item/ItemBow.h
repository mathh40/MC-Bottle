#pragma once
#include "Item.h"

class ItemBow : public Item
{
  public:
    ItemBow();
    void onPlayerStoppedUsing(ItemStack stack, World *worldIn, EntityLivingBase *entityLiving,
                              int32_t timeLeft) override;
    static float getArrowVelocity(int32_t charge);
    int32_t getMaxItemUseDuration(ItemStack stack) override;
    EnumAction getItemUseAction(ItemStack stack) override;
    ActionResult onItemRightClick(World *worldIn, EntityPlayer *playerIn, EnumHand handIn) override;
    int32_t getItemEnchantability() override;

  protected:
    bool isArrow(ItemStack stack) const;

  private:
    ItemStack findAmmo(EntityPlayer *player);
};
