#pragma once
#include "BlockPattern.h"

#include <string_view>
#include <unordered_map>
#include <vector>

class FactoryBlockPattern
{
  public:
    FactoryBlockPattern aisle(std::vector<std::string> &aisleIn);
    static FactoryBlockPattern start();
    template <typename T> FactoryBlockPattern where(char symbol, T blockMatcher);
    BlockPattern build();

  private:
    FactoryBlockPattern();
    multivec3<> &makePredicateArray();
    void checkMissingPredicates() const;

    static constexpr std::string_view COMMA_JOIN = ",";
    std::vector<std::vector<std::string>> depth;
    std::unordered_map<char, bool> symbolMap;
    uint64_t aisleHeight;
    uint64_t rowWidth;
};

template <typename T> FactoryBlockPattern FactoryBlockPattern::where(char symbol, T blockMatcher)
{
    symbolMap.emplace(symbol, blockMatcher);
    return *this;
}
