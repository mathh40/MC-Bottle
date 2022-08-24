#pragma once
#include "AbstractCriterionInstance.h"
#include "../ICriterionTrigger.h"
#include "../../../../../nlohmann_json/tests/abi/include/nlohmann/json_v3_10_5.hpp"

#include <unordered_map>


class EntityAgeable;
class EntityAnimal;
class EntityPlayerMP;
class PlayerAdvancements;
class ResourceLocation;
class EntityPredicate;

class BredAnimalsTrigger :public ICriterionTrigger
{
public:
      class Instance :public AbstractCriterionInstance
      {
    public:
      Instance(EntityPredicate* parent, EntityPredicate* partner, EntityPredicate* child);
      bool test(EntityPlayerMP* player, EntityAnimal* parent1In, EntityAnimal* parent2In, EntityAgeable* childIn) const;
    private:
      EntityPredicate* parent;
      EntityPredicate* partner;
      EntityPredicate* child;
    };

  class Listeners {
  public:
        Listeners(const PlayerAdvancements& playerAdvancementsIn);
    bool isEmpty() const;
    void add(ICriterionTrigger::Listener* listener);
    void remove(ICriterionTrigger::Listener* listener);
    void trigger(EntityPlayerMP* player, EntityAnimal* parent1, EntityAnimal* parent2, EntityAgeable* child) const;
  private:
          const PlayerAdvancements* playerAdvancements;
      std::set< ICriterionTrigger::Listener*> listeners;
    };

    ResourceLocation& getId() const override;
    void addListener(PlayerAdvancements playerAdvancementsIn, ICriterionTrigger::Listener* listener) override;
    void removeListener(PlayerAdvancements playerAdvancementsIn, ICriterionTrigger::Listener* listener) override;
    void removeAllListeners(PlayerAdvancements playerAdvancementsIn) override;
    std::unique_ptr<ICriterionInstance> deserializeInstance(nlohmann::json& json) override;
    void trigger(EntityPlayerMP* player, EntityAnimal* parent1, EntityAnimal* parent2, EntityAgeable* child) const;

private:
   static ResourceLocation ID = ResourceLocation("bred_animals");
   std::unordered_map<PlayerAdvancements,std::unique_ptr<BredAnimalsTrigger::Listeners>> listeners;
};
