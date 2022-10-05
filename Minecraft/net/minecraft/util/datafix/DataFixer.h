#pragma once
#include "IDataFixer.h"
#include "spdlog/spdlog.h"

class IFixableData;
class IDataWalker;
enum class FixTypes;
class NBTTagCompound;

class DataFixer : public IDataFixer
{
  public:
    DataFixer(int32_t versionIn);
    std::shared_ptr<NBTTagCompound> process(IFixType type, std::shared_ptr<NBTTagCompound> compound);
    std::shared_ptr<NBTTagCompound> process(IFixType type, std::shared_ptr<NBTTagCompound> compound,
                                            int32_t versionIn) override;

    void registerWalker(FixTypes type, IDataWalker walker);
    void registerVanillaWalker(IFixType type, IDataWalker walker);
    void registerFix(IFixType type, IFixableData fixable);

  private:
    static std::shared_ptr<spdlog::logger> LOGGER;
    std::unordered_map<IFixType, IDataWalker> walkerMap;
    std::unordered_map<IFixType, IFixableData> fixMap;
    int32_t version;

    std::shared_ptr<NBTTagCompound> processFixes(IFixType type, std::shared_ptr<NBTTagCompound> compound,
                                                 int32_t versionIn);
    std::shared_ptr<NBTTagCompound> processWalkers(IFixType type, std::shared_ptr<NBTTagCompound> compound,
                                                   int32_t versionIn);

    template <class T> List getTypeList(T map, IFixType type);
};

template <class T> List DataFixer::getTypeList(T map, IFixType type)
{
    List list = (List)map.get(type);
    if (list == null)
    {
        list = Lists.newArrayList();
        map.put(type, list);
    }

    return (List)list;
}
