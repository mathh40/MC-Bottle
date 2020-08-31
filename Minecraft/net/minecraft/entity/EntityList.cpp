#include "EntityList.h"

#include "Entity.h"

#include <typeindex>
std::shared_ptr<spdlog::logger> EntityList::LOGGER = spdlog::get("Minecraft")->clone("EntityList");

ResourceLocation EntityList::getKey(Entity *entityIn) {
    return getKey(entityIn->getClass());
}

ResourceLocation EntityList::getKey(std::type_index entityIn) {
    REGISTRY.getNameForObject(entityIn);
}

std::string EntityList::getEntityString(Entity *entityIn) {
    auto i = REGISTRY.getIDForObject(entityIn->getClass());
    return i == -1 ? "" : OLD_NAMES[i];
}

std::string EntityList::getTranslationName(std::optional<ResourceLocation> entityType) {
    auto i = REGISTRY.getIDForObject(REGISTRY.getObject(entityType).value());
    return i == -1 ? "" : OLD_NAMES[i];
}

std::optional<std::type_index> EntityList::getClassFromID(int32_t entityID) {
    return REGISTRY.getObjectById(entityID);
}

std::optional<std::type_index> EntityList::getClassFromName(std::string_view p_192839_0_) {
    return REGISTRY.getObject(ResourceLocation(p_192839_0_));
}

Entity * EntityList::newEntity(std::optional<std::type_index> clazz, World *worldIn) {
    if (!clazz) {
         return nullptr;
      } else {
         try {
            return conname[clazz.value().name()](worldIn);
         } catch (std::exception& var3) 
         {
            return nullptr;
         }
      }
}

Entity * EntityList::createEntityByID(int32_t entityID, World *worldIn) {
    return newEntity(getClassFromID(entityID), worldIn);
}
