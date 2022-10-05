#pragma once
#include "ITickable.h"
#include "TileEntity.h"

class PlayerProfileCache;
class MinecraftSessionService;

class TileEntitySkull : public TileEntity, ITickable
{
  public:
    static void setProfileCache(const PlayerProfileCache &profileCacheIn);
    static void setSessionService(const MinecraftSessionService &sessionServiceIn);

    NBTTagCompound *writeToNBT(NBTTagCompound *compound) override;
    void readFromNBT(NBTTagCompound *compound) override;
    void update() override;
    float getAnimationProgress(float p_184295_1_);
    std::optional<GameProfile> getPlayerProfile();
    std::optional<SPacketUpdateTileEntity> getUpdatePacket() override;
    NBTTagCompound *getUpdateTag() override;
    void setType(int32_t type);
    void setPlayerProfile(std::optional<GameProfile> playerProfile);
    static std::optional<GameProfile> updateGameProfile(std::optional<GameProfile> input);
    int32_t getSkullType() const;
    int32_t getSkullRotation() const;
    void setSkullRotation(int32_t rotation);
    void mirror(Mirror mirrorIn) override;
    void rotate(Rotation rotationIn) override;

  private:
    void updatePlayerProfile();

    int32_t skullType;
    int32_t skullRotation;
    std::optional<GameProfile> playerProfile;
    int32_t dragonAnimatedTicks;
    bool dragonAnimated;
    static PlayerProfileCache profileCache;
    static MinecraftSessionService sessionService;
};
