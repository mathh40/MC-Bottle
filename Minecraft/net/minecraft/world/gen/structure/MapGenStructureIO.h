#pragma once
#include "StructureStart.h"
#include "spdlog/async_logger.h"
#include "unordered_map"

class MapGenStructureIO
{
  public:
    void registerStructure(Class startClass, std::string_view structureName);
    std::string getStructureStartName(const StructureStart &start);
    std::string getStructureComponentName(const StructureComponent &component);
    std::optional<StructureStart> getStructureStart(NBTTagCompound *tagCompound, World *worldIn);
    StructureComponent *getStructureComponent(NBTTagCompound *tagCompound, World *worldIn);

  private:
    static std::shared_ptr<spdlog::logger> LOGGER;
    std::unordered_map<> startNameToClassMap;
    std::unordered_map<> startClassToNameMap;
    std::unordered_map<> componentNameToClassMap;
    std::unordered_map<> componentClassToNameMap;
};
