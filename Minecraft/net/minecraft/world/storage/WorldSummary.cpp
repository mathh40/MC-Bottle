#include "WorldSummary.h"

#include "WorldInfo.h"
#include "text/translation/I18n.h"

WorldSummary::WorldSummary(WorldInfo info, std::string fileNameIn, std::string displayNameIn, int64_t sizeOnDiskIn,
                           bool requiresConversionIn)
    : fileName(fileNameIn), displayName(displayNameIn), lastTimePlayed(info.getLastTimePlayed()),
      sizeOnDisk(sizeOnDiskIn), gameType(info.getGameType()), brequiresConversion(requiresConversionIn),
      hardcore(info.isHardcoreModeEnabled()), cheatsEnabled(info.areCommandsAllowed()),
      versionName(info.getVersionName()), versionId(info.getVersionId()), versionSnapshot(info.isVersionSnapshot())
{
}

std::string WorldSummary::getFileName() const
{
    return fileName;
}

std::string WorldSummary::getDisplayName() const
{
    return displayName;
}

int64_t WorldSummary::getSizeOnDisk() const
{
    return sizeOnDisk;
}

bool WorldSummary::requiresConversion() const
{
    return brequiresConversion;
}

int64_t WorldSummary::getLastTimePlayed() const
{
    return lastTimePlayed;
}

GameType WorldSummary::getEnumGameType() const
{
    return gameType;
}

bool WorldSummary::isHardcoreModeEnabled() const
{
    return hardcore;
}

bool WorldSummary::getCheatsEnabled() const
{
    return cheatsEnabled;
}

std::string WorldSummary::getVersionName() const
{
    return versionName.empty() ? I18n::translateToLocal("selectWorld.versionUnknown") : versionName;
}

bool WorldSummary::markVersionInList() const
{
    return askToOpenWorld();
}

bool WorldSummary::askToOpenWorld() const
{
    return versionId > 1343;
}

bool operator<(const WorldSummary &lhs, const WorldSummary &rhs)
{
    return lhs.getLastTimePlayed() < rhs.getLastTimePlayed();
}
