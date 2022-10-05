#pragma once
#include "../Entity.h"

class EntityFallingBlock : public Entity
{
  public:
    EntityFallingBlock(World *worldIn);
    EntityFallingBlock(World *worldIn, double x, double y, double z, IBlockState *fallingBlockState);
    bool canBeAttackedWithItem() override;
    void setOrigin(const BlockPos &p_184530_1_);
    BlockPos getOrigin();
    bool canBeCollidedWith() override;
    void onUpdate() override;
    void fall(float distance, float damageMultiplier) override;
    static void registerFixesFallingBlock(DataFixer fixer);
    World *getWorldObj() const;
    void setHurtEntities(bool p_145806_1_);
    bool canRenderOnFire() override;
    void addEntityCrashInfo(CrashReportCategory category);
    IBlockState *getBlock() const;
    bool ignoreItemEntityData() override;

    int32_t fallTime;
    bool shouldDropItem = true;
    NBTTagCompound *tileEntityData;

  protected:
    bool canTriggerWalking() override;
    void entityInit() override;
    void writeEntityToNBT(NBTTagCompound *compound) override;
    void readEntityFromNBT(NBTTagCompound *compound) override;

    static DataParameter ORIGIN;

  private:
    IBlockState *fallTile;
    bool dontSetBlock;
    bool hurtEntities;
    int32_t fallHurtMax  = 40;
    float fallHurtAmount = 2.0F;
};
