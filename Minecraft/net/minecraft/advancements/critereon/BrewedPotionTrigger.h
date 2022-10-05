#pragma once
#include "../../potion/PotionType.h"
#include "../../util/ResourceLocation.h"
#include "../ICriterionTrigger.h"
#include "../PlayerAdvancements.h"
#include "AbstractCriterionInstance.h"

#include <unordered_map>

class EntityPlayerMP;

class BrewedPotionTrigger : public ICriterionTrigger
{
  public:
    class Listeners
    {
      public:
        Listeners(PlayerAdvancements playerAdvancementsIn);
        bool isEmpty() const;
        void addListener(ICriterionTrigger::Listener *listener);
        void removeListener(ICriterionTrigger::Listener *listener);
        void trigger(PotionType potion);

      private:
        PlayerAdvancements playerAdvancements;
        std::set<ICriterionTrigger::Listener *> listeners;
    };

    class Instance : private AbstractCriterionInstance
    {
      public:
        Instance(PotionType potion);
        bool test(PotionType potion);

      private:
        PotionType potion;
    };

    ResourceLocation &getId() const override;
    void addListener(PlayerAdvancements var1, ICriterionTrigger::Listener *var2) override;
    void removeListener(PlayerAdvancements var1, ICriterionTrigger::Listener *var2) override;
    void removeAllListeners(PlayerAdvancements var1) override;
    std::unique_ptr<ICriterionInstance> deserializeInstance(nlohmann::json &var1) override;
    void trigger(EntityPlayerMP *player, PotionType &potionIn);

  private:
    static ResourceLocation ID = ResourceLocation("brewed_potion");
    std::unordered_map<PlayerAdvancements, BrewedPotionTrigger::Listeners *> listeners;
};
