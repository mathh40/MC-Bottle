#pragma once
#include "Advancement.h"
#include "../../../../nlohmann_json/tests/abi/include/nlohmann/json_v3_10_5.hpp"

class Advancement;
class ResourceLocation;
class PlayerAdvancements;
class ICriterionInstance;

class ICriterionTrigger
{
public:
  virtual ~ICriterionTrigger() = default;

  class Listener {
   public:
    Listener(ICriterionInstance* criterionInstanceIn, Advancement advancementIn, std::string_view criterionNameIn);
    ICriterionInstance* getCriterionInstance() const;
    void grantCriterion(PlayerAdvancements playerAdvancementsIn);
    friend bool operator==(const Listener& a,const Listener& b);
    uint64_t hashCode();


   private:
      ICriterionInstance* criterionInstance;
      Advancement advancement;
      std::string criterionName;
   };

  virtual ResourceLocation& getId() const;

   virtual void addListener(PlayerAdvancements var1, ICriterionTrigger::Listener* var2);

   virtual void removeListener(PlayerAdvancements var1, ICriterionTrigger::Listener* var2);

   virtual void removeAllListeners(PlayerAdvancements var1);

   virtual std::unique_ptr<ICriterionInstance> deserializeInstance(nlohmann::json& var1);
private:
};
