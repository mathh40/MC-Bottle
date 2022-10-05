#pragma once
#include "IPlayerFileData.h"
#include "World.h"

class ISaveHandler
{
  public:
    virtual std::optional<WorldInfo> loadWorldInfo()                           = 0;
    virtual void checkSessionLock()                                            = 0;
    virtual IChunkLoader *getChunkLoader(WorldProvider *var1)                  = 0;
    virtual void saveWorldInfoWithPlayer(WorldInfo var1, NBTTagCompound *var2) = 0;
    virtual void saveWorldInfo(WorldInfo var1)                                 = 0;
    virtual IPlayerFileData *getPlayerNBTManager()                             = 0;
    virtual void flush()                                                       = 0;
    virtual std::filesystem::path getWorldDirectory()                          = 0;
    virtual std::filesystem::path getMapFileFromName(std::string_view var1)    = 0;
    virtual TemplateManager getStructureTemplateManager()                      = 0;
};
