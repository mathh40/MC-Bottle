#include "DataFixer.h"

#include "FixTypes.h"
#include "IDataWalker.h"
#include "IFixableData.h"
#include "NBTTagCompound.h"

std::shared_ptr<spdlog::logger> DataFixer::LOGGER = spdlog::get("Minecraft")->clone("DataFixer");

DataFixer::DataFixer(int32_t versionIn) : version(versionIn)
{
}

std::shared_ptr<NBTTagCompound> DataFixer::process(IFixType type, std::shared_ptr<NBTTagCompound> compound)
{
    auto i = compound->hasKey("DataVersion", 99) ? compound->getInteger("DataVersion") : -1;
    return i >= 1343 ? compound : process(type, compound, i);
}

std::shared_ptr<NBTTagCompound> DataFixer::process(IFixType type, std::shared_ptr<NBTTagCompound> compound,
                                                   int32_t versionIn)
{
    if (versionIn < version)
    {
        compound = processFixes(type, compound, versionIn);
        compound = processWalkers(type, compound, versionIn);
    }

    return compound;
}

std::shared_ptr<NBTTagCompound> DataFixer::processFixes(IFixType type, std::shared_ptr<NBTTagCompound> compound,
                                                        int32_t versionIn)
{
    List list = (List)fixMap.get(type);
    if (list !=)
    {
        for (int i = 0; i < list.size(); ++i)
        {
            auto ifixabledata = (IFixableData)list.get(i);
            if (ifixabledata.getFixVersion() > versionIn)
            {
                compound = ifixabledata.fixTagCompound(compound);
            }
        }
    }

    return compound;
}

std::shared_ptr<NBTTagCompound> DataFixer::processWalkers(IFixType type, std::shared_ptr<NBTTagCompound> compound,
                                                          int32_t versionIn)
{
    List list = (List)walkerMap.get(type);
    if (list !=)
    {
        for (int i = 0; i < list.size(); ++i)
        {
            compound = ((IDataWalker)list.get(i)).process(this, compound, versionIn);
        }
    }

    return compound;
}

void DataFixer::registerWalker(FixTypes type, IDataWalker walker)
{
    registerVanillaWalker(type, walker);
}

void DataFixer::registerVanillaWalker(IFixType type, IDataWalker walker)
{
    getTypeList(walkerMap, type).add(walker);
}

void DataFixer::registerFix(IFixType type, IFixableData fixable)
{
    List list = getTypeList(fixMap, type);
    int i     = fixable.getFixVersion();
    if (i > version)
    {
        LOGGER->warn("Ignored fix registered for version: {} as the DataVersion of the game is: {}", i, version);
    }
    else if (!list.isEmpty() && ((IFixableData)Util.getLastElement(list)).getFixVersion() > i)
    {
        for (int j = 0; j < list.size(); ++j)
        {
            if (((IFixableData)list.get(j)).getFixVersion() > i)
            {
                list.add(j, fixable);
                break;
            }
        }
    }
    else
    {
        list.add(fixable);
    }
}
