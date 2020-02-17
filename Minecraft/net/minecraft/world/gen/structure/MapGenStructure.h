#pragma once
#include "gen/MapGenBase.h"
#include <unordered_map>
#include <mutex>
#include "../../../../../../pcg-cpp/pcg_random.hpp"
#include "StructureStart.h"

class ChunkPos;
class ChunkPrimer;
class World;

class MapGenStructure :public MapGenBase
{
public:
	virtual std::string getStructureName() = 0;
	bool generateStructure(World* worldIn, pcg32& randomIn, const ChunkPos& chunkCoord);
	bool isInsideStructure(BlockPos& pos);
	bool isPositionInStructure(World* worldIn, const BlockPos& pos);
	virtual std::optional<BlockPos> getNearestStructurePos(World* var1, BlockPos& var2, bool var3) = 0;
protected:
	std::unordered_map<int64_t, StructureStart> structureMap;
	void recursiveGenerate(World* worldIn, int32_t chunkX, int32_t chunkZ, int32_t originalX, int32_t originalZ, ChunkPrimer& chunkPrimerIn) override;
	std::optional<StructureStart> getStructureAt(const BlockPos& pos);
	void initializeStructureData(World* worldIn);
	virtual bool canSpawnStructureAtCoords(int32_t var1, int32_t var2) = 0;
	virtual StructureStart getStructureStart(int32_t var1, int32_t var2) = 0;
	std::optional<BlockPos> findNearestStructurePosBySpacing(World* worldIn, MapGenStructure& structureType, BlockPos& startPos, int32_t distanceStep, int32_t stepOffset, int32_t randomSeedZ, bool addExtraRandomness, int32_t maxAttempts, bool findUnexplored);
private:
	MapGenStructureData structureData;
	mutable std::mutex mux;
	void setStructureStart(int chunkX, int chunkZ, StructureStart start);
};
