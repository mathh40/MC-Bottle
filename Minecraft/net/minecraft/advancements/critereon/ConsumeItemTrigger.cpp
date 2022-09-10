#include "ConsumeItemTrigger.h"

ConsumeItemTrigger::Instance::Instance(ItemPredicate item) : AbstractCriterionInstance(ConsumeItemTrigger::ID), item(item)
{

}

bool ConsumeItemTrigger::Instance::test(const ItemStack& item) {
    return item.test(item);
}

ConsumeItemTrigger::Listeners::Listeners(PlayerAdvancements playerAdvancementsIn) : playerAdvancements(playerAdvancementsIn) {}

bool ConsumeItemTrigger::Listeners::isEmpty() const {
    return listeners.empty();
}

void ConsumeItemTrigger::Listeners::add(ICriterionTrigger::Listener* listener) {
    listeners.emplace(listener);
}

void ConsumeItemTrigger::Listeners::remove(ICriterionTrigger::Listener* listener) {
    listeners.erase(listener);
}

void ConsumeItemTrigger::Listeners::trigger(const ItemStack& item) {
    std::vector<ICriterionTrigger::Listener*> list;
         for (auto listener1 : listeners){
            if (((ConsumeItemTrigger::Instance*)listener1.getCriterionInstance()).test(item)) {

               list.add(listener1);
            }
         }

         if (!list.empty()) {
           for (auto listener1: list){
               listener1.grantCriterion(playerAdvancements);
            }
         }

      }

ResourceLocation& ConsumeItemTrigger::getId() const {
    return ID;
}

void ConsumeItemTrigger::addListener(PlayerAdvancements var1, ICriterionTrigger::Listener* var2) override {
    ConsumeItemTrigger.Listeners consumeitemtrigger$listeners = (ConsumeItemTrigger.Listeners)this.listeners.get(playerAdvancementsIn);
      if (consumeitemtrigger$listeners == null) {
         consumeitemtrigger$listeners = new ConsumeItemTrigger.Listeners(playerAdvancementsIn);
         this.listeners.put(playerAdvancementsIn, consumeitemtrigger$listeners);
      }

      consumeitemtrigger$listeners.add(listener);
}

void ConsumeItemTrigger::removeListener(PlayerAdvancements var1, ICriterionTrigger::Listener* var2) override {
    ConsumeItemTrigger.Listeners consumeitemtrigger$listeners = (ConsumeItemTrigger.Listeners)this.listeners.get(playerAdvancementsIn);
      if (consumeitemtrigger$listeners != null) {
         consumeitemtrigger$listeners.remove(listener);
         if (consumeitemtrigger$listeners.isEmpty()) {
            this.listeners.remove(playerAdvancementsIn);
         }
      }
}

void ConsumeItemTrigger::removeAllListeners(PlayerAdvancements var1) override {
    listeners.remove(playerAdvancementsIn);
}

std::unique_ptr<ICriterionInstance> ConsumeItemTrigger::deserializeInstance(nlohmann::json& var1) override {
    ItemPredicate itempredicate = ItemPredicate.deserialize(json.get("item"));
      return new ConsumeItemTrigger.Instance(itempredicate);
}

void ConsumeItemTrigger::trigger(EntityPlayerMP* player, const ItemStack& item) {
    ConsumeItemTrigger.Listeners consumeitemtrigger$listeners = (ConsumeItemTrigger.Listeners)this.listeners.get(player.getAdvancements());
      if (consumeitemtrigger$listeners != null) {
         consumeitemtrigger$listeners.trigger(item);
      }
}
