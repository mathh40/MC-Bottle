#pragma once
#include "EntityMinecart.h"

class EntityMinecartFurnace : public EntityMinecart
{
  public:
    explicit EntityMinecartFurnace(World *worldIn);
    EntityMinecartFurnace(World *worldIn, double x, double y, double z);
    static void registerFixesMinecartFurnace(DataFixer fixer);
    EntityMinecart::Type getType() override;
    void onUpdate() override;
    void killMinecart(DamageSource::DamageSource source) override;
    bool processInitialInteract(EntityPlayer *player, EnumHand hand) override;
    IBlockState *getDefaultDisplayTile() override;

  protected:
    void entityInit() override;
    double getMaximumSpeed() const override;
    void moveAlongTrack(const BlockPos &pos, IBlockState *state) override;
    void applyDrag() override;
    void writeEntityToNBT(NBTTagCompound *compound) override;
    void readEntityFromNBT(NBTTagCompound *compound) override;
    bool isMinecartPowered();
    void setMinecartPowered(bool p_94107_1_);

    double pushX;
    double pushZ;

  private:
    static DataParameter POWERED;
    int32_t fuel;
};
