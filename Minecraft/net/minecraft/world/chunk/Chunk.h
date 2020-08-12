#pragma once
#include "concurrentqueue.h"
#include "math/ChunkPos.h"
#include "../../../../../spdlog/include/spdlog/logger.h"
#include "state/IBlockState.h"
#include "state/BlockPistonStructureHelper.h"

class World;
class ChunkPrimer;
class IChunkGenerator;
enum class EnumSkyBlock;

class Chunk
{
public:
	enum class EnumCreateEntityType
	{
		IMMEDIATE,
		QUEUED,
		CHECK
	};

	static ExtendedBlockStorage NULL_BLOCK_STORAGE;
	int32_t x;
	int32_t z;
	bool unloadQueued;

	Chunk(World* worldIn, int32_t x, int32_t z);
	Chunk(World* worldIn, ChunkPrimer primer, int32_t x, int32_t z);
	bool isAtLocation(int32_t x, int32_t z);
	int32_t getHeight(BlockPos& pos);
	int32_t getHeightValue(int32_t x, int32_t z);
	int32_t getTopFilledSegment();
	std::array<ExtendedBlockStorage*, 16> getBlockStorageArray();
	void generateSkylightMap();
	int32_t getBlockLightOpacity(BlockPos& pos);
	IBlockState* getBlockState(BlockPos& pos);
	IBlockState* getBlockState(int32_t x, int32_t y, int32_t z);
	IBlockState* setBlockState(BlockPos& pos, IBlockState* state);
	int32_t getLightFor(EnumSkyBlock type, BlockPos& pos);
	void setLightFor(EnumSkyBlock type, BlockPos& pos, int32_t value);
	int32_t getLightSubtracted(BlockPos& pos, int32_t amount);
	void addEntity(Entity* entityIn);
	void removeEntity(Entity* entityIn);
	void removeEntityAtIndex(Entity* entityIn, int32_t index);
	bool canSeeSky(BlockPos& pos);
	TileEntity* getTileEntity(BlockPos& pos, Chunk::EnumCreateEntityType creationMode);
	void addTileEntity(TileEntity* tileEntityIn);
	void addTileEntity(BlockPos& pos, TileEntity* tileEntityIn);
	void removeTileEntity(BlockPos& pos);
	void onLoad();
	void onUnload();
	void markDirty();
	template<typename Predicate>
	void getEntitiesWithinAABBForEntity(Entity* entityIn, AxisAlignedBB& aabb, std::vector<Entity*> listToFill, Predicate filter);
	template<typename T,typename Predicate>
	void getEntitiesOfTypeWithinAABB(AxisAlignedBB aabb, std::vector<Entity*> listToFill, Predicate filter);
	bool needsSaving(bool p_76601_1_) const;
	pcg32 getRandomWithSeed(int64_t seed) const;
	bool isEmpty();
	void populate(IChunkProvider* chunkProvider, IChunkGenerator* chunkGenrator);
	BlockPos getPrecipitationHeight(BlockPos& pos);
	void onTick(bool skipRecheckGaps);
	bool isPopulated() const;
	bool wasTicked() const;
	ChunkPos getPos() const;
	bool isEmptyBetween(int32_t startY, int32_t endY);
	void setStorageArrays(std::array<ExtendedBlockStorage*, 16> newStorageArrays);
	void read(PacketBuffer buf, int32_t availableSections, bool groundUpContinuous);
	Biome* getBiome(BlockPos& pos, BiomeProvider provider);
	std::array<unsigned char, 256> getBiomeArray() const;
	void setBiomeArray(std::array<unsigned char, 256> biomeArray);
	void resetRelightChecks();
	void enqueueRelightChecks();
	void checkLight();
	bool isLoaded() const;
	void markLoaded(bool loaded);
	World* getWorld() const;
	std::array<int32_t, 256> getHeightMap() const;
	void setHeightMap(std::array<int32_t, 256> newHeightMap);
	std::unordered_map<BlockPos, Entity*> getTileEntityMap() const;
	ClassInheritanceMultiMap[] getEntityLists();
	bool isTerrainPopulated() const;
	void setTerrainPopulated(bool terrainPopulated);
	bool isLightPopulated() const;
	void setLightPopulated(bool lightPopulated);
	void setModified(bool modified);
	void setHasEntities(bool hasEntitiesIn);
	void setLastSaveTime(int64_t saveTime);
	int32_t getLowestHeight() const;
	int64_t getInhabitedTime() const;
	void setInhabitedTime(int64_t newInhabitedTime);

protected: 
	void generateHeightMap();
	void populate(IChunkGenerator* generator);
private:
	static std::shared_ptr<spdlog::logger> LOGGER;
	std::array<ExtendedBlockStorage*, 16> storageArrays;
	std::array<unsigned char, 256> blockBiomeArray;
	std::array<int32_t, 256> precipitationHeightMap;
	std::array<bool, 256> updateSkylightColumns;
	bool loaded;
	World* world;
	std::array<int32_t, 256> heightMap;
	bool isGapLightingUpdated;
	std::unordered_map<BlockPos,Entity*> tileEntities;
	ClassInheritanceMultiMap[] entityLists;
	bool TerrainPopulated;
	bool LightPopulated;
	bool ticked;
	bool dirty;
	bool hasEntities;
	int64_t lastSaveTime;
	int32_t heightMapMinimum;
	int64_t inhabitedTime;
	int32_t queuedLightChecks;
	moodycamel::ConcurrentQueue<BlockPos> tileEntityPosQueue;

	ExtendedBlockStorage* getLastExtendedBlockStorage();
	void propagateSkylightOcclusion(int32_t x, int32_t z);
	void recheckGaps(bool onlyOne);
	void checkSkylightNeighborHeight(int32_t x, int32_t z, int32_t maxValue);
	void updateSkylightNeighborHeight(int32_t x, int32_t z, int32_t startY, int32_t endY);
	void relightBlock(int32_t x, int32_t y, int32_t z);
	int32_t getBlockLightOpacity(int32_t x, int32_t y, int32_t z);
	TileEntity* createNewTileEntity(BlockPos& pos);
	void setSkylightUpdated();
	void checkLightSide(EnumFacing facing);
	bool checkLight(int32_t x, int32_t z);

};

template <typename Predicate>
void Chunk::getEntitiesWithinAABBForEntity(Entity* entityIn, AxisAlignedBB& aabb, std::vector<Entity*> listToFill,
	Predicate filter)
{
	auto i = MathHelper::floor((aabb.getminY() - 2.0) / 16.0);
	auto j = MathHelper::floor((aabb.getmaxY() + 2.0) / 16.0);
	i = MathHelper::clamp(i, 0, this.entityLists.length - 1);
	j = MathHelper::clamp(j, 0, this.entityLists.length - 1);

label67:
	for (auto k = i; k <= j; ++k) 
	{
		if (!entityLists[k].isEmpty()) 
		{
			Iterator var8 = entityLists[k].iterator();

			while (true) 
			{
				Entity[] aentity;
				do 
				{
					Entity entity;
					do 
					{
						do 
						{
							if (!var8.hasNext()) 
							{
								goto label67;
							}

							entity = (Entity)var8.next();
						} 
						while (!entity->getEntityBoundingBox().intersects(aabb));
					}
					while (entity == entityIn);

					if (filter == null || filter.apply(entity)) 
					{
						listToFill.add(entity);
					}

					aentity = entity->getParts();
				} 
				while (aentity == null);

				Entity[] var11 = aentity;
				int var12 = aentity.length;

				for (int var13 = 0; var13 < var12; ++var13) 
				{
					Entity entity1 = var11[var13];
					if (entity1 != entityIn && entity1.getEntityBoundingBox().intersects(aabb) && (filter == null || filter.apply(entity1))) 
					{
						listToFill.add(entity1);
					}
				}
			}
		}
	}
}

template <typename T, typename Predicate>
void Chunk::getEntitiesOfTypeWithinAABB(AxisAlignedBB aabb, std::vector<Entity*> listToFill, Predicate filter)
{
	auto i = MathHelper::floor((aabb.getminY() - 2.0) / 16.0);
	auto j = MathHelper::floor((aabb.getmaxY() + 2.0) / 16.0);
	i = MathHelper::clamp(i, 0, entityLists.size() - 1);
	j = MathHelper::clamp(j, 0, entityLists.size() - 1);

label33:
	for (int k = i; k <= j; ++k) 
	{
		Iterator var8 = entityLists[k].getByClass(entityClass).iterator();

		while (true) 
		{
			Entity* t;
			do 
			{
				do 
				{
					if (!var8.hasNext()) 
					{
						goto label33;
					}

					t = (Entity*)var8.next();
				} 
				while (!t->getEntityBoundingBox().intersects(aabb));
			} 
			while (filter != null && !filter.apply(t));

			listToFill.add(t);
		}
	}
}
