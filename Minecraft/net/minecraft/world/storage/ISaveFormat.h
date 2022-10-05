#pragma once
#include "ISaveHandler.h"
#include "World.h"

#include <string>
#include <vector>

class IProgressUpdate;

class ISaveFormat
{
  public:
    virtual std::string getName() = 0;

    virtual ISaveHandler *getSaveLoader(std::string_view var1, bool var2) = 0;

    virtual std::vector<WorldSummary> getSaveList() = 0;

    virtual void flushCache() = 0;

    virtual std::optional<WorldInfo> getWorldInfo(std::string_view var1) = 0;

    virtual bool isNewLevelIdAcceptable(std::string_view var1) = 0;

    virtual bool deleteWorldDirectory(std::string_view var1) = 0;

    virtual void renameWorld(std::string_view var1, std::string_view var2) = 0;

    virtual bool isConvertible(std::string_view var1) = 0;

    virtual bool isOldMapFormat(std::string_view var1) = 0;

    virtual bool convertMapFormat(std::string_view var1, IProgressUpdate *var2) = 0;

    virtual bool canLoadWorld(std::string_view var1) = 0;

    virtual std::filesystem::path getFile(std::string_view var1, std::string_view var2) = 0;
};
