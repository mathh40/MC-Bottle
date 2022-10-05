#pragma once
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

class WorldProvider;
class DimensionType
{
  public:
    static DimensionType OVERWORLD;
    static DimensionType NETHER;
    static DimensionType THE_END;

    uint32_t getId() const;
    std::string getName() const;
    std::string getSuffix() const;
    WorldProvider *createDimension() const;
    DimensionType &getById(uint32_t id) const;
    DimensionType &byName(std::string nameIn) const;
    DimensionType(const DimensionType &&lhs) = default;

  private:
    uint32_t id;
    std::string name;
    std::string suffix;
    std::unique_ptr<WorldProvider> clazz;
    static std::vector<DimensionType *> value;

    DimensionType(uint32_t idIn, std::string nameIn, std::string suffixIn, std::unique_ptr<WorldProvider> &clazzIn);
};
