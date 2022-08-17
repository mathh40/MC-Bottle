#pragma once
#include "IBlockAccess.h"
#include "../block/BlockAnvil.h"
#include <vector>
#include "../util/EnumParticleTypes.h"
#include "../util/Util.h"
#include "../util/math/MathHelper.h"
#include "DimensionType.h"
#include "GameRules.h"
#include "DifficultyInstance.h"
#include "EnumDifficulty.h"
#include "WorldType.h"
#include "WorldSettings.h"
#include "../pathfinding/PathWorldListener.h"
#include "../profiler/Profiler.h"
#include "../scoreboard/Scoreboard.h"
#include "../scoreboard/ServerScoreboard.h"
#include "../tileentity/TileEntity.h"
#include "biome/Biome.h"
#include "biome/BiomeProvider.h"
#include "chunk/Chunk.h"
#include "chunk/IChunkProvider.h"
#include "border/WorldBorder.h"
#include "storage/WorldSavedData.h"
#include "storage/MapStorage.h"
#include "../village/VillageCollection.h"
#include "gen/ChunkGeneratorEnd.h"
#include "..\util\random\RandomJava.h"

class CrashReport;
class NextTickListEntry;
class Packet;
class StructureBoundingBox;
class WorldSettings;
class IWorldEventListener;
enum class EnumSkyBlock;
class NBTTagCompound;
class EnumParticleTypes;
class SoundCategory;

class World :public IBlockAccess
{
public:
	std::vector<Entity*> loadedEntityList;
	std::vector<TileEntity*> loadedTileEntityList;
	std::vector<TileEntity*> tickableTileEntities;
	std::vector<EntityPlayer*> playerEntities;
	std::vector<Entity*> weatherEffects;
	RandomJava rand;
	WorldProvider* provider;
	Profiler profiler;
	bool isRemote;
	virtual World* init();
	Biome& getBiome(const BlockPos& pos) override;
	BiomeProvider& getBiomeProvider();
	void initialize(WorldSettings settings);
	virtual MinecraftServer* getMinecraftServer();
	virtual void setInitialSpawnLocation();
	IBlockState* getGroundAboveSeaLevel(const BlockPos& pos);
	bool isAirBlock(const BlockPos& pos) override;
	bool isBlockLoaded(BlockPos& pos);
	bool isBlockLoaded(BlockPos& pos, bool allowEmpty);
	bool isAreaLoaded(BlockPos& center, int32_t radius);
	bool isAreaLoaded(BlockPos& center, int32_t radius, bool allowEmpty);
	bool isAreaLoaded(BlockPos& from, BlockPos& to);
	bool isAreaLoaded(BlockPos& from, BlockPos& to, bool allowEmpty);
	bool isAreaLoaded(StructureBoundingBox box);
	bool isAreaLoaded(StructureBoundingBox box, bool allowEmpty);
	Chunk& getChunk(BlockPos pos);
	Chunk& getChunk(int32_t chunkX, int32_t chunkZ);
	bool isChunkGeneratedAt(int32_t x, int32_t z);
	bool setBlockState(BlockPos& pos, IBlockState* newState, int32_t flags);
	bool setBlockToAir(BlockPos& pos);
	bool destroyBlock(BlockPos& pos, bool dropBlock);
	bool setBlockState(BlockPos& pos, IBlockState* state);
	void notifyBlockUpdate(BlockPos& pos, IBlockState* oldState, IBlockState* newState, int32_t flags);
	void notifyNeighborsRespectDebug(BlockPos& pos, Block* blockType, bool updateObservers);
	void markBlocksDirtyVertical(int32_t x, int32_t z, int32_t y1, int32_t y2);
	void markBlockRangeForRenderUpdate(BlockPos& rangeMin, BlockPos& rangeMax);
	void markBlockRangeForRenderUpdate(int32_t x1, int32_t y1, int32_t z1, int32_t x2, int32_t y2, int32_t z2);
	void updateObservingBlocksAt(BlockPos& pos, Block* blockType);
	void notifyNeighborsOfStateChange(BlockPos& pos, Block* blockType, bool updateObservers);
	void notifyNeighborsOfStateExcept(BlockPos& pos, Block* blockType, EnumFacing skipSide);
	void neighborChanged(BlockPos& pos, const Block* blockIn, BlockPos& fromPos);
	void observedNeighborChanged(BlockPos& pos, const Block* changedBlock, BlockPos& changedBlockPos);
	virtual bool isBlockTickPending(BlockPos& pos, Block* blockType) const;
	bool canSeeSky(BlockPos& pos);
	bool canBlockSeeSky(BlockPos& pos);
	int32_t getLight(BlockPos& pos);
	int32_t getLightFromNeighbors(BlockPos& pos);
	int32_t getLight(BlockPos& pos, bool checkNeighbors);
	BlockPos getHeight(BlockPos& pos);
	int32_t getHeight(int32_t x, int32_t z);
	int32_t getChunksLowestHorizon(int32_t x, int32_t z);
	int32_t getLightFromNeighborsFor(EnumSkyBlock type, BlockPos& pos);
	int32_t getLightFor(EnumSkyBlock type, BlockPos& pos);
	void setLightFor(EnumSkyBlock type, BlockPos& pos, int32_t lightValue);
	void notifyLightSet(BlockPos& pos);
	int32_t getCombinedLight(BlockPos& pos, int32_t lightValue);
	float getLightBrightness(const BlockPos& pos);
	IBlockState* getBlockState(const BlockPos& pos) override;
	bool isDaytime() const;
	std::optional<RayTraceResult> rayTraceBlocks(Vec3d& start, Vec3d& end);
	std::optional<RayTraceResult> rayTraceBlocks(Vec3d& start, Vec3d& end, bool stopOnLiquid);
	std::optional<RayTraceResult> rayTraceBlocks(Vec3d& vec31, Vec3d& vec32, bool stopOnLiquid, bool ignoreBlockWithoutBoundingBox, bool returnLastUncollidableBlock);
	void playSound(EntityPlayer* player, BlockPos pos, SoundEvent soundIn, SoundCategory category, float volume, float pitch);
	void playSound(EntityPlayer* player, double x, double y, double z, SoundEvent soundIn, SoundCategory category, float volume, float pitch);
	void playSound(double x, double y, double z, SoundEvent soundIn, SoundCategory category, float volume, float pitch, bool distanceDelay);
	void playRecord(BlockPos& blockPositionIn, std::optional <SoundEvent> soundEventIn);

	void spawnParticle(EnumParticleTypes particleType, double xCoord, double yCoord, double zCoord, double xSpeed, double ySpeed, double zSpeed, std::initializer_list<int32_t> parameters);
	void spawnAlwaysVisibleParticle(int32_t id, double x, double y, double z, double xSpeed, double ySpeed, double zSpeed, std::initializer_list<int32_t> parameters);
	void spawnParticle(EnumParticleTypes particleType, bool ignoreRange, double xCoord, double yCoord, double zCoord, double xSpeed, double ySpeed, double zSpeed, std::initializer_list<int32_t> parameters);
	virtual bool addWeatherEffect(Entity* entityIn);
	virtual bool spawnEntity(Entity* entityIn);
	void removeEntity(Entity* entityIn);
	void removeEntityDangerously(Entity* entityIn);
	void addEventListener(IWorldEventListener* listener);
	void removeEventListener(IWorldEventListener* listener);
	std::optional<std::vector<AxisAlignedBB>> getCollisionBoxes(Entity* entityIn, AxisAlignedBB& aabb);
	bool isInsideWorldBorder(Entity* entityToCheck);
	bool collidesWithAnyBlock(AxisAlignedBB& bbox);
	int32_t calculateSkylightSubtracted(float partialTicks);
	float getSunBrightness(float partialTicks);
	Vec3d getSkyColor(Entity* entityIn, float partialTicks);
	Vec3d getFogColor(float partialTicks);
	BlockPos getPrecipitationHeight(BlockPos& pos);
	BlockPos getTopSolidOrLiquidBlock(BlockPos& pos);
	float getStarBrightness(float partialTicks);
	bool isUpdateScheduled(BlockPos& pos, Block* blk) const;
	void scheduleUpdate(BlockPos& pos, Block* blockIn, int32_t delay) const;
	void updateBlockTick(BlockPos& pos, Block* blockIn, int32_t delay, int32_t priority) const;
	virtual void scheduleBlockUpdate(BlockPos& pos, Block* blockIn, int32_t delay, int32_t priority);
	virtual void updateEntities();
	bool addTileEntity(TileEntity* tile);
	void addTileEntities(std::initializer_list<TileEntity*> tileEntityCollection);
	void updateEntity(Entity* ent);
	virtual void updateEntityWithOptionalForce(Entity* entityIn, bool forceUpdate);
	bool checkNoEntityCollision(AxisAlignedBB& bb);
	bool checkNoEntityCollision(AxisAlignedBB& bb, std::optional<Entity*> entityIn);
	bool checkBlockCollision(AxisAlignedBB& bb);
	bool containsAnyLiquid(AxisAlignedBB& bb);
	bool isFlammableWithin(AxisAlignedBB& bb);
	bool handleMaterialAcceleration(AxisAlignedBB& bb, Material& materialIn, Entity* entityIn);
	bool isMaterialInBB(AxisAlignedBB& bb, Material& materialIn);
	Explosion createExplosion(Entity* entityIn, double x, double y, double z, float strength, bool damagesTerrain);
	virtual Explosion newExplosion(Entity* entityIn, double x, double y, double z, float strength, bool causesFire, bool damagesTerrain);
	float getBlockDensity(Vec3d vec, AxisAlignedBB& bb);
	bool extinguishFire(EntityPlayer* player, BlockPos& pos, EnumFacing side);
	std::string getDebugLoadedEntities();
	std::string getProviderName();
	TileEntity* getTileEntity(const BlockPos& pos) override;
	TileEntity* getPendingTileEntityAt(BlockPos& pos);
	void setTileEntity(BlockPos& pos, TileEntity* tileEntityIn);
	void removeTileEntity(BlockPos& pos);
	void markTileEntityForRemoval(TileEntity* tileEntityIn);
	bool isBlockFullCube(BlockPos& pos);
	bool isBlockNormalCube(BlockPos& pos, bool _default);
	void calculateInitialSkylight();
	void setAllowedSpawnTypes(bool hostile, bool peaceful);
	virtual void tick();
	void immediateBlockTick(BlockPos& pos, IBlockState* state, pcg32& random);
	bool canBlockFreezeWater(BlockPos& pos);
	bool canBlockFreezeNoWater(BlockPos& pos);
	bool canBlockFreeze(BlockPos& pos, bool noWaterAdj);
	bool canSnowAt(BlockPos& pos, bool checkLight);
	bool checkLight(BlockPos& pos);
	bool checkLightFor(EnumSkyBlock lightType, BlockPos& pos);
	virtual bool tickUpdates(bool runAllPending);
    virtual std::vector<NextTickListEntry> getPendingBlockUpdates(Chunk& chunkIn, bool remove);
    virtual std::vector<NextTickListEntry> getPendingBlockUpdates(StructureBoundingBox& structureBB, bool remove);
	std::vector<Entity*> getEntitiesWithinAABBExcludingEntity(Entity* entityIn, AxisAlignedBB& bb);
	std::vector<Entity*> getEntitiesInAABBexcluding(Entity* entityIn, AxisAlignedBB& boundingBox, std::function<bool(Entity*)> predicate);
	template<class Class,typename Predicate>
	std::vector<Class*> getEntities(Predicate filter);
	template<class Class, typename Predicate>
	std::vector<Class*> getPlayers(Predicate filter);
	template<class Class>
	std::vector<Class*> getEntitiesWithinAABB(const AxisAlignedBB& bb);
	template<class Class, typename Predicate>
	std::vector<Class*> getEntitiesWithinAABB(const AxisAlignedBB& aabb, Predicate filter);
	template<class Class>
	Entity* findNearestEntityWithinAABB(AxisAlignedBB& aabb, Entity* closestTo);
	Entity* getEntityByID(int32_t id);
	std::vector<Entity*> getLoadedEntityList() const;
	void markChunkDirty(BlockPos& pos, TileEntity* unusedTileEntity);
	template<class Class>
	int32_t countEntities();
	virtual void loadEntities(std::initializer_list<Entity*> entityCollection);
	void unloadEntities(std::initializer_list<Entity*> entityCollection);
	bool mayPlace(Block* blockIn, BlockPos& pos, bool skipCollisionCheck, EnumFacing sidePlacedOn, Entity* placer);
	uint32_t getSeaLevel() const;
	void setSeaLevel(uint32_t seaLevelIn);
	int32_t getStrongPower(BlockPos& pos, EnumFacing direction);
	WorldType getWorldType() override;
	int32_t getStrongPower(BlockPos& pos);
	bool isSidePowered(BlockPos& pos, EnumFacing side);
	int32_t getRedstonePower(BlockPos& pos, EnumFacing facing);
	bool isBlockPowered(BlockPos& pos);
	int32_t getRedstonePowerFromNeighbors(BlockPos& pos);
	EntityPlayer* getClosestPlayerToEntity(Entity* entityIn, double distance);
	EntityPlayer* getNearestPlayerNotCreative(Entity* entityIn, double distance);
	EntityPlayer* getClosestPlayer(double posX, double posY, double posZ, double distance, bool spectator);
	template<class Predicate>
	EntityPlayer* getClosestPlayer(double x, double y, double z, double distance, Predicate predicate);
	bool isAnyPlayerWithinRangeAt(double x, double y, double z, double range);
	EntityPlayer* getNearestAttackablePlayer(Entity* entityIn, double maxXZDistance, double maxYDistance);
	EntityPlayer* getNearestAttackablePlayer(BlockPos& pos, double maxXZDistance, double maxYDistance);
	template<class Predicate, class Function>
	EntityPlayer* getNearestAttackablePlayer(double posX, double posY, double posZ, double maxXZDistance, double maxYDistance, std::optional<Function> playerToDouble, Predicate predicate);
	EntityPlayer* getPlayerEntityByName(std::string name);
	EntityPlayer* getPlayerEntityByUUID(xg::Guid& uuid);
	void sendQuittingDisconnectingPacket();
	void checkSessionLock();
	void setTotalWorldTime(int64_t worldTime);
	int64_t getSeed();
	int64_t getTotalWorldTime();
	int64_t getWorldTime();
	void setWorldTime(int64_t time);
	BlockPos getSpawnPoint();
	void setSpawnPoint(BlockPos& pos);
	void joinEntityInSurroundings(Entity* entityIn);
	virtual bool isBlockModifiable(EntityPlayer* player, BlockPos& pos);
	virtual void setEntityState(Entity* entityIn, std::byte state);
	virtual IChunkProvider* getChunkProvider();
	virtual void addBlockEvent(BlockPos& pos, Block* blockIn, int32_t eventID, int32_t eventParam);
	ISaveHandler* getSaveHandler();
	WorldInfo getWorldInfo();
	GameRules getGameRules();
	virtual void updateAllPlayersSleepingFlag();
	float getThunderStrength(float delta);
	void setThunderStrength(float strength);
	float getRainStrength(float delta);
	void setRainStrength(float strength);
	bool isThundering();
	bool isRaining();
	bool isRainingAt(BlockPos& position);
	bool isBlockinHighHumidity(BlockPos& pos);
	MapStorage* getMapStorage();
	void setData(std::string dataID, WorldSavedData* worldSavedDataIn);
	template<class Class>
	WorldSavedData* loadData(std::string_view dataID);
	int32_t getUniqueDataId(std::string key);
	void playBroadcastSound(int32_t id, const BlockPos& pos, int32_t data);
	void playEvent(int32_t type, const BlockPos& pos, int32_t data);
	void playEvent(EntityPlayer* player, int32_t type, const BlockPos& pos, int32_t data);
	int32_t getHeight() const;
	int32_t getActualHeight();
	pcg32& setRandomSeed(int32_t seedX, int32_t seedY, int32_t seedZ);
	double getHorizon();
	CrashReportCategory addWorldInfoToCrashReport(CrashReport report);
	void sendBlockBreakProgress(int32_t breakerId, BlockPos& pos, int32_t progress);
	Calendar getCurrentDate();
	void makeFireworks(double x, double y, double z, double motionX, double motionY, double motionZ, NBTTagCompound* compound);
	Scoreboard& getScoreboard();
	void updateComparatorOutputLevel(BlockPos& pos, Block* blockIn);
	DifficultyInstance getDifficultyForLocation(BlockPos& pos);
	EnumDifficulty getDifficulty();
	int32_t getSkylightSubtracted();
	void setSkylightSubtracted(int32_t newSkylightSubtracted);
	int32_t getLastLightningBolt();
	void setLastLightningBolt(int32_t lastLightningBoltIn);
	VillageCollection getVillageCollection();
	WorldBorder getWorldBorder();
	bool isSpawnChunk(int32_t x, int32_t z);
	void sendPacketToServer(Packet& packetIn);
	LootTableManager getLootTableManager();
	BlockPos findNearestStructure(std::string structureName, BlockPos& position, bool findUnexplored);
	IBlockState* getBlockState(const BlockPos& pos);
protected:
	bool scheduledUpdatesAreImmediate;
	std::vector<Entity*> unloadedEntityList;
	std::unordered_map<uint32_t, Entity*>entitiesById;
	uint32_t updateLCG;
	uint32_t DIST_HASH_MAGIC = 1013904223;
	float prevRainingStrength;
	float rainingStrength;
	float prevThunderingStrength;
	float thunderingStrength;
	PathWorldListener pathListener;
	std::vector<IWorldEventListener*> eventListeners;
	IChunkProvider* chunkProvider;
	ISaveHandler* saveHandler;
	WorldInfo& worldInfo;
	bool findingSpawnPoint;
	MapStorage mapStorage;
	VillageCollection villageCollection;
	LootTableManager lootTable;
	AdvancementManager advancementManager;
	FunctionManager functionManager;
	Scoreboard worldScoreboard;
	bool spawnHostileMobs;
	bool spawnPeacefulMobs;
	World(ISaveHandler* saveHandlerIn, WorldInfo info, WorldProvider providerIn, Profiler profilerIn, bool client);
	virtual IChunkProvider* createChunkProvider();
	virtual bool isChunkLoaded(int32_t var1, int32_t var2, bool var3) = 0;
	virtual void onEntityAdded(Entity* entityIn);
	virtual void onEntityRemoved(Entity* entityIn);
	virtual void tickPlayers();
	void calculateInitialWeather();
	virtual void updateWeather();
	void playMoodSoundAndCheckLight(int32_t x, int32_t z, Chunk& chunkIn);
	virtual void updateBlocks();
private:
	uint32_t seaLevel = 63;
	std::vector<TileEntity*> addedTileEntityList;
	std::vector<TileEntity*> tileEntitiesToBeRemoved;
	uint64_t cloudColour = 16777215L;
	uint32_t skylightSubtracted;
	int lastLightningBolt;
	Calendar calendar;
	bool processingLoadedTiles;
	WorldBorder worldBorder;
	std::vector<uint32_t> lightUpdateBlockList;
	bool isValid(BlockPos& pos) const;
	bool isOutsideBuildHeight(BlockPos& pos) const;
	bool isAreaLoaded(int32_t xStart, int32_t yStart, int32_t zStart, int32_t xEnd, int32_t yEnd, int32_t zEnd, bool allowEmpty);
	void spawnParticle(int32_t particleID, bool ignoreRange, double xCood, double yCoord, double zCoord, double xSpeed, double ySpeed, double zSpeed, std::initializer_list<int32_t> parameters);
	bool getCollisionBoxes(Entity* entityIn, AxisAlignedBB aabb, bool p_191504_3_, std::optional<std::vector<AxisAlignedBB>>& outList);
	bool isWater(BlockPos& pos);
	int32_t getRawLight(BlockPos pos, EnumSkyBlock lightType);
};

template <class Class, class Predicate>
std::vector<Class*> World::getEntities(Predicate filter)
{
	std::vector<Entity*> list;
	for (auto entity4 : loadedEntityList)
	{
		if (Util::instanceof<Class>(entity4) && filter(entity4))
		{
			list.emplace_back(entity4);
		}
	}

	return list;
}

template <class Class, typename Predicate>
std::vector<Class*> World::getPlayers(Predicate filter)
{
	std::vector<Entity*> list;

	for (auto entity4 : playerEntities)
	{
		if (Util::instanceof<Class>(entity4) && filter(entity4)) 
		{
			list.emplace_back(entity4);
		}
	}

	return list;
}

template <class Class>
std::vector<Class*> World::getEntitiesWithinAABB(const AxisAlignedBB& bb)
{
	return getEntitiesWithinAABB<Class>(bb, EntitySelectors::NOT_SPECTATING);
}

template <class Class, typename Predicate>
std::vector<Class*> World::getEntitiesWithinAABB(const AxisAlignedBB& aabb, Predicate filter)
{
	auto j2 = MathHelper::floor((aabb.getminX() - 2.0) / 16.0);
	auto k2 = MathHelper::ceil((aabb.getmaxX() + 2.0) / 16.0);
	auto l2 = MathHelper::floor((aabb.getminZ() - 2.0) / 16.0);
	auto i3 = MathHelper::ceil((aabb.getmaxZ() + 2.0) / 16.0);
	std::vector<Entity*> list;

	for (auto j3 = j2; j3 < k2; ++j3) 
	{
		for (auto k3 = l2; k3 < i3; ++k3) 
		{
			if (isChunkLoaded(j3, k3, true)) 
			{
				getChunk(j3, k3).getEntitiesOfTypeWithinAABB<Class>(aabb, list, filter);
			}
		}
	}

	return list;
}

template <class Class>
Entity* World::findNearestEntityWithinAABB(AxisAlignedBB& aabb, Entity* closestTo)
{
	auto list = getEntitiesWithinAABB<Class>(aabb);
	auto t = nullptr;
	double d0 = std::numeric_limits<double>::max();

	for (auto t1 : list)
	{
		if (t1 != closestTo && EntitySelectors::NOT_SPECTATING(t1)) 
		{
			double d1 = closestTo->getDistanceSq(t1);
			if (d1 <= d0) 
			{
				t = t1;
				d0 = d1;
			}
		}
	}

	return t;
}

template <class Class>
int32_t World::countEntities()
{
	auto j2 = 0;
	auto entity4 = loadedEntityList.begin();

	while (true) 
	{
		do 
		{
			if (var3 == loadedEntityList.end())
			{
				return j2;
			}
		} 
		while (Util::instanceof<EntityLiving>(*entity4) && ((EntityLiving)(*entity4)).isNoDespawnRequired());

		if (Util::instanceof<Class>(*entity4)) 
		{
			++j2;
		}
	}
}

template <class Predicate>
EntityPlayer* World::getClosestPlayer(double x, double y, double z, double distance, Predicate predicate)
{
	double d0 = -1.0;
	EntityPlayer* entityplayer = nullptr;

	for (auto entityplayer1 : playerEntities)
	{
		if (predicate(entityplayer1)) 
		{
			double d1 = entityplayer1->getDistanceSq(x, y, z);
			if ((distance < 0.0 || d1 < distance * distance) && (d0 == -1.0 || d1 < d0)) 
			{
				d0 = d1;
				entityplayer = entityplayer1;
			}
		}
	}

	return entityplayer;
}

template <class Predicate, class Function>
EntityPlayer* World::getNearestAttackablePlayer(double posX, double posY, double posZ, double maxXZDistance,
	double maxYDistance, std::optional<Function> playerToDouble, Predicate predicate)
{
	double d0 = -1.0;
	EntityPlayer* entityplayer = nullptr;

	for (auto entityplayer1 : playerEntities)
	{
		if (!entityplayer1->capabilities.disableDamage && entityplayer1->isEntityAlive() && !entityplayer1->isSpectator() && (!predicate || predicate(entityplayer1))) 
		{
			double d1 = entityplayer1->getDistanceSq(posX, entityplayer1->posY, posZ);
			double d2 = maxXZDistance;
			if (entityplayer1->isSneaking()) {
				d2 = maxXZDistance * 0.800000011920929;
			}

			if (entityplayer1->isInvisible()) {
				float f = entityplayer1->getArmorVisibility();
				if (f < 0.1F) {
					f = 0.1F;
				}

				d2 *= (double)(0.7F * f);
			}

			if (playerToDouble) {
				d2 *= (Double)MoreObjects.firstNonNull(playerToDouble(entityplayer1), 1.0);
			}

			if ((maxYDistance < 0.0 || MathHelper::abs(entityplayer1->posY - posY) < maxYDistance * maxYDistance) && (maxXZDistance < 0.0 || d1 < d2 * d2) && (d0 == -1.0 || d1 < d0)) 
			{
				d0 = d1;
				entityplayer = entityplayer1;
			}
		}
	}

	return entityplayer;
}

template <class Class>
WorldSavedData* World::loadData(std::string_view dataID)
{
	return mapStorage.getOrLoadData<Class>(dataID);
}
