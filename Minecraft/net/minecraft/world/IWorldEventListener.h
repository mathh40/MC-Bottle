#include <cstdint>
#include <initializer_list>
class World;
class IBlockState;
class SoundCategory;
class SoundEvent;
class Entity;
class EntityPlayer;
class BlockPos;

class IWorldEventListener
{
public:
	virtual ~IWorldEventListener() = default;
	virtual void notifyBlockUpdate(World* var1, BlockPos& var2, IBlockState* var3, IBlockState* var4, int32_t var5) = 0;

	virtual void notifyLightSet(BlockPos& var1) = 0;

	virtual void markBlockRangeForRenderUpdate(int32_t var1, int32_t var2, int32_t var3, int32_t var4, int32_t var5, int32_t var6) = 0;

	virtual void playSoundToAllNearExcept(EntityPlayer* var1, SoundEvent& var2, SoundCategory& var3, double var4, double var6, double var8, float var10, float var11) = 0;

	virtual void playRecord(SoundEvent& var1, BlockPos& var2) = 0;

	virtual void spawnParticle(int32_t var1, bool var2, double var3, double var5, double var7, double var9, double var11, double var13, std::initializer_list<int32_t> var15) = 0;

	virtual void spawnParticle(int32_t var1, bool var2, bool var3, double var4, double var6, double var8, double var10, double var12, double var14, std::initializer_list<int32_t> var16) = 0;

	virtual void onEntityAdded(Entity* var1) = 0;

	virtual void onEntityRemoved(Entity* var1) = 0;

	virtual void broadcastSound(int32_t var1, BlockPos& var2, int32_t var3) = 0;

	virtual void playEvent(EntityPlayer* var1, int32_t var2, BlockPos& var3, int32_t var4) = 0;

	virtual void sendBlockBreakProgress(int32_t var1, BlockPos& var2, int32_t var3) = 0;
private:
};
