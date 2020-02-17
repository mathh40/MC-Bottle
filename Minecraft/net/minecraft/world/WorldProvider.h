#pragma once
#include "World.h"
#include "border/WorldBorder.h"

class EntityPlayerMP;

class WorldProvider
{
public:
	virtual ~WorldProvider() = default;
	static std::array<float,8> MOON_PHASE_FACTORS;

	void setWorld(World* worldIn);
	virtual IChunkGenerator& createChunkGenerator();
	virtual bool canCoordinateBeSpawn(int32_t x, int32_t z);
	virtual float calculateCelestialAngle(int64_t worldTime, float partialTicks) const;
	int32_t getMoonPhase(int64_t worldTime) const;
	virtual bool isSurfaceWorld() const;
	virtual std::optional<std::array<float, 4>> calcSunriseSunsetColors(float celestialAngle, float partialTicks);
	virtual Vec3d getFogColor(float p_76562_1_, float p_76562_2_) const;
	virtual bool canRespawnHere() const;
	virtual float getCloudHeight() const;
	virtual bool isSkyColored() const;
	virtual std::optional<BlockPos> getSpawnCoordinate() const;
	virtual int32_t getAverageGroundLevel() const;
	double getVoidFogYFactor() const;
	virtual bool doesXZShowFog(int32_t x, int32_t z) const;
	BiomeProvider& getBiomeProvider() const;
	bool doesWaterVaporize() const;
	bool hasSkyLight() const;
	bool isNether() const;
	std::array<float, 16> getLightBrightnessTable() const;
	virtual WorldBorder createWorldBorder();
	void onPlayerAdded(EntityPlayerMP* player);
	void onPlayerRemoved(EntityPlayerMP* player);
	virtual DimensionType& getDimensionType() = 0;
	virtual void onWorldSave();
	virtual void onWorldUpdateEntities();
	virtual bool canDropChunk(int32_t x, int32_t z);
protected:
	World* world;
	BiomeProvider biomeProvider;
	bool dWaterVaporize;
	bool nether;
	bool SkyLight;
	std::array<float, 16> lightBrightnessTable;
	virtual void generateLightBrightnessTable();
	virtual void init();
private:
	WorldType terrainType;
	std::string generatorSettings;

	std::array<float, 4>  colorsSunriseSunset;
};
