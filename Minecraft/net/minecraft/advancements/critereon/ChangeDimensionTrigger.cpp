#include "ChangeDimensionTrigger.h"

ChangeDimensionTrigger::Listeners::Listeners(PlayerAdvancements playerAdvancementsIn)
    : playerAdvancements(playerAdvancementsIn)
{
}
bool ChangeDimensionTrigger::Listeners::isEmpty()
{
    return listeners.empty();
}

void ChangeDimensionTrigger::Listeners::add(ICriterionTrigger::Listener *listener)
{
    listeners.emplace(listener);
}

void ChangeDimensionTrigger::Listeners::remove(ICriterionTrigger::Listener *listener)
{
    listeners.erase(listener);
}

void ChangeDimensionTrigger::Listeners::trigger(DimensionType from, DimensionType to)
{
    std::vector<ChangeDimensionTrigger::Instance *> list;
    for (auto listener1 : listeners)
    {
        if (((ChangeDimensionTrigger::Instance)listener1.getCriterionInstance()).test(from, to))
        {

            list.emplace_back(listener1);
        }
    }

    if (!list.empty())
    {
        for (auto listener1 : list)
        {
            listener1.grantCriterion(playerAdvancements);
        }
    }
}

ChangeDimensionTrigger::Instance::Instance(DimensionType from, DimensionType to)
    : AbstractCriterionInstance(ChangeDimensionTrigger::ID), from(from), to(to)
{
}

bool ChangeDimensionTrigger::Instance::test(DimensionType from, DimensionType to)
{
    if (from != nullptr && from != from)
    {
        return false;
    }
    else
    {
        return to == nullptr || to == to;
    }
}

ResourceLocation &ChangeDimensionTrigger::getId() const
{
    return ID;
}

void ChangeDimensionTrigger::addListener(PlayerAdvancements var1, ICriterionTrigger::Listener *var2)
{
    ChangeDimensionTrigger.Listeners changedimensiontrigger$listeners =
        (ChangeDimensionTrigger.Listeners)this.listeners.get(playerAdvancementsIn);
    if (changedimensiontrigger$listeners == null)
    {
        changedimensiontrigger$listeners = new ChangeDimensionTrigger.Listeners(playerAdvancementsIn);
        listeners.put(playerAdvancementsIn, changedimensiontrigger$listeners);
    }

    changedimensiontrigger$listeners.add(listener);
}

void ChangeDimensionTrigger::removeListener(PlayerAdvancements var1, ICriterionTrigger::Listener *var2)
{
    ChangeDimensionTrigger.Listeners changedimensiontrigger$listeners =
        (ChangeDimensionTrigger.Listeners)this.listeners.get(playerAdvancementsIn);
    if (changedimensiontrigger$listeners != null)
    {
        changedimensiontrigger$listeners.remove(listener);
        if (changedimensiontrigger$listeners.isEmpty())
        {
            listeners.remove(playerAdvancementsIn);
        }
    }
}

void ChangeDimensionTrigger::removeAllListeners(PlayerAdvancements var1)
{
    listeners.remove(playerAdvancementsIn);
}

std::unique_ptr<ICriterionInstance> ChangeDimensionTrigger::deserializeInstance(nlohmann::json &var1)
{
    DimensionType dimensiontype  = json.has("from") ? DimensionType.byName(JsonUtils.getString(json, "from")) : null;
    DimensionType dimensiontype1 = json.has("to") ? DimensionType.byName(JsonUtils.getString(json, "to")) : null;
    return new ChangeDimensionTrigger.Instance(dimensiontype, dimensiontype1);
}

void ChangeDimensionTrigger::trigger(EntityPlayerMP *player, DimensionType from, DimensionType to)
{
    ChangeDimensionTrigger.Listeners changedimensiontrigger$listeners =
        (ChangeDimensionTrigger.Listeners)this.listeners.get(player.getAdvancements());
    if (changedimensiontrigger$listeners != null)
    {
        changedimensiontrigger$listeners.trigger(from, to);
    }
}
