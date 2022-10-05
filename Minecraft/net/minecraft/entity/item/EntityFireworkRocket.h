#pragma once
#include "../Entity.h"

class EntityFireworkRocket : public Entity
{
  public:
    EntityFireworkRocket(World *worldIn);
    EntityFireworkRocket(World *worldIn, double x, double y, double z, ItemStack givenItem);
    EntityFireworkRocket(World *worldIn, ItemStack givenItem, EntityLivingBase *p_i47367_3_);
    bool isInRangeToRenderDist(double distance) override;
    bool isInRangeToRender3d(double x, double y, double z) override;
    void setVelocity(double x, double y, double z) override;
    void onUpdate() override;
    bool isAttachedToEntity();
    void handleStatusUpdate(std::byte id) override;
    static void registerFixesFireworkRocket(DataFixer fixer);
    void writeEntityToNBT(NBTTagCompound *compound) override;
    void readEntityFromNBT(NBTTagCompound *compound) override;
    bool canBeAttackedWithItem() override;

  protected:
    void entityInit() override;

  private:
    void dealExplosionDamage();

    static DataParameter FIREWORK_ITEM;
    static DataParameter BOOSTED_ENTITY_ID;
    int32_t fireworkAge;
    int32_t lifetime;
    EntityLivingBase *boostedEntity;
};
