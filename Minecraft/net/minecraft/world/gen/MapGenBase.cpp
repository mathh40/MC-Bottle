#include "MapGenBase.h"

void MapGenBase::generate(World* worldIn, int32_t x, int32_t z, ChunkPrimer& primer)
{
	auto i = range;
	world = worldIn;
	rand.seed(worldIn->getSeed());
	auto j = MathHelper::nextLong(rand);
	auto k = MathHelper::nextLong(rand);

	for (auto l = x - i; l <= x + i; ++l) 
	{
		for (auto i1 = z - i; i1 <= z + i; ++i1) 
		{
			auto j1 = (long)l * j;
			auto k1 = (long)i1 * k;
			rand.seed(j1 ^ k1 ^ worldIn->getSeed());
			recursiveGenerate(worldIn, l, i1, x, z, primer);
		}
	}
}

void MapGenBase::setupChunkSeed(int64_t p_191068_0_, pcg32& p_191068_2_, int64_t p_191068_3_, int64_t p_191068_4_)
{
	p_191068_2_.seed(p_191068_0_);
	const auto i = MathHelper::nextLong(rand);
	const auto j = MathHelper::nextLong(rand);
	const auto k = (long)p_191068_3_ * i;
	const auto l = (long)p_191068_4_ * j;
	p_191068_2_.seed(k ^ l ^ p_191068_0_);
}

void MapGenBase::recursiveGenerate(World* worldIn, int32_t chunkX, int32_t chunkZ, int32_t originalX, int32_t originalZ,
	ChunkPrimer& chunkPrimerIn)
{
}
