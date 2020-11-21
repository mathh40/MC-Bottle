#pragma once
#include "IBehaviorDispenseItem.h"

class IPosition;

class BehaviorDefaultDispenseItem :public IBehaviorDispenseItem {
  public:
    ItemStack dispense(IBlockSource *source, const ItemStack &stack) override;
    static void doDispense(World *worldIn, const ItemStack &stack, int32_t speed, EnumFacing facing,
                           IPosition *position);
protected:
    virtual ItemStack dispenseStack(IBlockSource *source, const ItemStack &stack);
    virtual void playDispenseSound(IBlockSource *source);
    void spawnDispenseParticles(IBlockSource *source, EnumFacing facingIn) const;

private:
    int32_t getWorldEventDataFrom(EnumFacing facingIn);
};

