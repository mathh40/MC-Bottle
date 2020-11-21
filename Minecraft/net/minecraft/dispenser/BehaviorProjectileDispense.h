#pragma once
#include "BehaviorDefaultDispenseItem.h"

class IProjectile;

class BehaviorProjectileDispense :public BehaviorDefaultDispenseItem {
  public:
    ItemStack dispenseStack(IBlockSource *source, const ItemStack &stack) override;
   protected:
    void playDispenseSound(IBlockSource *source) override;
   virtual IProjectile* getProjectileEntity(World var1, IPosition var2, ItemStack var3) = 0;
    float getProjectileInaccuracy();
    float getProjectileVelocity();
};
