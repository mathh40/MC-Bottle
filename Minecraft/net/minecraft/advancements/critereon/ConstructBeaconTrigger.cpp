#include "ConstructBeaconTrigger.h"

ConstructBeaconTrigger::Listeners::Listeners(PlayerAdvancements playerAdvancementsIn)
    : playerAdvancements(playerAdvancementsIn)
{
}

bool ConstructBeaconTrigger::Listeners::isEmpty() const
{
    return listeners.empty();
}

void ConstructBeaconTrigger::Listeners::add(ICriterionTrigger::Listener *listener)
{
    listeners.emplace(listener);
}

void ConstructBeaconTrigger::Listeners::remove(ICriterionTrigger::Listener *listener)
{
    listeners.erase(listener);
}

ConstructBeaconTrigger::Instance::Instance(MinMaxBounds level)
    : AbstractCriterionInstance(ConstructBeaconTrigger::ID), level(level)
{
}

bool ConstructBeaconTrigger::Instance::test(TileEntityBeacon *beacon)
{
    return level.test((float)beacon.getLevels());
}

void ConstructBeaconTrigger::Listeners::trigger(TileEntityBeacon *beacon)
{
    std::vector<ICriterionTrigger::Listener *> list;
    for (auto listener1 : listeners)
    {
        if (((ConstructBeaconTrigger::Instance)listener1.getCriterionInstance()).test(beacon))
        {
            if (list == null)
            {
                list = Lists.newArrayList();
            }

            list.add(listener1);
        }
    }

    if (list != null)
    {
        var3 = list.iterator();

        while (var3.hasNext())
        {
            listener1 = (ICriterionTrigger::Listener)var3.next();
            listener1.grantCriterion(playerAdvancements);
        }
    }
}

ResourceLocation &ConstructBeaconTrigger::getId() const
{
    return ID;
}

void ConstructBeaconTrigger::addListener(PlayerAdvancements playerAdvancementsIn, ICriterionTrigger::Listener *listener)
{
    ConstructBeaconTrigger.Listeners constructbeacontrigger$listeners =
        (ConstructBeaconTrigger.Listeners)this.listeners.get(playerAdvancementsIn);
    if (constructbeacontrigger$listeners == null)
    {
        constructbeacontrigger$listeners = new ConstructBeaconTrigger.Listeners(playerAdvancementsIn);
        listeners.put(playerAdvancementsIn, constructbeacontrigger$listeners);
    }

    constructbeacontrigger$listeners.add(listener);
}

void ConstructBeaconTrigger::removeListener(PlayerAdvancements var1, ICriterionTrigger::Listener *var2)
{
    ConstructBeaconTrigger.Listeners constructbeacontrigger$listeners =
        (ConstructBeaconTrigger.Listeners)this.listeners.get(playerAdvancementsIn);
    if (constructbeacontrigger$listeners != null)
    {
        constructbeacontrigger$listeners.remove(listener);
        if (constructbeacontrigger$listeners.isEmpty())
        {
            listeners.remove(playerAdvancementsIn);
        }
    }
}

void ConstructBeaconTrigger::removeAllListeners(PlayerAdvancements var1)
{
    listeners.remove(playerAdvancementsIn);
}

std::unique_ptr<ICriterionInstance> ConstructBeaconTrigger::deserializeInstance(nlohmann::json &var1)
{
    MinMaxBounds minmaxbounds = MinMaxBounds.deserialize(json.get("level"));
    return new ConstructBeaconTrigger::Instance(minmaxbounds);
}

void ConstructBeaconTrigger::trigger(EntityPlayerMP *player, TileEntityBeacon *beacon)
{
    ConstructBeaconTrigger.Listeners constructbeacontrigger$listeners =
        (ConstructBeaconTrigger.Listeners)this.listeners.get(player.getAdvancements());
    if (constructbeacontrigger$listeners != null)
    {
        constructbeacontrigger$listeners.trigger(beacon);
    }
}
