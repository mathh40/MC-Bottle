#pragma once
#include <string>
#include "MapGenStructure.h"

class MapGenMineshaft :public MapGenStructure
{
public:
	MapGenMineshaft() = default;
	std::string getStructureName() override;
	explicit MapGenMineshaft(const std::unordered_map<std::string,std::string>& p_i2034_1_);
	std::optional<BlockPos> getNearestStructurePos(World* worldIn, const BlockPos& pos, bool findUnexplored) override;

	enum Type
	{
		NORMAL,
		MESA
	};

protected:
	bool canSpawnStructureAtCoords(int32_t chunkX, int32_t chunkZ) override;
	StructureStart getStructureStart(int32_t chunkX, int32_t chunkZ) override;
private:
	double chance = 0.004;
};
