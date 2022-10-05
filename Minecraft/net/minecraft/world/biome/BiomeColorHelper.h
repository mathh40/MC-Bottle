#pragma once
#include "../../util/math/BlockPos.h"
#include "Biome.h"

namespace BiomeColorHelper
{
using ColorResolver = std::function<int32_t(Biome *biome, BlockPos &blockPosition)>;

const auto GRASS_COLOR = [](Biome *biome, BlockPos &blockPosition) { return biome->getGrassColorAtPos(blockPosition); };

const auto FOLIAGE_COLOR = [](Biome *biome, BlockPos &blockPosition) {
    return biome->getFoliageColorAtPos(blockPosition);
};

const auto WATER_COLOR = [](Biome *biome, BlockPos &blockPosition) { return biome->getWaterColor(); };

int32_t getGrassColorAtPos(IBlockAccess *blockAccess, BlockPos &pos);
int32_t getFoliageColorAtPos(IBlockAccess *blockAccess, BlockPos &pos);
int32_t getWaterColorAtPos(IBlockAccess *blockAccess, BlockPos &pos);

} // namespace BiomeColorHelper
