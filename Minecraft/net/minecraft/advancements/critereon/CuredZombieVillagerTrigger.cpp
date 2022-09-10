#include "CuredZombieVillagerTrigger.h"

CuredZombieVillagerTrigger::Instance::Instance(EntityPredicate zombie, EntityPredicate villager)
  : AbstractCriterionInstance(CuredZombieVillagerTrigger::ID), zombie(zombie), villager(villager)
{

}

bool CuredZombieVillagerTrigger::Instance::test(EntityPlayerMP* player, EntityZombie* zombie, EntityVillager* villager)
{
    return !zombie.test(player, zombie) ? false : villager.test(player, villager);
}

CuredZombieVillagerTrigger::Listeners::Listeners(PlayerAdvancements playerAdvancementsIn) : playerAdvancements(playerAdvancementsIn) {

 }

bool CuredZombieVillagerTrigger::Listeners::isEmpty() const {
    return listeners.empty();
}

void CuredZombieVillagerTrigger::Listeners::add(ICriterionTrigger::Listener* listener) {
    listeners.emplace(listener);
}

void CuredZombieVillagerTrigger::Listeners::remove(ICriterionTrigger::Listener* listener) {
    listeners.erase(listener);
}

void trigger(EntityPlayerMP* player, EntityZombie* zombie, EntityVillager* villager) {
    std::vector<> list;
         for (auto listener1 : listeners){
            listener1 = (ICriterionTrigger.Listener)var5.next();
            if (((CuredZombieVillagerTrigger.Instance)listener1.getCriterionInstance()).test(player, zombie, villager)) {

               list.add(listener1);
            }
         }

         if (!list.empty()) {
            for (auto listener1 : list){
               listener1.grantCriterion(playerAdvancements);
            }
         }
}

ResourceLocation& CuredZombieVillagerTrigger::getId() const override {
    return ID;
}

void CuredZombieVillagerTrigger::addListener(PlayerAdvancements var1, ICriterionTrigger::Listener* var2) override{
    CuredZombieVillagerTrigger.Listeners curedzombievillagertrigger$listeners = (CuredZombieVillagerTrigger.Listeners)this.listeners.get(playerAdvancementsIn);
      if (curedzombievillagertrigger$listeners == null) {
         curedzombievillagertrigger$listeners = new CuredZombieVillagerTrigger.Listeners(playerAdvancementsIn);
         listeners.put(playerAdvancementsIn, curedzombievillagertrigger$listeners);
      }

      curedzombievillagertrigger$listeners.add(listener);
}

void CuredZombieVillagerTrigger::removeListener(PlayerAdvancements var1, ICriterionTrigger::Listener* var2) override {
    CuredZombieVillagerTrigger.Listeners curedzombievillagertrigger$listeners = (CuredZombieVillagerTrigger.Listeners)this.listeners.get(playerAdvancementsIn);
      if (curedzombievillagertrigger$listeners != null) {
         curedzombievillagertrigger$listeners.remove(listener);
         if (curedzombievillagertrigger$listeners.isEmpty()) {
            this.listeners.remove(playerAdvancementsIn);
         }
      }
}

void CuredZombieVillagerTrigger::removeAllListeners(PlayerAdvancements var1) override {
    listeners.remove(playerAdvancementsIn);
}

std::unique_ptr<ICriterionInstance> CuredZombieVillagerTrigger::deserializeInstance(nlohmann::json& var1) override {
    EntityPredicate entitypredicate = EntityPredicate.deserialize(json.get("zombie"));
      EntityPredicate entitypredicate1 = EntityPredicate.deserialize(json.get("villager"));
      return new CuredZombieVillagerTrigger.Instance(entitypredicate, entitypredicate1);
}

void CuredZombieVillagerTrigger::trigger(EntityPlayerMP* player, EntityZombie* zombie, EntityVillager* villager) {
    CuredZombieVillagerTrigger.Listeners curedzombievillagertrigger$listeners = (CuredZombieVillagerTrigger.Listeners)this.listeners.get(player.getAdvancements());
      if (curedzombievillagertrigger$listeners != null) {
         curedzombievillagertrigger$listeners.trigger(player, zombie, villager);
      }
}
