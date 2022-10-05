#include "WorldInfo.h"

#include "WorldSettings.h"
#include "datafix/DataFixer.h"
#include "datafix/FixTypes.h"
#include "datafix/IDataWalker.h"
#include "math/BlockPos.h"

EnumDifficulty WorldInfo::DEFAULT_DIFFICULTY = EnumDifficulty::NORMAL;

class Playerwalker : public IDataWalker
{
  public:
    Playerwalker(DataFixer fixer) : fixer(fixer)
    {
    }
    std::unique_ptr<NBTTagCompound> process(IDataFixer *type, std::unique_ptr<NBTTagCompound> compound,
                                            int32_t versionIn) override
    {
        if (compound->hasKey("Player", 10))
        {
            compound->setTag("Player", fixer.process(FixTypes::PLAYER, compound->getCompoundTag("Player"), versionIn));
        }

        return compound;
    }

  private:
    DataFixer fixer;
};

void WorldInfo::registerFixes(DataFixer fixer)
{
    fixer.registerWalker(FixTypes::LEVEL, new Playerwalker());
}

WorldInfo::WorldInfo(NBTTagCompound *nbt) : gameType(GameType::SURVIVAL), terrainType(WorldType::DEFAULT)
{
    generatorOptions                = "";
    borderSize                      = 6.0E7;
    borderSafeZone                  = 5.0;
    borderDamagePerBlock            = 0.2;
    borderWarningDistance           = 5;
    borderWarningTime               = 15;
    NBTTagCompound *nbttagcompound1 = nullptr;
    if (nbt->hasKey("Version", 10))
    {
        nbttagcompound1 = nbt->getCompoundTag("Version");
        versionName     = nbttagcompound1->getString("Name");
        versionId       = nbttagcompound1->getInteger("Id");
        versionSnapshot = nbttagcompound1->getBoolean("Snapshot");
    }

    randomSeed = nbt->getLong("RandomSeed");
    if (nbt->hasKey("generatorName", 8))
    {
        auto s1            = nbt->getString("generatorName");
        auto terrainTypeIn = WorldType::byName(s1);

        terrainType = terrainTypeIn == std::nullopt ? terrainTypeIn.value() : WorldType::DEFAULT;
        if (terrainType.isVersioned())
        {
            auto i = 0;
            if (nbt->hasKey("generatorVersion", 99))
            {
                i = nbt->getInteger("generatorVersion");
            }

            terrainType = terrainType.getWorldTypeForGeneratorVersion(i);
        }

        if (nbt->hasKey("generatorOptions", 8))
        {
            generatorOptions = nbt->getString("generatorOptions");
        }
    }

    gameType = GameType::getByID(nbt->getInteger("GameType"));
    if (nbt->hasKey("MapFeatures", 99))
    {
        mapFeaturesEnabled = nbt->getBoolean("MapFeatures");
    }
    else
    {
        mapFeaturesEnabled = true;
    }

    spawnX    = nbt->getInteger("SpawnX");
    spawnY    = nbt->getInteger("SpawnY");
    spawnZ    = nbt->getInteger("SpawnZ");
    totalTime = nbt->getLong("Time");
    if (nbt->hasKey("DayTime", 99))
    {
        worldTime = nbt->getLong("DayTime");
    }
    else
    {
        worldTime = totalTime;
    }

    lastTimePlayed   = nbt->getLong("LastPlayed");
    sizeOnDisk       = nbt->getLong("SizeOnDisk");
    levelName        = nbt->getString("LevelName");
    saveVersion      = nbt->getInteger("version");
    cleanWeatherTime = nbt->getInteger("clearWeatherTime");
    rainTime         = nbt->getInteger("rainTime");
    raining          = nbt->getBoolean("raining");
    thunderTime      = nbt->getInteger("thunderTime");
    thundering       = nbt->getBoolean("thundering");
    hardcore         = nbt->getBoolean("hardcore");
    if (nbt->hasKey("initialized", 99))
    {
        initialized = nbt->getBoolean("initialized");
    }
    else
    {
        initialized = true;
    }

    if (nbt->hasKey("allowCommands", 99))
    {
        allowCommands = nbt->getBoolean("allowCommands");
    }
    else
    {
        allowCommands = gameType == GameType::CREATIVE;
    }

    if (nbt->hasKey("Player", 10))
    {
        playerTag = nbt->getCompoundTag("Player");
        dimension = playerTag->getInteger("Dimension");
    }

    if (nbt->hasKey("GameRules", 10))
    {
        gameRules.readFromNBT(nbt->getCompoundTag("GameRules"));
    }

    if (nbt->hasKey("Difficulty", 99))
    {
        difficulty = EnumDifficulty::byId(nbt->getByte("Difficulty"));
    }

    if (nbt->hasKey("DifficultyLocked", 1))
    {
        difficultyLocked = nbt->getBoolean("DifficultyLocked");
    }

    if (nbt->hasKey("BorderCenterX", 99))
    {
        borderCenterX = nbt->getDouble("BorderCenterX");
    }

    if (nbt->hasKey("BorderCenterZ", 99))
    {
        borderCenterZ = nbt->getDouble("BorderCenterZ");
    }

    if (nbt->hasKey("BorderSize", 99))
    {
        borderSize = nbt->getDouble("BorderSize");
    }

    if (nbt->hasKey("BorderSizeLerpTime", 99))
    {
        borderSizeLerpTime = nbt->getLong("BorderSizeLerpTime");
    }

    if (nbt->hasKey("BorderSizeLerpTarget", 99))
    {
        borderSizeLerpTarget = nbt->getDouble("BorderSizeLerpTarget");
    }

    if (nbt->hasKey("BorderSafeZone", 99))
    {
        borderSafeZone = nbt->getDouble("BorderSafeZone");
    }

    if (nbt->hasKey("BorderDamagePerBlock", 99))
    {
        borderDamagePerBlock = nbt->getDouble("BorderDamagePerBlock");
    }

    if (nbt->hasKey("BorderWarningBlocks", 99))
    {
        borderWarningDistance = nbt->getInteger("BorderWarningBlocks");
    }

    if (nbt->hasKey("BorderWarningTime", 99))
    {
        borderWarningTime = nbt->getInteger("BorderWarningTime");
    }

    if (nbt->hasKey("DimensionData", 10))
    {
        nbttagcompound1 = nbt->getCompoundTag("DimensionData");
        for (auto s : nbttagcompound1->getCompoundMap())
        {
            dimensionData.emplace(DimensionType::getById(std::stoi(s)), nbttagcompound1->getCompoundTag(s));
        }
    }
}

WorldInfo::WorldInfo(WorldSettings settings, std::string name)
    : gameType(GameType::SURVIVAL), terrainType(WorldType::DEFAULT)
{
    generatorOptions      = "";
    borderSize            = 6.0E7;
    borderSafeZone        = 5.0;
    borderDamagePerBlock  = 0.2;
    borderWarningDistance = 5;
    borderWarningTime     = 15;
    populateFromWorldSettings(settings);
    levelName   = name;
    difficulty  = DEFAULT_DIFFICULTY;
    initialized = false;
}

void WorldInfo::populateFromWorldSettings(WorldSettings settings)
{
    randomSeed         = settings.getSeed();
    gameType           = settings.getGameType();
    mapFeaturesEnabled = settings.isMapFeaturesEnabled();
    hardcore           = settings.getHardcoreEnabled();
    terrainType        = settings.getTerrainType();
    generatorOptions   = settings.getGeneratorOptions();
    allowCommands      = settings.areCommandsAllowed();
}

WorldInfo::WorldInfo(const WorldInfo &worldInformation) : gameType(GameType::SURVIVAL), terrainType(WorldType::DEFAULT)
{
    generatorOptions      = "";
    borderSize            = 6.0E7;
    borderSafeZone        = 5.0;
    borderDamagePerBlock  = 0.2;
    borderWarningDistance = 5;
    borderWarningTime     = 15;
    randomSeed            = worldInformation.randomSeed;
    terrainType           = worldInformation.terrainType;
    generatorOptions      = worldInformation.generatorOptions;
    gameType              = worldInformation.gameType;
    mapFeaturesEnabled    = worldInformation.mapFeaturesEnabled;
    spawnX                = worldInformation.spawnX;
    spawnY                = worldInformation.spawnY;
    spawnZ                = worldInformation.spawnZ;
    totalTime             = worldInformation.totalTime;
    worldTime             = worldInformation.worldTime;
    lastTimePlayed        = worldInformation.lastTimePlayed;
    sizeOnDisk            = worldInformation.sizeOnDisk;
    playerTag             = std::move(worldInformation.playerTag);
    dimension             = worldInformation.dimension;
    levelName             = worldInformation.levelName;
    saveVersion           = worldInformation.saveVersion;
    rainTime              = worldInformation.rainTime;
    raining               = worldInformation.raining;
    thunderTime           = worldInformation.thunderTime;
    thundering            = worldInformation.thundering;
    hardcore              = worldInformation.hardcore;
    allowCommands         = worldInformation.allowCommands;
    initialized           = worldInformation.initialized;
    gameRules             = worldInformation.gameRules;
    difficulty            = worldInformation.difficulty;
    difficultyLocked      = worldInformation.difficultyLocked;
    borderCenterX         = worldInformation.borderCenterX;
    borderCenterZ         = worldInformation.borderCenterZ;
    borderSize            = worldInformation.borderSize;
    borderSizeLerpTime    = worldInformation.borderSizeLerpTime;
    borderSizeLerpTarget  = worldInformation.borderSizeLerpTarget;
    borderSafeZone        = worldInformation.borderSafeZone;
    borderDamagePerBlock  = worldInformation.borderDamagePerBlock;
    borderWarningTime     = worldInformation.borderWarningTime;
    borderWarningDistance = worldInformation.borderWarningDistance;
}

std::unique_ptr<NBTTagCompound> WorldInfo::cloneNBTCompound(NBTTagCompound *nbt)
{
    if (nbt == nullptr)
    {
        nbt = playerTag.get();
    }

    auto nbttagcompound = std::make_unique<NBTTagCompound>();
    updateTagCompound(nbttagcompound, nbt);
    return nbttagcompound;
}

int64_t WorldInfo::getSeed() const
{
    return randomSeed;
}

int32_t WorldInfo::getSpawnX() const
{
    return spawnX;
}

int32_t WorldInfo::getSpawnY() const
{
    return spawnY;
}

int32_t WorldInfo::getSpawnZ() const
{
    return spawnZ;
}

int64_t WorldInfo::getWorldTotalTime() const
{
    return totalTime;
}

int64_t WorldInfo::getWorldTime() const
{
    return worldTime;
}

int64_t WorldInfo::getSizeOnDisk() const
{
    return sizeOnDisk;
}

NBTTagCompound *WorldInfo::getPlayerNBTTagCompound() const
{
    return playerTag.get();
}

void WorldInfo::setSpawnX(int32_t x)
{
    spawnX = x;
}

void WorldInfo::setSpawnY(int32_t y)
{
    spawnY = y;
}

void WorldInfo::setSpawnZ(int32_t z)
{
    spawnZ = z;
}

void WorldInfo::setWorldTotalTime(int64_t time)
{
    totalTime = time;
}

void WorldInfo::setWorldTime(int64_t time)
{
    worldTime = time;
}

void WorldInfo::setSpawn(const BlockPos &spawnPoint)
{
    spawnX = spawnPoint.getx();
    spawnY = spawnPoint.gety();
    spawnZ = spawnPoint.getz();
}

std::string WorldInfo::getWorldName() const
{
    return levelName;
}

void WorldInfo::setWorldName(std::string_view worldName)
{
    levelName = worldName;
}

int32_t WorldInfo::getSaveVersion() const
{
    return saveVersion;
}

void WorldInfo::setSaveVersion(int32_t version)
{
    saveVersion = version;
}

int64_t WorldInfo::getLastTimePlayed() const
{
    return lastTimePlayed;
}

int32_t WorldInfo::getCleanWeatherTime() const
{
    return cleanWeatherTime;
}

void WorldInfo::setCleanWeatherTime(int32_t cleanWeatherTimeIn)
{
    cleanWeatherTime = cleanWeatherTimeIn;
}

bool WorldInfo::isThundering() const
{
    return thundering;
}

void WorldInfo::setThundering(bool thunderingIn)
{
    thundering = thunderingIn;
}

int32_t WorldInfo::getThunderTime() const
{
    return thunderTime;
}

void WorldInfo::setThunderTime(int32_t time)
{
    thunderTime = time;
}

bool WorldInfo::isRaining() const
{
    return raining;
}

void WorldInfo::setRaining(bool isRaining)
{
    raining = isRaining;
}

int32_t WorldInfo::getRainTime() const
{
    return rainTime;
}

void WorldInfo::setRainTime(int32_t time)
{
    rainTime = time;
}

GameType WorldInfo::getGameType() const
{
    return gameType;
}

bool WorldInfo::isMapFeaturesEnabled() const
{
    return mapFeaturesEnabled;
}

void WorldInfo::setMapFeaturesEnabled(bool enabled)
{
    mapFeaturesEnabled = enabled;
}

void WorldInfo::setGameType(GameType type)
{
    gameType = type;
}

bool WorldInfo::isHardcoreModeEnabled() const
{
    return hardcore;
}

void WorldInfo::setHardcore(bool hardcoreIn)
{
    hardcore = hardcoreIn;
}

WorldType WorldInfo::getTerrainType() const
{
    return terrainType;
}

void WorldInfo::setTerrainType(WorldType type)
{
    terrainType = type;
}

std::string WorldInfo::getGeneratorOptions() const
{
    return generatorOptions;
}

bool WorldInfo::areCommandsAllowed() const
{
    return allowCommands;
}

void WorldInfo::setAllowCommands(bool allow)
{
    allowCommands = allow;
}

bool WorldInfo::isInitialized() const
{
    return initialized;
}

void WorldInfo::setServerInitialized(bool initializedIn)
{
    initialized = initializedIn;
}

GameRules WorldInfo::getGameRulesInstance() const
{
    return gameRules;
}

double WorldInfo::getBorderCenterX() const
{
    return borderCenterX;
}

double WorldInfo::getBorderSize() const
{
    return borderSize;
}

double WorldInfo::getBorderCenterZ() const
{
    return borderCenterZ;
}

void WorldInfo::setBorderSize(double size)
{
    borderSize = size;
}

int64_t WorldInfo::getBorderLerpTime() const
{
    return borderSizeLerpTime;
}

void WorldInfo::setBorderLerpTime(int64_t time)
{
    borderSizeLerpTime = time;
}

double WorldInfo::getBorderLerpTarget() const
{
    return borderSizeLerpTarget;
}

void WorldInfo::setBorderLerpTarget(double lerpSize)
{
    borderSizeLerpTarget = lerpSize;
}

void WorldInfo::setBorderCenterZ(double posZ)
{
    borderCenterZ = posZ;
}

void WorldInfo::setBorderCenterX(double posX)
{
    borderCenterX = posX;
}

double WorldInfo::getBorderSafeZone() const
{
    return borderSafeZone;
}

void WorldInfo::setBorderSafeZone(double amount)
{
    borderSafeZone = amount;
}

double WorldInfo::getBorderDamagePerBlock() const
{
    return borderDamagePerBlock;
}

void WorldInfo::setBorderDamagePerBlock(double damage)
{
    borderDamagePerBlock = damage;
}

int32_t WorldInfo::getBorderWarningDistance() const
{
    return borderWarningDistance;
}

int32_t WorldInfo::getBorderWarningTime() const
{
    return borderWarningTime;
}

void WorldInfo::setBorderWarningDistance(int32_t amountOfBlocks)
{
    borderWarningDistance = amountOfBlocks;
}

void WorldInfo::setBorderWarningTime(int32_t ticks)
{
    borderWarningTime = ticks;
}

EnumDifficulty WorldInfo::getDifficulty() const
{
    return difficulty;
}

void WorldInfo::setDifficulty(EnumDifficulty newDifficulty)
{
    difficulty = std::move(newDifficulty);
}

bool WorldInfo::isDifficultyLocked() const
{
    return difficultyLocked;
}

void WorldInfo::setDifficultyLocked(bool locked)
{
    difficultyLocked = locked;
}

void WorldInfo::addToCrashReport(CrashReportCategory category)
{
    category.addDetail("Level seed", new ICrashReportDetail(){
		 public String call() throws Exception {
			return String.valueOf(WorldInfo.this.getSeed());
}
});
    category.addDetail("Level generator", new ICrashReportDetail(){
	   public String call() throws Exception {
		  return String.format("ID %02d - %s, ver %d. Features enabled: %b", WorldInfo.this.terrainType.getId(), WorldInfo.this.terrainType.getName(), WorldInfo.this.terrainType.getVersion(), WorldInfo.this.mapFeaturesEnabled);
    }
    });
    category.addDetail("Level generator options", new ICrashReportDetail(){
	   public String call() throws Exception {
		  return WorldInfo.this.generatorOptions;
    }
    });
    category.addDetail("Level spawn location", new ICrashReportDetail(){
	   public String call() throws Exception {
		  return CrashReportCategory.getCoordinateInfo(WorldInfo.this.spawnX, WorldInfo.this.spawnY, WorldInfo.this.spawnZ);
    }
    });
    category.addDetail("Level time", new ICrashReportDetail(){
	   public String call() throws Exception {
		  return String.format("%d game time, %d day time", WorldInfo.this.totalTime, WorldInfo.this.worldTime);
    }
    });
    category.addDetail("Level dimension", new ICrashReportDetail(){
	   public String call() throws Exception {
		  return String.valueOf(WorldInfo.this.dimension);
    }
    });
    category.addDetail("Level storage version", new ICrashReportDetail(){
	   public String call() throws Exception {
		  String s = "Unknown?";

		  try {
    switch (WorldInfo.this.saveVersion)
    {
    case 19132:
        s = "McRegion";
        break;
    case 19133:
        s = "Anvil";
    }
		  }
catch (Throwable var3) {
}

return String.format("0x%05X - %s", WorldInfo.this.saveVersion, s);
    }
    });
    category.addDetail("Level weather", new ICrashReportDetail(){
	   public String call() throws Exception {
		  return String.format("Rain time: %d (now: %b), thunder time: %d (now: %b)", WorldInfo.this.rainTime, WorldInfo.this.raining, WorldInfo.this.thunderTime, WorldInfo.this.thundering);
    }
    });
    category.addDetail("Level game mode", new ICrashReportDetail(){
	   public String call() throws Exception {
		  return String.format("Game mode: %s (ID %d). Hardcore: %b. Cheats: %b", WorldInfo.this.gameType.getName(), WorldInfo.this.gameType.getID(), WorldInfo.this.hardcore, WorldInfo.this.allowCommands);
    }
    });
    }

    std::unique_ptr<NBTTagCompound> WorldInfo::getDimensionData(DimensionType dimensionIn)
    {
        auto nbttagcompound = dimensionData[dimensionIn];
        return nbttagcompound == nullptr ? std::make_unique<NBTTagCompound>() : nbttagcompound;
    }

    void WorldInfo::setDimensionData(DimensionType dimensionIn, NBTTagCompound *compound)
    {
        dimensionData.emplace(dimensionIn, compound);
    }

    int32_t WorldInfo::getVersionId() const
    {
        return versionId;
    }

    bool WorldInfo::isVersionSnapshot() const
    {
        return versionSnapshot;
    }

    std::string WorldInfo::getVersionName() const
    {
        return versionName;
    }

    WorldInfo::WorldInfo()
        : terrainType(WorldType::DEFAULT), generatorOptions(""), borderSize(6.0E7), borderSafeZone(5.0),
          borderDamagePerBlock(0.2), borderWarningDistance(5), borderWarningTime(15), gameType(GameType::SURVIVAL)
    {
    }

    void WorldInfo::updateTagCompound(NBTTagCompound *nbt, NBTTagCompound *playerNbt)
    {
        auto nbttagcompound = std::make_unique<NBTTagCompound>();
        nbttagcompound->setString("Name", "1.12.2");
        nbttagcompound->setInteger("Id", 1343);
        nbttagcompound->setBoolean("Snapshot", false);
        nbt->setTag("Version", nbttagcompound);
        nbt->setInteger("DataVersion", 1343);
        nbt->setLong("RandomSeed", randomSeed);
        nbt->setString("generatorName", terrainType.getName());
        nbt->setInteger("generatorVersion", terrainType.getVersion());
        nbt->setString("generatorOptions", generatorOptions);
        nbt->setInteger("GameType", gameType.getID());
        nbt->setBoolean("MapFeatures", mapFeaturesEnabled);
        nbt->setInteger("SpawnX", spawnX);
        nbt->setInteger("SpawnY", spawnY);
        nbt->setInteger("SpawnZ", spawnZ);
        nbt->setLong("Time", totalTime);
        nbt->setLong("DayTime", worldTime);
        nbt->setLong("SizeOnDisk", sizeOnDisk);
        nbt->setLong("LastPlayed", MinecraftServer::getCurrentTimeMillis());
        nbt->setString("LevelName", levelName);
        nbt->setInteger("version", saveVersion);
        nbt->setInteger("clearWeatherTime", cleanWeatherTime);
        nbt->setInteger("rainTime", rainTime);
        nbt->setBoolean("raining", raining);
        nbt->setInteger("thunderTime", thunderTime);
        nbt->setBoolean("thundering", thundering);
        nbt->setBoolean("hardcore", hardcore);
        nbt->setBoolean("allowCommands", allowCommands);
        nbt->setBoolean("initialized", initialized);
        nbt->setDouble("BorderCenterX", borderCenterX);
        nbt->setDouble("BorderCenterZ", borderCenterZ);
        nbt->setDouble("BorderSize", borderSize);
        nbt->setLong("BorderSizeLerpTime", borderSizeLerpTime);
        nbt->setDouble("BorderSafeZone", borderSafeZone);
        nbt->setDouble("BorderDamagePerBlock", borderDamagePerBlock);
        nbt->setDouble("BorderSizeLerpTarget", borderSizeLerpTarget);
        nbt->setDouble("BorderWarningBlocks", (double)borderWarningDistance);
        nbt->setDouble("BorderWarningTime", (double)borderWarningTime);
        nbt->setByte("Difficulty", difficulty.getId());

        nbt->setBoolean("DifficultyLocked", difficultyLocked);
        nbt->setTag("GameRules", gameRules.writeToNBT());
        auto nbttagcompound1 = std::make_unique<NBTTagCompound>();

        for (auto entry : dimensionData)
        {
            nbttagcompound1->setTag(String.valueOf((entry.first()).getId()), entry.second());
        }

        nbt->setTag("DimensionData", nbttagcompound1);
        if (playerNbt != nullptr)
        {
            nbt->setTag("Player", playerNbt);
        }
    }
