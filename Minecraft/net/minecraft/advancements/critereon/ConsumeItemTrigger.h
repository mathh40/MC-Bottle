#include "../../util/ResourceLocation.h"
#include "../ICriterionTrigger.h"
#include "../PlayerAdvancements.h"
#include "AbstractCriterionInstance.h"

class ConsumeItemTrigger : public ICriterionTrigger
{
  public:
    class Instance : public AbstractCriterionInstance
    {
      public:
        Instance(ItemPredicate item);
        bool test(const ItemStack &item);

      private:
        ItemPredicate item;
    };
    class Listeners
    {
      public:
        Listeners(PlayerAdvancements playerAdvancementsIn);
        bool isEmpty() const;
        void add(ICriterionTrigger::Listener *listener);
        void remove(ICriterionTrigger::Listener *listener);
        void trigger(const ItemStack &item);

      private:
        PlayerAdvancements playerAdvancements;
        std::set<ICriterionTrigger::Listener *> listeners;
    };

    ResourceLocation &getId() const override;
    void addListener(PlayerAdvancements var1, ICriterionTrigger::Listener *var2) override;
    void removeListener(PlayerAdvancements var1, ICriterionTrigger::Listener *var2) override;
    void removeAllListeners(PlayerAdvancements var1) override;
    std::unique_ptr<ICriterionInstance> deserializeInstance(nlohmann::json &var1) override;
    void trigger(EntityPlayerMP *player, const ItemStack &item);

  private:
    static ResourceLocation ID = ResourceLocation("consume_item");
    std::unordered_map<PlayerAdvancements, ConsumeItemTrigger::Listeners *> listeners;
};
