#pragma once
#include "IPlayerFileData.h"
#include "ISaveHandler.h"
#include "datafix/DataFixer.h"

class SaveHandler : public ISaveHandler, public IPlayerFileData
{
  public:
    SaveHandler(std::filesystem::path p_i46648_1_, std::string saveDirectoryNameIn, bool p_i46648_3_,
                DataFixer dataFixerIn);
    std::filesystem::path getWorldDirectory() override;
    void checkSessionLock() override;
    IChunkLoader *getChunkLoader(WorldProvider *provider) override;
    std::optional<WorldInfo> loadWorldInfo() override;
    void saveWorldInfoWithPlayer(WorldInfo worldInformation, NBTTagCompound *tagCompound) override;
    void saveWorldInfo(WorldInfo worldInformation) override;
    void writePlayerData(EntityPlayer *player) override;
    std::unique_ptr<NBTTagCompound> readPlayerData(EntityPlayer *player) override;
    IPlayerFileData *getPlayerNBTManager() override;
    std::vector<std::string> getAvailablePlayerDat() override;
    void flush() override;
    std::filesystem::path getMapFileFromName(std::string_view mapName) override;
    TemplateManager getStructureTemplateManager() override;

  protected:
    DataFixer dataFixer;

  private:
    static std::shared_ptr<spdlog::logger> LOGGER;
    std::filesystem::path worldDirectory;
    std::filesystem::path playersDirectory;
    std::filesystem::path mapDataDir;
    int64_t initializationTime = MinecraftServer.getCurrentTimeMillis();
    std::string saveDirectoryName;
    std::optional<TemplateManager> structureTemplateManager;
    void setSessionLock() const;
};
