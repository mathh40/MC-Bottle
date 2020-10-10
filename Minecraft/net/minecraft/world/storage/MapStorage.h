#pragma once
#include "ISaveHandler.h"
#include "WorldSavedData.h"
#include <fstream>

class MapStorage
{
public:
	template <class T>
	std::unique_ptr<T> getOrLoadData(std::string_view dataIdentifier);
	MapStorage(ISaveHandler* saveHandlerIn);
	virtual void setData(std::string_view dataIdentifier, WorldSavedData* data);
	virtual void saveAllData();
	virtual int32_t getUniqueDataId(std::string_view key);
protected:
	std::unordered_map<std::string, WorldSavedData*> loadedDataMap;

	
private:
	ISaveHandler* saveHandler;
	std::vector<WorldSavedData*> loadedDataList;
	std::unordered_map<std::string, int16_t> idCounts;

	void saveData(WorldSavedData* data);
	void loadIdCounts();
};

template <class T>
std::unique_ptr<T> MapStorage::getOrLoadData(std::string_view dataIdentifier)
{
	T* worldsaveddata = loadedDataMap[dataIdentifier];
	if (worldsaveddata != nullptr) 
	{
		return worldsaveddata;
	}
	else 
	{
		if (saveHandler != nullptr) 
		{
			try 
			{
				auto file1 = saveHandler->getMapFileFromName(dataIdentifier);
				if (exists(file1))
				{
					try 
					{
						worldsaveddata = std::make_unique<T>(dataIdentifier);
					}
					catch (std::exception& var7) 
					{
						throw std::runtime_error(std::string("Failed to instantiate ") + var7.what());
					}

					std::ifstream fileinputstream(file1);
					auto nbttagcompound = CompressedStreamTools::readCompressed(fileinputstream);
					fileinputstream.close();
					worldsaveddata->readFromNBT(nbttagcompound->getCompoundTag("data"));
				}
			}
			catch (std::exception& var8)
			{
				var8.printStackTrace();
			}
		}

		if (worldsaveddata != nullptr) 
		{
			loadedDataMap.emplace(dataIdentifier, worldsaveddata);
			loadedDataList.emplace_back(worldsaveddata);
		}

		return worldsaveddata;
	}
}
