#pragma once
#include "MapStorage.h"

class SaveDataMemoryStorage : public MapStorage
{
  public:
    SaveDataMemoryStorage();
    template <class T> std::unique_ptr<WorldSavedData> getOrLoadData(std::string_view dataIdentifier);
    void setData(std::string_view dataIdentifier, WorldSavedData *data) override;
    void saveAllData() override;
    int32_t getUniqueDataId(std::string_view key) override;

  private:
};

template <class T> std::unique_ptr<WorldSavedData> SaveDataMemoryStorage::getOrLoadData(std::string_view dataIdentifier)
{
    return loadedDataMap[dataIdentifier];
}
