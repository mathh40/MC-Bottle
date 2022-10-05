#include "WorldType.h"

#include <optional>
#include <string>

WorldType WorldType::DEFAULT                = (WorldType(0, "default", 1)).setVersioned();
WorldType WorldType::FLAT                   = WorldType(1, "flat");
WorldType WorldType::LARGE_BIOMES           = WorldType(2, "largeBiomes");
WorldType WorldType::AMPLIFIED              = (WorldType(3, "amplified")).enableInfoNotice();
WorldType WorldType::CUSTOMIZED             = WorldType(4, "customized");
WorldType WorldType::DEBUG_ALL_BLOCK_STATES = WorldType(5, "debug_all_block_states");
WorldType WorldType::DEFAULT_1_1            = (WorldType(8, "default_1_1", 0)).setCanBeCreated(false);

std::string WorldType::getName() const
{
    return name;
}

std::string WorldType::getTranslationKey() const
{
    return "generator." + name;
}

std::string WorldType::getInfoTranslationKey() const
{
    return getTranslationKey() + ".info";
}

int32_t WorldType::getVersion() const
{
    return version;
}

WorldType &WorldType::getWorldTypeForGeneratorVersion(int32_t version)
{
    return *this == DEFAULT && version == 0 ? DEFAULT_1_1 : *this;
}

bool WorldType::canBeCreated() const
{
    return BeCreated;
}

bool WorldType::isVersioned() const
{
    return versioned;
}

std::optional<WorldType> WorldType::byName(std::string_view type)
{
    for (auto worldtype : WORLD_TYPES)
    {
        if (worldtype != nullptr && worldtype->name == type)
        {
            return *worldtype;
        }
    }
    return std::nullopt;
}

int32_t WorldType::getId() const
{
    return id;
}

bool WorldType::hasInfoNotice() const
{
    return InfoNotice;
}

WorldType::WorldType(int32_t id, std::string name) : WorldType(id, name, 0)
{
}

WorldType::WorldType(int32_t id, std::string name, int32_t version)
{
    name            = name;
    version         = version;
    BeCreated       = true;
    id              = id;
    WORLD_TYPES[id] = this;
}

WorldType &WorldType::setCanBeCreated(bool enable)
{
    BeCreated = enable;
    return *this;
}

WorldType &WorldType::setVersioned()
{
    versioned = true;
    return *this;
}

WorldType &WorldType::enableInfoNotice()
{
    InfoNotice = true;
    return *this;
}
