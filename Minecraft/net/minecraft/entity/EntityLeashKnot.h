#pragma once
#include "EntityHanging.h"

class EntityLeashKnot : public EntityHanging
{
  public:
    explicit EntityLeashKnot(World *worldIn);
    EntityLeashKnot(World *worldIn, BlockPos hangingPositionIn);
    void setPosition(double x, double y, double z) override;
    void updateFacingWithBoundingBox(EnumFacing facingDirectionIn) override;
    int32_t getWidthPixels() override;
    int32_t getHeightPixels() override;
    float getEyeHeight();
    bool isInRangeToRenderDist(double distance) override;
    void onBroken(Entity *brokenEntity) override;
    bool writeToNBTOptional(NBTTagCompound *compound) override;
    void writeEntityToNBT(NBTTagCompound *compound) override;
    void readEntityFromNBT(NBTTagCompound *compound) override;
    bool processInitialInteract(EntityPlayer *player, EnumHand hand) override;
    bool onValidSurface() override;
    static EntityLeashKnot *createKnot(World *worldIn, BlockPos fence);
    static EntityLeashKnot *getKnotForPosition(World *worldIn, BlockPos pos);
    void playPlaceSound() override;

  protected:
    void updateBoundingBox() override;
};
