#pragma once
#include "Entity.h"

class EntityHanging :public Entity {
public:
    EntityHanging(World* worldIn);
    EntityHanging(World* worldIn, BlockPos hangingPositionIn);
    void onUpdate() override;
    virtual bool onValidSurface();
    bool canBeCollidedWith() override;
    bool hitByEntity(Entity* entityIn) override;
    EnumFacing getHorizontalFacing() const;
    bool attackEntityFrom(DamageSource::DamageSource source, float amount) override;
    void move(const MoverType& type, double x, double y, double z) override;
    void addVelocity(double x, double y, double z) override;
    void writeEntityToNBT(NBTTagCompound* compound) override;
    void readEntityFromNBT(NBTTagCompound* compound) override;
    virtual int32_t getWidthPixels() = 0;
    virtual int32_t getHeightPixels() = 0;
    virtual void onBroken(Entity* var1) = 0;
    virtual void playPlaceSound() = 0;
    EntityItem* entityDropItem(ItemStack stack, float offsetY) override;
    void setPosition(double x, double y, double z) override;
    BlockPos getHangingPosition() const;
    float getRotatedYaw(Rotation transformRotation);
    float getMirroredYaw(Mirror transformMirror);
    void onStruckByLightning(EntityLightningBolt* lightningBolt) override;


    std::optional<EnumFacing> facingDirection;
protected:
    void entityInit() override;
    virtual void updateFacingWithBoundingBox(EnumFacing facingDirectionIn);
    virtual void updateBoundingBox();
    bool shouldSetPosAfterLoading() override;


    BlockPos hangingPosition;
private:
    double offs(int32_t p_190202_1_);

    int32_t tickCounter1;
};