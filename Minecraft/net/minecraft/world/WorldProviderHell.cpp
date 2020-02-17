#include "WorldProviderHell.h"

void WorldProviderHell::init()
{
	biomeProvider = BiomeProviderSingle(Biomes::HELL);
	dWaterVaporize = true;
	nether = true;
}

Vec3d WorldProviderHell::getFogColor(float p_76562_1_, float p_76562_2_) const
{
	return Vec3d(0.20000000298023224, 0.029999999329447746, 0.029999999329447746);
}

void WorldProviderHell::generateLightBrightnessTable()
{
	auto f = 0.1F;

	for (auto i = 0; i <= 15; ++i) 
	{
		auto f1 = 1.0F - i / 15.0F;
		lightBrightnessTable[i] = (1.0F - f1) / (f1 * 3.0F + 1.0F) * 0.9F + 0.1F;
	}
}

IChunkGenerator& WorldProviderHell::createChunkGenerator()
{
	return ChunkGeneratorHell(world, world->getWorldInfo().isMapFeaturesEnabled(), world->getSeed());
}

bool WorldProviderHell::isSurfaceWorld() const
{
	return false;
}

bool WorldProviderHell::canCoordinateBeSpawn(int32_t x, int32_t z)
{
	return false;
}

float WorldProviderHell::calculateCelestialAngle(int64_t worldTime, float partialTicks) const
{
	return 0.5F;
}

bool WorldProviderHell::canRespawnHere() const
{
	return false;
}

bool WorldProviderHell::doesXZShowFog(int32_t x, int32_t z) const
{
	return true;
}

WorldBorder WorldProviderHell::createWorldBorder()
{
	return WorldBorder()
	{
		 public double getCenterX() {
			return super.getCenterX() / 8.0D;
		 }

		 public double getCenterZ() {
			return super.getCenterZ() / 8.0D;
		 }
	};
}

DimensionType& WorldProviderHell::getDimensionType()
{
	return DimensionType::NETHER;
}
