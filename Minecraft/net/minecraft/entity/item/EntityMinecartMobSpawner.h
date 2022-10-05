#pragma once
#include "../../tileentity/MobSpawnerBaseLogic.h"
#include "EntityMinecart.h"

class EntityMinecartMobSpawner : public EntityMinecart
{
  public:
    EntityMinecartMobSpawner(World *worldIn);
    EntityMinecartMobSpawner(World *worldIn, double x, double y, double z);
    static void registerFixesMinecartMobSpawner(DataFixer fixer);
    EntityMinecart::Type getType() override;
    IBlockState *getDefaultDisplayTile() override;
    void handleStatusUpdate(std::byte id) override;
    void onUpdate() override;

  protected:
    void readEntityFromNBT(NBTTagCompound *compound) override;
    void writeEntityToNBT(NBTTagCompound *compound) override;

  private:
    class MobSpawnerMinecartLogic : public MobSpawnerBaseLogic
    {
      public:
        explicit MobSpawnerMinecartLogic(EntityMinecartMobSpawner *entityIn);

        void broadcastEvent(std::byte id) override;

        World *getSpawnerWorld() const override;

        BlockPos getSpawnerPosition() const override;

      private:
        EntityMinecartMobSpawner *entity;
    };

    MobSpawnerMinecartLogic mobSpawnerLogic;
};
