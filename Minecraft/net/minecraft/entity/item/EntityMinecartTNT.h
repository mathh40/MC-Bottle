#pragma once
#include "EntityMinecart.h"

class EntityMinecartTNT :public EntityMinecart {
public:
    explicit EntityMinecartTNT(World* worldIn);
    EntityMinecartTNT(World* worldIn, double x, double y, double z);
    static void registerFixesMinecartTNT(DataFixer fixer);
    EntityMinecart::Type getType() override;
    IBlockState* getDefaultDisplayTile() override;
    void onUpdate() override;
    bool attackEntityFrom(DamageSource::DamageSource source, float amount) override;
    void killMinecart(DamageSource::DamageSource source) override;
    void fall(float distance, float damageMultiplier) override;
    void onActivatorRailPass(int32_t x, int32_t y, int32_t z, bool receivingPower) override;
    void handleStatusUpdate(std::byte id) override;
    void ignite();
    int getFuseTicks() const;
    bool isIgnited() const;
    float getExplosionResistance(Explosion explosionIn, World* worldIn, BlockPos pos, IBlockState* blockStateIn) override;
    bool canExplosionDestroyBlock(Explosion explosionIn, World* worldIn, BlockPos pos, IBlockState* blockStateIn, float p_174816_5_) override;

protected:
    void explodeCart(double p_94103_1_);
    void readEntityFromNBT(NBTTagCompound* compound) override;
    void writeEntityToNBT(NBTTagCompound* compound) override;
private:
    int32_t minecartTNTFuse = -1;
};