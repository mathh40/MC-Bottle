#pragma once
#include "MapGenStructure.h"

class MapGenNetherBridge :public MapGenStructure
{
private:
	std::vector<SpawnListEntry> spawnList;
protected:
	bool canSpawnStructureAtCoords(int32_t chunkX, int32_t chunkZ) override;
	StructureStart getStructureStart(int32_t chunkX, int32_t chunkZ) override;
public:
	MapGenNetherBridge();
	std::string getStructureName() override;
	std::vector<SpawnListEntry> getSpawnList() const;
	std::optional<BlockPos> getNearestStructurePos(World* worldIn, const BlockPos& pos, bool findUnexplored);

	class Start :public StructureStart
	{
	public:
		Start() = default;
		Start(World* worldIn, pcg32& random, int32_t chunkX, int32_t chunkZ);
	};
};
