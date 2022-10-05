#pragma once
#include <cstdint>
#include <string>
#include <vector>

class EnumDifficulty
{
  public:
    static EnumDifficulty PEACEFUL;
    static EnumDifficulty EASY;
    static EnumDifficulty NORMAL;
    static EnumDifficulty HARD;

    uint16_t getId() const;
    static EnumDifficulty &byId(uint16_t id);
    std::string getTranslationKey() const;

    friend bool operator==(const EnumDifficulty &lhs, const EnumDifficulty &rhs);

  private:
    EnumDifficulty(uint16_t difficultyIdIn, std::string difficultyResourceKeyIn);

    static std::vector<EnumDifficulty *> ID_MAPPING;
    uint16_t id;
    std::string translationKey;
};
