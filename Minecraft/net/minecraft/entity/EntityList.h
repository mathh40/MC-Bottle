#pragma once
#include "ResourceLocation.h"
#include "registry/RegistryNamespaced.h"

#include <unordered_set>

class World;

namespace std {
    class type_index;
}

class Entity;

class EntityList {
public:
    static ResourceLocation LIGHTNING_BOLT = ResourceLocation("lightning_bolt");
    static RegistryNamespaced<ResourceLocation,std::type_index> REGISTRY;
    static std::unordered_map<> ENTITY_EGGS;
    static std::unordered_set<> KNOWN_TYPES;

    static ResourceLocation getKey(Entity* entityIn);
    static ResourceLocation getKey(std::type_index entityIn);
    static std::string getEntityString(Entity* entityIn);
    static std::string getTranslationName(std::optional<ResourceLocation> entityType);
    static std::optional<std::type_index> getClassFromID(int32_t entityID);
    static std::optional<std::type_index> getClassFromName(std::string_view p_192839_0_);
    static Entity* newEntity(std::optional<std::type_index> clazz, World* worldIn);
    static Entity* createEntityByID(int32_t entityID, World* worldIn);

private:
       
   static ResourceLocation PLAYER = ResourceLocation("player");
   static std::shared_ptr<spdlog::logger> LOGGER; 
   static std::vector<std::string> OLD_NAMES;
    static std::unordered_map<std::string,std::function<Entity*(World*)>> conname;
};
