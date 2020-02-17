#pragma once
#include "WorldGenerator.h"
#include "ResourceLocation.h"
#include "spdlog/logger.h"

class WorldGenDungeons : public WorldGenerator
{
public:
	bool generate(World* worldIn, pcg32& rand, const BlockPos& position) override;
private:
	static std::shared_ptr<spdlog::logger> LOGGER;
	static std::vector<ResourceLocation> SPAWNERTYPES = { EntityList.getKey(EntitySkeleton.class), EntityList.getKey(EntityZombie.class), EntityList.getKey(EntityZombie.class), EntityList.getKey(EntitySpider.class) };
	static ResourceLocation pickMobSpawner(pcg32& rand);
};
