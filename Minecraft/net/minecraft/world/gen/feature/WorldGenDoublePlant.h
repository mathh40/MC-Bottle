#pragma once
#include "WorldGenerator.h"

class WorldGenDoublePlant : public WorldGenerator
{
public:
	void setPlantType(BlockDoublePlant::EnumPlantType plantTypeIn);
	bool generate(World* worldIn, pcg32& rand, const BlockPos& position) override;
private:
	BlockDoublePlant::EnumPlantType plantType;
};