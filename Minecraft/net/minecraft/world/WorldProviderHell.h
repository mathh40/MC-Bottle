#pragma once
#include "WorldProvider.h"
#include "border/WorldBorder.h"

class WorldProviderHell :public WorldProvider
{
public:
	void init() override;
	Vec3d getFogColor(float p_76562_1_, float p_76562_2_) const override;
	IChunkGenerator& createChunkGenerator() override;
	bool isSurfaceWorld() const override;
	bool canCoordinateBeSpawn(int32_t x, int32_t z) override;
	float calculateCelestialAngle(int64_t worldTime, float partialTicks) const override;
	bool canRespawnHere() const override;
	bool doesXZShowFog(int32_t x, int32_t z) const override;
	WorldBorder createWorldBorder() override;
	DimensionType& getDimensionType() override;
protected:
	void generateLightBrightnessTable() override;
private:
};
