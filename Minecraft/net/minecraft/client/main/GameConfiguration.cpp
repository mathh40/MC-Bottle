#include "GameConfiguration.h"

GameConfiguration::GameConfiguration(UserInformation userInfoIn, DisplayInformation displayInfoIn,
                                     FolderInformation folderInfoIn, GameInformation gameInfoIn,
                                     ServerInformation serverInfoIn)
{
}

GameConfiguration::UserInformation::UserInformation(Session sessionIn, PropertyMap userPropertiesIn,
                                                    PropertyMap profilePropertiesIn)
    : session(sessionIn), userProperties(userPropertiesIn), profileProperties(profilePropertiesIn)
{
}

GameConfiguration::ServerInformation::ServerInformation(std::string serverNameIn, std::uint32_t serverPortIn)
    : serverName(serverNameIn), serverPort(serverPortIn)
{
}

GameConfiguration::GameInformation::GameInformation(bool demo, std::string versionIn, std::string versionTypeIn)
    : isDemo(demo), version(versionIn), versionType(versionTypeIn)
{
}

GameConfiguration::FolderInformation::FolderInformation(std::filesystem::path mcDataDirIn,
                                                        std::filesystem::path resourcePacksDirIn,
                                                        std::filesystem::path assetsDirIn,
                                                        std::optional<std::string> assetIndexIn)
    : gameDir(mcDataDirIn), resourcePacksDir(resourcePacksDirIn), assetsDir(assetsDirIn), assetIndex(assetIndexIn)
{
}

ResourceIndex GameConfiguration::FolderInformation::getAssetsIndex()
{
    return (ResourceIndex)(assetIndex.has_value() ? ResourceIndexFolder(assetsDir)
                                                  : ResourceIndex(assetsDir, assetIndex.value()));
}

GameConfiguration::DisplayInformation::DisplayInformation(uint32_t widthIn, uint32_t heightIn, bool fullscreenIn,
                                                          bool checkGlErrorsIn)
    : width(widthIn), height(heightIn), fullscreen(fullscreenIn), checkGlErrors(checkGlErrorsIn)
{
}
