#include "BiomeEndDecorator.h"

void BiomeEndDecorator::genDecorations(Biome* biomeIn, World* worldIn, pcg32& random)
{
	generateOres(worldIn, random);
	WorldGenSpikes.EndSpike[] aworldgenspikes$endspike = getSpikesForWorld(worldIn);
	WorldGenSpikes.EndSpike[] var5 = aworldgenspikes$endspike;
	int var6 = aworldgenspikes$endspike.length;

	for (int var7 = 0; var7 < var6; ++var7) {
		WorldGenSpikes.EndSpike worldgenspikes$endspike = var5[var7];
		if (worldgenspikes$endspike.doesStartInChunk(chunkPos)) 
		{
			spikeGen.setSpike(worldgenspikes$endspike);
			spikeGen.generate(worldIn, random, BlockPos(worldgenspikes$endspike.getCenterX(), 45, worldgenspikes$endspike.getCenterZ()));
		}
	}
}

WorldGenSpikes.EndSpike[] BiomeEndDecorator::getSpikesForWorld(World* p_185426_0_)
{
	pcg32 random(p_185426_0_->getSeed());
	auto i = random.nextLong() & 65535L;
	return (WorldGenSpikes.EndSpike[])SPIKE_CACHE.getUnchecked(i);
}