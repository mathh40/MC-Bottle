#pragma once
#include "../Entity.h"

class EntityEnderEye : public Entity
{
  public:
    explicit EntityEnderEye(World *worldIn);
    EntityEnderEye(World *worldIn, double x, double y, double z);
    bool isInRangeToRenderDist(double distance) override;
    void moveTowards(const BlockPos &pos);
    void setVelocity(double x, double y, double z) override;
    void onUpdate() override;
    void writeEntityToNBT(NBTTagCompound *compound) override;
    void readEntityFromNBT(NBTTagCompound *compound) override;
    float getBrightness() override;
    int32_t getBrightnessForRender() override;
    bool canBeAttackedWithItem() override;

  protected:
    void entityInit() override;

  private:
    double targetX;
    double targetY;
    double targetZ;
    int32_t despawnTimer;
    bool shatterOrDrop;
};
