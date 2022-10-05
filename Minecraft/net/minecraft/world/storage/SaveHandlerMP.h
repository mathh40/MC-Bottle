#pragma once
#include "ISaveHandler.h"

class SaveHandlerMP : public ISaveHandler
{
  public:
    std::optional<WorldInfo> loadWorldInfo() override
    {
        return std::nullopt;
    }

    void checkSessionLock() override
    {
    }

    IChunkLoader *getChunkLoader(WorldProvider *provider) override
    {
        return nullptr;
    }

    void saveWorldInfoWithPlayer(WorldInfo worldInformation, NBTTagCompound *tagCompound) override
    {
    }

    void saveWorldInfo(WorldInfo worldInformation) override
    {
    }

    IPlayerFileData *getPlayerNBTManager() override
    {
        return nullptr;
    }

    void flush() override
    {
    }

    std::filesystem::path getMapFileFromName(std::string_view mapName) override
    {
        return "";
    }

    std::filesystem::path getWorldDirectory() override
    {
        return "";
    }

    TemplateManager getStructureTemplateManager() override
    {
        return nullptr;
    }
};
