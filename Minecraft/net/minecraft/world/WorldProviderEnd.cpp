#include "WorldProviderEnd.h"
#include "WorldEntitySpawner.h"

void WorldProviderEnd::init()
{
	biomeProvider = BiomeProviderSingle(Biomes::SKY);
	auto nbttagcompound = world->getWorldInfo().getDimensionData(DimensionType::THE_END);
	dragonFightManager = Util::instanceof<WorldServer>(world) ? std::make_unique<DragonFightManager>((WorldServer)world, nbttagcompound.getCompoundTag("DragonFight")) : nullptr;
}

IChunkGenerator WorldProviderEnd::createChunkGenerator()
{
	return new ChunkGeneratorEnd(world, world->getWorldInfo().isMapFeaturesEnabled(), world->getSeed(), getSpawnCoordinate());
}

float WorldProviderEnd::calculateCelestialAngle(int64_t worldTime, float partialTicks) const
{
	return 0.0F;
}

std::optional<std::array<float, 4>> WorldProviderEnd::calcSunriseSunsetColors(float celestialAngle, float partialTicks)
{
	return std::nullopt;
}

Vec3d WorldProviderEnd::getFogColor(float p_76562_1_, float p_76562_2_) const
{
	auto i = 10518688;
	auto f = MathHelper::cos(p_76562_1_ * 6.2831855F) * 2.0F + 0.5F;
	f = MathHelper::clamp(f, 0.0F, 1.0F);
	auto f1 = 0.627451F;
	auto f2 = 0.5019608F;
	auto f3 = 0.627451F;
	f1 *= f * 0.0F + 0.15F;
	f2 *= f * 0.0F + 0.15F;
	f3 *= f * 0.0F + 0.15F;
	return Vec3d(f1, f2, f3);
}

bool WorldProviderEnd::isSkyColored() const
{
	return false;
}

bool WorldProviderEnd::canRespawnHere() const
{
	return false;
}

bool WorldProviderEnd::isSurfaceWorld() const
{
	return false;
}

float WorldProviderEnd::getCloudHeight() const
{
	return 8.0F;
}

bool WorldProviderEnd::canCoordinateBeSpawn(int32_t x, int32_t z)
{
	return world->getGroundAboveSeaLevel(BlockPos(x, 0, z)).getMaterial().blocksMovement();
}

std::optional<BlockPos> WorldProviderEnd::getSpawnCoordinate() const
{
	return BlockPos(100, 50, 0);
}

int32_t WorldProviderEnd::getAverageGroundLevel() const
{
	return 50;
}

bool WorldProviderEnd::doesXZShowFog(int32_t x, int32_t z) const
{
	return false;
}

DimensionType& WorldProviderEnd::getDimensionType()
{
	return DimensionType::THE_END;
}

void WorldProviderEnd::onWorldSave()
{
	auto nbttagcompound = std::make_unique<NBTTagCompound>();
	if (dragonFightManager != nullptr) 
	{
		nbttagcompound->setTag("DragonFight", dragonFightManager.getCompound());
	}

	world->getWorldInfo().setDimensionData(DimensionType::THE_END, nbttagcompound);
}

void WorldProviderEnd::onWorldUpdateEntities()
{
	if (dragonFightManager != nullptr) 
	{
		dragonFightManager.tick();
	}
}

DragonFightManager* WorldProviderEnd::getDragonFightManager()
{
	return dragonFightManager.get();
}
