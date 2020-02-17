#pragma once
#include <cstdint>
#include <vector>
#include <memory>
#include <optional>

class ChunkGeneratorSettings;
class WorldType;

class GenLayer
{
public:
	explicit GenLayer(int64_t baseSeedIn);
	virtual void initWorldGenSeed(int64_t seed);
	void initChunkSeed(int64_t chunkSeedIn, int64_t chunkSeedIn2);
	virtual std::vector<int32_t> getInts(int32_t areaX, int32_t areaY, int32_t areaWidth, int32_t areaHeight) = 0;
	std::vector<std::shared_ptr<GenLayer>> initializeAllBiomeGenerators(int64_t seed, WorldType p_180781_2_, std::optional<ChunkGeneratorSettings> p_180781_3_);
protected:
	std::shared_ptr<GenLayer> parent;
	int64_t baseSeed;
	int32_t nextInt(int32_t value);
	bool biomesEqualOrMesaPlateau(int32_t biomeIDA, int32_t biomeIDB);
	bool isBiomeOceanic(int32_t biomeID);
	int32_t selectRandom(const std::vector<int32_t>& p_151619_1_);
	virtual int32_t selectModeOrRandom(int32_t p_151617_1_, int32_t p_151617_2_, int32_t p_151617_3_, int32_t p_151617_4_);
private:
	int64_t worldGenSeed;
	int64_t chunkSeed;
};
