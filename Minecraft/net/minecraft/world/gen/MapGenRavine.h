#pragma once
#include "MapGenBase.h"

class MapGenRavine :public MapGenBase
{
public:
protected:
	static IBlockState* FLOWING_LAVA;
	static IBlockState* AIR;

	void addTunnel(int64_t p_180707_1_, int32_t p_180707_3_, int32_t p_180707_4_, ChunkPrimer& p_180707_5_, double p_180707_6_, double p_180707_8_, double p_180707_10_, float p_180707_12_, float p_180707_13_, float p_180707_14_, int32_t p_180707_15_, int32_t p_180707_16_, double p_180707_17_);
	void recursiveGenerate(World* worldIn, int32_t chunkX, int32_t chunkZ, int32_t originalX, int32_t originalZ, ChunkPrimer& chunkPrimerIn) override;
private:
	std::array<float, 1024> rs;
};
