#include "DerivedWorldInfo.h"

DerivedWorldInfo::DerivedWorldInfo(WorldInfo worldInfoIn) : delegate(worldInfoIn)
{
}

std::unique_ptr<NBTTagCompound> DerivedWorldInfo::cloneNBTCompound(NBTTagCompound *nbt)
{
    return delegate.cloneNBTCompound(nbt);
}

std::string DerivedWorldInfo::getWorldName() const
{
    return delegate.getWorldName();
}

int32_t DerivedWorldInfo::getSaveVersion() const
{
    return delegate.getSaveVersion();
}

int64_t DerivedWorldInfo::getLastTimePlayed() const
{
    return delegate.getLastTimePlayed();
}

bool DerivedWorldInfo::isThundering() const
{
    return delegate.isThundering();
}

int32_t DerivedWorldInfo::getThunderTime() const
{
    return delegate.getThunderTime();
}

bool DerivedWorldInfo::isRaining() const
{
    return delegate.isRaining();
}

int32_t DerivedWorldInfo::getRainTime() const
{
    return delegate.getRainTime();
}

GameType DerivedWorldInfo::getGameType() const
{
    return delegate.getGameType();
}

void DerivedWorldInfo::setSpawnX(int x)
{
}

void DerivedWorldInfo::setSpawnY(int y)
{
}

void DerivedWorldInfo::setSpawnZ(int z)
{
}

void DerivedWorldInfo::setWorldTotalTime(int64_t time)
{
}

void DerivedWorldInfo::setWorldTime(int64_t time)
{
}

void DerivedWorldInfo::setSpawn(const BlockPos &spawnPoint)
{
}

void DerivedWorldInfo::setWorldName(std::string_view worldName)
{
}

void DerivedWorldInfo::setSaveVersion(int32_t version)
{
}

void DerivedWorldInfo::setThundering(bool thunderingIn)
{
}

void DerivedWorldInfo::setThunderTime(int32_t time)
{
}

void DerivedWorldInfo::setRaining(bool isRaining)
{
}

void DerivedWorldInfo::setRainTime(int32_t time)
{
}

bool DerivedWorldInfo::isMapFeaturesEnabled() const
{
    return delegate.isMapFeaturesEnabled();
}

bool DerivedWorldInfo::isHardcoreModeEnabled() const
{
    return delegate.isHardcoreModeEnabled();
}

WorldType DerivedWorldInfo::getTerrainType() const
{
    return delegate.getTerrainType();
}

void DerivedWorldInfo::setTerrainType(WorldType type)
{
}

bool DerivedWorldInfo::areCommandsAllowed() const
{
    return delegate.areCommandsAllowed();
}

void DerivedWorldInfo::setAllowCommands(bool allow)
{
}

bool DerivedWorldInfo::isInitialized() const
{
    return delegate.isInitialized();
}

void DerivedWorldInfo::setServerInitialized(bool initializedIn)
{
}

GameRules DerivedWorldInfo::getGameRulesInstance() const
{
    return delegate.getGameRulesInstance();
}

EnumDifficulty DerivedWorldInfo::getDifficulty() const
{
    return delegate.getDifficulty();
}

void DerivedWorldInfo::setDifficulty(EnumDifficulty newDifficulty)
{
}

bool DerivedWorldInfo::isDifficultyLocked() const
{
    return delegate.isDifficultyLocked();
}

void DerivedWorldInfo::setDifficultyLocked(bool locked)
{
}

void DerivedWorldInfo::setDimensionData(DimensionType dimensionIn, NBTTagCompound *compound)
{
    delegate.setDimensionData(dimensionIn, compound);
}

std::unique_ptr<NBTTagCompound> DerivedWorldInfo::getDimensionData(DimensionType dimensionIn)
{
    return delegate.getDimensionData(dimensionIn);
}

NBTTagCompound *DerivedWorldInfo::getPlayerNBTTagCompound() const
{
    return delegate.getPlayerNBTTagCompound();
}

int64_t DerivedWorldInfo::getSizeOnDisk() const
{
    return delegate.getSizeOnDisk();
}

int64_t DerivedWorldInfo::getWorldTime() const
{
    return delegate.getWorldTime();
}

int64_t DerivedWorldInfo::getWorldTotalTime() const
{
    return delegate.getWorldTotalTime();
}

int32_t DerivedWorldInfo::getSpawnZ() const
{
    return delegate.getSpawnZ();
}

int32_t DerivedWorldInfo::getSpawnY() const
{
    return delegate.getSpawnY();
}

int32_t DerivedWorldInfo::getSpawnX() const
{
    return delegate.getSpawnX();
}

int64_t DerivedWorldInfo::getSeed() const
{
    return delegate.getSeed();
}
