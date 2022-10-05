#include "BrewedPotionTrigger.h"

#include "../../entity/player/EntityPlayerMP.h"
#include "../../potion/PotionType.h"
#include "../ICriterionInstance.h"

BrewedPotionTrigger::Listeners::Listeners(PlayerAdvancements playerAdvancementsIn)
    : playerAdvancements(playerAdvancementsIn)
{
}

bool BrewedPotionTrigger::Listeners::isEmpty() const
{
    return listeners.empty();
}

void BrewedPotionTrigger::Listeners::addListener(ICriterionTrigger::Listener *listener)
{
    listeners.emplace(listener);
}

void BrewedPotionTrigger::Listeners::removeListener(ICriterionTrigger::Listener *listener)
{
    listeners.erase(listener);
}

void BrewedPotionTrigger::Listeners::trigger(PotionType potion)
{
    std::vector<ICriterionTrigger::Listener *> list;

    for (auto listener1 : listeners)
    {
        if (static_cast<BrewedPotionTrigger::Instance *>(listener1->getCriterionInstance())->test(potion))
        {

            list.emplace_back(listener1);
        }
    }

    if (!list.empty())
    {
        for (auto listener1 : list)
        {
            listener1->grantCriterion(playerAdvancements);
        }
    }
}

BrewedPotionTrigger::Instance::Instance(PotionType potion)
    : AbstractCriterionInstance(BrewedPotionTrigger::ID), potion(potion)
{
}

bool BrewedPotionTrigger::Instance::test(PotionType potion)
{
    return potion == nullptr || potion == potion;
}

ResourceLocation &BrewedPotionTrigger::getId() const
{
    return ID;
}

void BrewedPotionTrigger::addListener(PlayerAdvancements playerAdvancementsIn, ICriterionTrigger::Listener *listener)
{
    auto brewedpotiontrigger$listeners = listeners.at(playerAdvancementsIn);
    if (brewedpotiontrigger$listeners == nullptr)
    {
        brewedpotiontrigger$listeners = new BrewedPotionTrigger::Listeners(playerAdvancementsIn);
        listeners.emplace(playerAdvancementsIn, brewedpotiontrigger$listeners);
    }

    brewedpotiontrigger$listeners->addListener(listener);
}

void BrewedPotionTrigger::removeListener(PlayerAdvancements playerAdvancementsIn, ICriterionTrigger::Listener *listener)
{
    auto brewedpotiontrigger$listeners = listeners.at(playerAdvancementsIn);
    if (brewedpotiontrigger$listeners != nullptr)
    {
        brewedpotiontrigger$listeners->removeListener(listener);
        if (brewedpotiontrigger$listeners->isEmpty())
        {
            listeners.erase(playerAdvancementsIn);
        }
    }
}

void BrewedPotionTrigger::removeAllListeners(PlayerAdvancements playerAdvancementsIn)
{
    listeners.erase(playerAdvancementsIn);
}

std::unique_ptr<ICriterionInstance> BrewedPotionTrigger::deserializeInstance(nlohmann::json &var1)
{
    PotionType potiontype;
    if (var1.at("potion"))
    {
        ResourceLocation resourcelocation = ResourceLocation(JsonUtils::getString(nlohmann::json, "potion"));
        if (!PotionType::REGISTRY::containsKey(resourcelocation))
        {
            throw std::logic_error("Unknown potion '" + resourcelocation + "'");
        }

        potiontype = (PotionType)PotionType::REGISTRY.getObject(resourcelocation);
    }

    return std::make_unique<BrewedPotionTrigger::Instance>(potiontype);
}

void BrewedPotionTrigger::trigger(EntityPlayerMP *player, PotionType &potionIn)
{
    auto brewedpotiontrigger$listeners = listeners.at(player->getAdvancements());
    if (brewedpotiontrigger$listeners != nullptr)
    {
        brewedpotiontrigger$listeners->trigger(potionIn);
    }
}
