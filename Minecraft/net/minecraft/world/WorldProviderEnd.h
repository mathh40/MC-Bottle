#pragma once
#include "WorldProvider.h"
#include "end/DragonFightManager.h"

class WorldProviderEnd :public WorldProvider
{
public:
	void init() override;
	IChunkGenerator* createChunkGenerator() override;
	float calculateCelestialAngle(int64_t worldTime, float partialTicks) const override;
	std::optional<std::array<float, 4>> calcSunriseSunsetColors(float celestialAngle, float partialTicks) override;
	Vec3d getFogColor(float p_76562_1_, float p_76562_2_) const override;
	bool isSkyColored() const override;
	bool canRespawnHere() const override;
	bool isSurfaceWorld() const override;
	float getCloudHeight() const override;
	bool canCoordinateBeSpawn(int32_t x, int32_t z) override;
	std::optional<BlockPos> getSpawnCoordinate() const override;
	int32_t getAverageGroundLevel() const override;
	bool doesXZShowFog(int32_t x, int32_t z) const override;
	DimensionType& getDimensionType() override;
	void onWorldSave() override;
	void onWorldUpdateEntities() override;
	DragonFightManager* getDragonFightManager();
private:
	std::unique_ptr<DragonFightManager> dragonFightManager;
};
