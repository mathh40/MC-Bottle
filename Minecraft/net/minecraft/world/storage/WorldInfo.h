#include "EnumDifficulty.h"
#include "GameRules.h"
#include "WorldType.h"
#include "GameType.h"
#include "DimensionType.h"

class BlockPos;
class WorldSettings;
class DataFixer;

class WorldInfo
{
public:
	static EnumDifficulty DEFAULT_DIFFICULTY;
	static void registerFixes(DataFixer fixer);
	explicit WorldInfo(NBTTagCompound* nbt);
	WorldInfo(WorldSettings settings, std::string name);
	void populateFromWorldSettings(WorldSettings settings);
	explicit WorldInfo(const WorldInfo& worldInformation);
	virtual std::unique_ptr<NBTTagCompound> cloneNBTCompound(NBTTagCompound* nbt);

	virtual int64_t getSeed() const;
	virtual int32_t getSpawnX() const;
	virtual int32_t getSpawnY() const;
	virtual int32_t getSpawnZ() const;
	virtual int64_t getWorldTotalTime() const;
	virtual int64_t getWorldTime() const;
	virtual int64_t getSizeOnDisk() const;
	virtual NBTTagCompound* getPlayerNBTTagCompound() const;
	virtual void setSpawnX(int32_t x);
	virtual void setSpawnY(int32_t y);
	virtual void setSpawnZ(int32_t z);
	virtual void setWorldTotalTime(int64_t time);
	virtual void setWorldTime(int64_t time);
	virtual void setSpawn(const BlockPos& spawnPoint);
	virtual std::string getWorldName() const;
	virtual void setWorldName(std::string_view worldName);
	virtual int32_t getSaveVersion() const;
	virtual void setSaveVersion(int32_t version);
	virtual int64_t getLastTimePlayed() const;
	int32_t getCleanWeatherTime() const;
	void setCleanWeatherTime(int32_t cleanWeatherTimeIn);
	virtual bool isThundering() const;
	virtual void setThundering(bool thunderingIn);
	virtual int32_t getThunderTime() const;
	virtual void setThunderTime(int32_t time);
	virtual bool isRaining() const;
	virtual void setRaining(bool isRaining);
	virtual int32_t getRainTime() const;
	virtual void setRainTime(int32_t time);
	virtual GameType getGameType() const;
	virtual bool isMapFeaturesEnabled() const;
	void setMapFeaturesEnabled(bool enabled);
	void setGameType(GameType type);
	virtual bool isHardcoreModeEnabled() const;
	void setHardcore(bool hardcoreIn);
	virtual WorldType getTerrainType() const;
	virtual void setTerrainType(WorldType type);
	std::string getGeneratorOptions() const;
	virtual bool areCommandsAllowed() const;
	virtual void setAllowCommands(bool allow);
	virtual bool isInitialized() const;
	virtual void setServerInitialized(bool initializedIn);
	virtual GameRules getGameRulesInstance() const;
	double getBorderCenterX() const;
	double getBorderSize() const;
	double getBorderCenterZ() const;
	void setBorderSize(double size);
	int64_t getBorderLerpTime() const;
	void setBorderLerpTime(int64_t time);
	double getBorderLerpTarget() const;
	void setBorderLerpTarget(double lerpSize);
	void setBorderCenterZ(double posZ);
	void setBorderCenterX(double posX);
	double getBorderSafeZone() const;
	void setBorderSafeZone(double amount);
	double getBorderDamagePerBlock() const;
	void setBorderDamagePerBlock(double damage);
	int32_t getBorderWarningDistance() const;
	int32_t getBorderWarningTime() const;
	void setBorderWarningDistance(int32_t amountOfBlocks);
	void setBorderWarningTime(int32_t ticks);
	virtual EnumDifficulty getDifficulty() const;
	virtual void setDifficulty(EnumDifficulty newDifficulty);
	virtual bool isDifficultyLocked() const;
	virtual void setDifficultyLocked(bool locked);
	void addToCrashReport(CrashReportCategory category);
	virtual std::unique_ptr<NBTTagCompound> getDimensionData(DimensionType dimensionIn);
	virtual void setDimensionData(DimensionType dimensionIn, NBTTagCompound* compound);
	int32_t getVersionId() const;
	bool isVersionSnapshot() const;
	std::string getVersionName() const;
	WorldInfo();
private:
	std::string versionName;
	int32_t versionId;
	bool versionSnapshot;
	int64_t randomSeed;
	WorldType terrainType;
	std::string generatorOptions;
	int32_t spawnX;
	int32_t spawnY;
	int32_t spawnZ;
	int64_t totalTime;
	int64_t worldTime;
	int64_t lastTimePlayed;
	int64_t sizeOnDisk;
	std::unique_ptr<NBTTagCompound> playerTag;
	int32_t dimension;
	std::string levelName;
	int32_t saveVersion;
	int32_t cleanWeatherTime;
	bool raining;
	int32_t rainTime;
	bool thundering;
	int32_t thunderTime;
	GameType gameType;
	bool mapFeaturesEnabled;
	bool hardcore;
	bool allowCommands;
	bool initialized;
	EnumDifficulty difficulty = EnumDifficulty::NORMAL;
	bool difficultyLocked;
	double borderCenterX;
	double borderCenterZ;
	double borderSize;
	int64_t borderSizeLerpTime;
	double borderSizeLerpTarget;
	double borderSafeZone;
	double borderDamagePerBlock;
	int32_t borderWarningDistance;
	int32_t borderWarningTime;
	std::unordered_map<DimensionType, std::unique_ptr<NBTTagCompound>> dimensionData;
	GameRules gameRules;

	void updateTagCompound(NBTTagCompound* nbt, NBTTagCompound* playerNbt);
};
