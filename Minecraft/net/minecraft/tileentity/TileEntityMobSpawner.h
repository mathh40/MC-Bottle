#include "ITickable.h"
#include "MobSpawnerBaseLogic.h"
#include "TileEntity.h"

class DataFixer;

class TileEntityMobSpawner : public TileEntity, ITickable
{
  public:
    TileEntityMobSpawner();
    static void registerFixesMobSpawner(DataFixer fixer);
    void readFromNBT(NBTTagCompound *compound) override;
    NBTTagCompound *writeToNBT(NBTTagCompound *compound) override;
    void update() override;
    std::optional<SPacketUpdateTileEntity> getUpdatePacket() override;
    NBTTagCompound *getUpdateTag() override;
    bool receiveClientEvent(int32_t id, int32_t type) override;
    bool onlyOpsCanSetNbt() override;
    MobSpawnerBaseLogic *getSpawnerBaseLogic() const;

  private:
    class MobSpawnerLogic : MobSpawnerBaseLogic
    {
      public:
        explicit MobSpawnerLogic(TileEntityMobSpawner *entity);
        void broadcastEvent(int32_t id) override;
        World *getSpawnerWorld() override;
        BlockPos getSpawnerPosition() override;
        void setNextSpawnData(WeightedSpawnerEntity p_184993_1_);

      private:
        TileEntityMobSpawner *entity;
    };

    std::unique_ptr<MobSpawnerLogic> spawnerLogic;
};
