#include "../../util/ResourceLocation.h"
#include "../ICriterionTrigger.h"
#include "../PlayerAdvancements.h"
#include "AbstractCriterionInstance.h"

class CuredZombieVillagerTrigger : public ICriterionTrigger
{
  public:
    class Instance : public AbstractCriterionInstance
    {
      public:
        Instance(EntityPredicate zombie, EntityPredicate villager);
        bool test(EntityPlayerMP *player, EntityZombie *zombie, EntityVillager *villager);

      private:
        EntityPredicate zombie;
        EntityPredicate villager;
    };

    class Listeners
    {
      public:
        Listeners(PlayerAdvancements playerAdvancementsIn);
        bool isEmpty() const;
        void add(ICriterionTrigger::Listener *listener);
        void remove(ICriterionTrigger::Listener *listener);
        void trigger(EntityPlayerMP *player, EntityZombie *zombie, EntityVillager *villager);

      private:
        PlayerAdvancements playerAdvancements;
        std::set<ICriterionTrigger::Listener *> listeners;
    };

    ResourceLocation &getId() const override;
    void addListener(PlayerAdvancements var1, ICriterionTrigger::Listener *var2) override;
    void removeListener(PlayerAdvancements var1, ICriterionTrigger::Listener *var2) override;
    void removeAllListeners(PlayerAdvancements var1) override;
    std::unique_ptr<ICriterionInstance> deserializeInstance(nlohmann::json &var1) override;
    void trigger(EntityPlayerMP *player, EntityZombie *zombie, EntityVillager *villager);

  private:
    static ResourceLocation ID = ResourceLocation("cured_zombie_villager");
    final std::unordered_map<PlayerAdvancements, CuredZombieVillagerTrigger::Listeners *> listeners;
};
