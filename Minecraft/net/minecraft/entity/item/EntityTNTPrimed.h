#pragma once
#include "../Entity.h"

class EntityTNTPrimed : public Entity
{
  public:
    EntityTNTPrimed(World *worldIn);
    EntityTNTPrimed(World *worldIn, double x, double y, double z, EntityLivingBase *igniter);
    bool canBeCollidedWith() override;
    void onUpdate() override;
    EntityLivingBase *getTntPlacedBy() const;
    float getEyeHeight() const override;
    void setFuse(int32_t fuseIn);
    void notifyDataManagerChange(DataParameter key) override;
    int32_t getFuseDataManager();
    int32_t getFuse() const;

  protected:
    void entityInit() override;
    bool canTriggerWalking() override;
    void writeEntityToNBT(NBTTagCompound *compound) override;
    void readEntityFromNBT(NBTTagCompound *compound) override;

  private:
    void explode();

    static DataParameter FUSE;
    EntityLivingBase *tntPlacedBy;
    int32_t fuse;
};
