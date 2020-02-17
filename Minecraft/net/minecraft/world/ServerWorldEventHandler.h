#pragma once
#include "IWorldEventListener.h"

class ServerWorldEventHandler : public IWorldEventListener
{
public:

	ServerWorldEventHandler(MinecraftServer& mcServerIn, WorldServer& worldServerIn);
	void notifyBlockUpdate(World* worldIn, BlockPos& pos, IBlockState* oldState, IBlockState* newState, int32_t flags) override;
	void notifyLightSet(BlockPos& pos) override;
	void markBlockRangeForRenderUpdate(int32_t x1, int32_t y1, int32_t z1, int32_t x2, int32_t y2,
		int32_t z2) override;
	void playSoundToAllNearExcept(EntityPlayer* player, SoundEvent& soundIn, SoundCategory& category, double x, double y,
		double z, float volume, float pitch) override;
	void playRecord(SoundEvent& soundIn, BlockPos& pos) override;
	void spawnParticle(int32_t particleID, bool ignoreRange, double xCoord, double yCoord, double zCoord, double xSpeed, double ySpeed,
		double zSpeed, std::initializer_list<int32_t> parameters) override;
	void spawnParticle(int32_t id, bool ignoreRange, bool minimiseParticleLevel, double x, double y, double z, double xSpeed,
		double ySpeed, double zSpeed, std::initializer_list<int32_t> parameters) override;
	void onEntityAdded(Entity* entityIn) override;
	void onEntityRemoved(Entity* entityIn) override;
	void broadcastSound(int32_t soundID, BlockPos& pos, int32_t data) override;
	void playEvent(EntityPlayer* player, int32_t type, BlockPos& blockPosIn, int32_t data) override;
	void sendBlockBreakProgress(int32_t breakerId, BlockPos& pos, int32_t progress) override;
private:
	MinecraftServer& server;
	WorldServer& world;
};
