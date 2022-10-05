#pragma once
#include "../stats/StatBase.h"
#include "../stats/StatList.h"
#include "ResourceLocation.h"
#include "registry/RegistryNamespaced.h"

#include <unordered_set>

class NBTTagCompound;
class World;

namespace std
{
class type_index;
}

class Entity;

class EntityList
{
  public:
    static class EntityEggInfo
    {
      public:
        ResourceLocation spawnedID;
        int32_t primaryColor;
        int32_t secondaryColor;
        StatBase killEntityStat;
        StatBase entityKilledByStat;

        EntityEggInfo(ResourceLocation idIn, int primaryColorIn, int secondaryColorIn)
        {
            spawnedID          = idIn;
            primaryColor       = primaryColorIn;
            secondaryColor     = secondaryColorIn;
            killEntityStat     = StatList::getStatKillEntity(this);
            entityKilledByStat = StatList::getStatEntityKilledBy(this);
        }
    };

    static inline ResourceLocation LIGHTNING_BOLT = ResourceLocation("lightning_bolt");
    static RegistryNamespaced<ResourceLocation, std::type_index> REGISTRY;
    static std::unordered_map<ResourceLocation, EntityEggInfo> ENTITY_EGGS;
    static std::unordered_set<ResourceLocation> KNOWN_TYPES;

    static ResourceLocation getKey(Entity *entityIn);
    static ResourceLocation getKey(std::type_index entityIn);
    static std::string getEntityString(Entity *entityIn);
    static std::string getTranslationName(std::optional<ResourceLocation> entityType);
    static std::optional<std::type_index> getClassFromID(int32_t entityID);
    static std::optional<std::type_index> getClassFromName(std::string_view p_192839_0_);
    static Entity *newEntity(std::optional<std::type_index> clazz, World *worldIn);
    static Entity *createEntityByID(int32_t entityID, World *worldIn);
    static Entity *createEntityByIDFromName(const ResourceLocation &name, World *worldIn);
    static Entity *createEntityFromNBT(NBTTagCompound *nbt, World *worldIn);
    static std::unordered_set<ResourceLocation> getEntityNameList();
    static bool isMatchingName(Entity *entityIn, const ResourceLocation &entityName);
    static bool isRegistered(const ResourceLocation &entityName);
    static std::string getValidTypeNames();
    static void init();

  protected:
    static EntityEggInfo addSpawnInfo(std::string_view id, int32_t primaryColor, int32_t secondaryColor);

  private:
    static inline ResourceLocation PLAYER = ResourceLocation("player");
    static std::shared_ptr<spdlog::logger> LOGGER;
    static std::vector<std::string> OLD_NAMES;
    static std::unordered_map<std::string, std::function<Entity *(World *)>> conname;

    template <class T> static void registers(int32_t id, std::string_view name, std::string_view oldName);
};

template <class T> void EntityList::registers(int32_t id, std::string_view name, std::string_view oldName)
{
    auto clazz = [](World *world) { return new T(world); };
    conname.insert(typeid(T).name(), clazz);
    ResourceLocation resourcelocation(name);
    REGISTRY.registe(id, resourcelocation, clazz);
    KNOWN_TYPES.emplace(resourcelocation);

    while (OLD_NAMES.size() <= id)
    {
        OLD_NAMES.emplace_back("");
    }

    OLD_NAMES[id] = oldName;
}
