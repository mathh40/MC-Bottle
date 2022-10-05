#include "MapStorage.h"

#include "CompressedStreamTools.h"
#include "NBTTagCompound.h"
#include "NBTTagShort.h"

MapStorage::MapStorage(ISaveHandler *saveHandlerIn) : saveHandler(saveHandlerIn)
{
    loadIdCounts();
}

void MapStorage::setData(std::string_view dataIdentifier, WorldSavedData *data)
{
    auto ite = loadedDataMap.find(std::string(dataIdentifier));
    if (ite != loadedDataMap.end())
    {
        loadedDataMap.erase(ite);
        loadedDataList.erase(std::remove_if(loadedDataList.begin(), loadedDataList.end(), ite->second()),
                             loadedDataList.end());
    }

    loadedDataMap.emplace(dataIdentifier, data);
    loadedDataList.emplace_back(data);
}

void MapStorage::saveAllData()
{
    for (auto worldsaveddata : loadedDataList)
    {
        if (worldsaveddata->isDirty())
        {
            saveData(worldsaveddata);
            worldsaveddata->setDirty(false);
        }
    }
}

int32_t MapStorage::getUniqueDataId(std::string_view key)
{
    auto oshort = idCounts[std::string(key)];
    oshort      = oshort + 1;

    idCounts.emplace(key, oshort);
    if (saveHandler == nullptr)
    {
        return oshort;
    }
    else
    {
        try
        {
            auto file1 = saveHandler->getMapFileFromName("idcounts");
            if (!file1.empty())
            {
                auto nbttagcompound = std::make_unique<NBTTagCompound>();

                for (auto s : idCounts)
                {
                    nbttagcompound->setShort(s, idCounts[std::string(key)]);
                }

                CompressedStreamTools::write(nbttagcompound, file1);
            }
        }
        catch (std::exception &var7)
        {
            var7.printStackTrace();
        }

        return oshort;
    }
}

void MapStorage::saveData(WorldSavedData *data)
{
    if (saveHandler != nullptr)
    {
        try
        {
            auto file1 = saveHandler->getMapFileFromName(data->mapName);
            if (!file1.empty())
            {
                auto nbttagcompound = std::make_unique<NBTTagCompound>();
                nbttagcompound->setTag("data", data->writeToNBT(new NBTTagCompound()));
                std::ofstream fileoutputstream(file1);
                CompressedStreamTools::writeCompressed(nbttagcompound, fileoutputstream);
                fileoutputstream.close();
            }
        }
        catch (std::exception &var5)
        {
            var5.printStackTrace();
        }
    }
}

void MapStorage::loadIdCounts()
{
    try
    {
        idCounts.clear();
        if (saveHandler == nullptr)
        {
            return;
        }

        auto file1 = saveHandler->getMapFileFromName("idcounts");
        if (exists(file1))
        {
            auto nbttagcompound = CompressedStreamTools::read(file1);

            for (auto s : nbttagcompound->getCompoundMap())
            {
                auto nbtbase = nbttagcompound->getTag(s.first);
                if (Util:: instanceof <NBTTagShort>(nbtbase))
                {
                    auto nbttagshort = (NBTTagShort *)nbtbase;
                    int16_t short1   = nbttagshort->getShort();
                    idCounts.emplace(s, short1);
                }
            }
        }
    }
    catch (std::exception &var9)
    {
        var9.printStackTrace();
    }
}
