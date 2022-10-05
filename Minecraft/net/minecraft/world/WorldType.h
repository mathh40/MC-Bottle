#pragma once
#include <array>
#include <optional>

class WorldType
{
  public:
    static std::array<WorldType *, 16> WORLD_TYPES;
    static WorldType DEFAULT;
    static WorldType FLAT;
    static WorldType LARGE_BIOMES;
    static WorldType AMPLIFIED;
    static WorldType CUSTOMIZED;
    static WorldType DEBUG_ALL_BLOCK_STATES;
    static WorldType DEFAULT_1_1;

    std::string getName() const;
    std::string getTranslationKey() const;
    std::string getInfoTranslationKey() const;
    int32_t getVersion() const;
    WorldType &getWorldTypeForGeneratorVersion(int32_t version);
    bool canBeCreated() const;
    bool isVersioned() const;
    static std::optional<WorldType> byName(std::string_view type);
    int32_t getId() const;
    bool hasInfoNotice() const;

  private:
    int32_t id;
    std::string name;
    int32_t version;
    bool BeCreated;
    bool versioned;
    bool InfoNotice;

    WorldType(int32_t id, std::string name);
    WorldType(int32_t id, std::string name, int32_t version);
    WorldType &setCanBeCreated(bool enable);
    WorldType &setVersioned();
    WorldType &enableInfoNotice();
};
