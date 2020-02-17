#pragma once
#include "WorldInfo.h"

class DerivedWorldInfo : public WorldInfo
{
public:
	explicit DerivedWorldInfo(WorldInfo worldInfoIn);

	std::unique_ptr<NBTTagCompound> cloneNBTCompound(NBTTagCompound* nbt) override;

	int64_t getSeed() const override;

	int32_t getSpawnX() const override;

	int32_t getSpawnY() const override;

	int32_t getSpawnZ() const override;

	int64_t getWorldTotalTime() const override;

	int64_t getWorldTime() const override;

	int64_t getSizeOnDisk() const override;

	NBTTagCompound* getPlayerNBTTagCompound() const override;

	std::string getWorldName() const override;

	int32_t getSaveVersion() const override;

	int64_t getLastTimePlayed() const override;

	bool isThundering() const override;

	int32_t getThunderTime() const override;

	bool isRaining() const override;

	int32_t getRainTime() const override;

	GameType getGameType() const override;

	void setSpawnX(int x) override;

	void setSpawnY(int y) override;

	void setSpawnZ(int z) override;

	void setWorldTotalTime(int64_t time) override;

	void setWorldTime(int64_t time) override;

	void setSpawn(const BlockPos& spawnPoint) override;

	void setWorldName(std::string_view worldName) override;

	void setSaveVersion(int32_t version) override;

	void setThundering(bool thunderingIn) override;

	void setThunderTime(int32_t time) override;

	void setRaining(bool isRaining) override;

	void setRainTime(int32_t time) override;

	bool isMapFeaturesEnabled() const override;

	bool isHardcoreModeEnabled() const override;

	WorldType getTerrainType() const override;

	void setTerrainType(WorldType type) override;

	bool areCommandsAllowed() const override;

	void setAllowCommands(bool allow) override;

	bool isInitialized() const override;

	void setServerInitialized(bool initializedIn) override;

	GameRules getGameRulesInstance() const override;

	EnumDifficulty getDifficulty() const override;

	void setDifficulty(EnumDifficulty newDifficulty) override;

	bool isDifficultyLocked() const override;

	void setDifficultyLocked(bool locked) override;

	void setDimensionData(DimensionType dimensionIn, NBTTagCompound* compound) override;

	std::unique_ptr<NBTTagCompound> getDimensionData(DimensionType dimensionIn) override;
private:
	WorldInfo delegate;
};
