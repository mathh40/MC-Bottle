#pragma once
#include "../command/FunctionObject.h"
#include "../util/ResourceLocation.h"

class EntityPlayerMP;

class AdvancementRewards
  {
public:
  static AdvancementRewards EMPTY;
  AdvancementRewards(uint64_t experience, std::vector<ResourceLocation> loot, std::vector<ResourceLocation> recipes, FunctionObject::CacheableFunction function);
  void apply(EntityPlayerMP* player);
  std::string toString();

private:
     uint64_t experience;
   std::vector<ResourceLocation> loot;
   std::vector<ResourceLocation> recipes;
   FunctionObject::CacheableFunction function;
};


using json = nlohmann::json;

namespace ns {
    void to_json(json& j, const class AdvancementRewards
& p) {
    }

    void from_json(const json& j, class AdvancementRewards
& p) {
    }
}
