#include "BredAnimalsTrigger.h"

#include "../ICriterionInstance.h"
#include "../PlayerAdvancements.h"


  BredAnimalsTrigger::Instance::Instance(EntityPredicate *parent, EntityPredicate *partner, EntityPredicate *child)
  : AbstractCriterionInstance(BredAnimalsTrigger::ID), parent(parent), partner(partner), child(child)
{
  return {};
}

bool BredAnimalsTrigger::Instance::test(EntityPlayerMP *player,
  EntityAnimal *parent1In,
  EntityAnimal *parent2In,
  EntityAgeable *childIn) const{ 
  if (!child->test(player, childIn)) {
            return false;
         } else {
            return parent->test(player, parent1In) && partner->test(player, parent2In) || parent->test(player, parent2In) && partner->test(player, parent1In);
         } 
}

BredAnimalsTrigger::Listeners::Listeners(const PlayerAdvancements &playerAdvancementsIn) : playerAdvancements(&playerAdvancementsIn) {}
bool BredAnimalsTrigger::Listeners::isEmpty() const { return listeners.empty(); }
void BredAnimalsTrigger::Listeners::add(ICriterionTrigger::Listener *listener) {
  listeners.insert(listener);
}

void BredAnimalsTrigger::Listeners::remove(ICriterionTrigger::Listener *listener) {listeners.erase(listener);}

void BredAnimalsTrigger::Listeners::trigger(EntityPlayerMP *player,
  EntityAnimal *parent1,
  EntityAnimal *parent2,
  EntityAgeable *child) const
{
  std::vector<ICriterionTrigger::Listener*> list;
         for (auto listener1 :listeners){
            if (reinterpret_cast<BredAnimalsTrigger::Instance*>(listener1->getCriterionInstance())->test(player, parent1, parent2, child)) {
               list.emplace_back(listener1);
            }
         }

         if (!list.empty()) {

            for (auto listener1 : list){
               listener1->grantCriterion(*playerAdvancements);
            }
         }
}


ResourceLocation& BredAnimalsTrigger::getId() const {
    return ID;
}

void BredAnimalsTrigger::addListener(PlayerAdvancements playerAdvancementsIn, ICriterionTrigger::Listener* listener) {
    auto bredanimalstrigger$listeners = listeners.at(playerAdvancementsIn);
      if (bredanimalstrigger$listeners == nullptr) {
         bredanimalstrigger$listeners = std::make_unique<BredAnimalsTrigger::Listeners>(playerAdvancementsIn);
         listeners.insert(playerAdvancementsIn, bredanimalstrigger$listeners);
      }

      bredanimalstrigger$listeners->add(listener);
}

void BredAnimalsTrigger::removeListener(PlayerAdvancements playerAdvancementsIn, ICriterionTrigger::Listener* listener) {
    auto bredanimalstrigger$listeners = listeners.at(playerAdvancementsIn);
      if (bredanimalstrigger$listeners != nullptr) {
         bredanimalstrigger$listeners->remove(listener);
         if (bredanimalstrigger$listeners->isEmpty()) {
            listeners.erase(playerAdvancementsIn);
         }
      }
}

void BredAnimalsTrigger::removeAllListeners(PlayerAdvancements playerAdvancementsIn)
{
    listeners.erase(playerAdvancementsIn);
}

std::unique_ptr<ICriterionInstance>  BredAnimalsTrigger::deserializeInstance(nlohmann::json &json)
{
   EntityPredicate entitypredicate = EntityPredicate::deserialize(json.get<EntityPredicate>("parent"));
      EntityPredicate entitypredicate1 = EntityPredicate::deserialize(json.get<EntityPredicate>("partner"));
      EntityPredicate entitypredicate2 = EntityPredicate::deserialize(json.get<EntityPredicate>("child"));
      return std::make_unique<BredAnimalsTrigger::Instance>(entitypredicate, entitypredicate1, entitypredicate2);
}
