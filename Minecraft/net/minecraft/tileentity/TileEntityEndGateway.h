#include "ITickable.h"
#include "TileEntityEndPortal.h"

class TileEntityEndGateway : public TileEntityEndPortal, ITickable
{
  public:
    NBTTagCompound *writeToNBT(NBTTagCompound *compound);
    void readFromNBT(NBTTagCompound *compound);
    double getMaxRenderDistanceSquared() const;
    void update();
    bool isSpawning();
    bool isCoolingDown();
    float getSpawnPercent(float p_184302_1_);
    float getCooldownPercent(float p_184305_1_);
    std::optional<SPacketUpdateTileEntity> getUpdatePacket();
    NBTTagCompound *getUpdateTag();
    void triggerCooldown();
    bool receiveClientEvent(int32_t id, int32_t type);
    void teleportEntity(Entity *entityIn);

  private:
    BlockPos findExitPosition();
    void findExitPortal();
    static BlockPos findHighestBlock(World *p_184308_0_, BlockPos p_184308_1_, int32_t p_184308_2_, bool p_184308_3_);

    static std::shared_ptr<spdlog::logger> LOGGER;
    int64_t age;
    int32_t teleportCooldown;
    std::optional<BlockPos> exitPortal;
    bool exactTeleport;
};
