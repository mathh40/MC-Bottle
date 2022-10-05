#include "CooldownTracker.h"

#include "math/MathHelper.h"

bool CooldownTracker::hasCooldown(Item *itemIn)
{
    return getCooldown(itemIn, 0.0F) > 0.0F;
}

float CooldownTracker::getCooldown(Item *itemIn, float partialTicks)
{
    auto cooldowntracker$cooldown = cooldowns.find(*itemIn)->second;
    auto f  = (float)(cooldowntracker$cooldown.expireTicks - cooldowntracker$cooldown.createTicks);
    auto f1 = (float)cooldowntracker$cooldown.expireTicks - ((float)ticks + partialTicks);
    return MathHelper::clamp(f1 / f, 0.0F, 1.0F);
}

void CooldownTracker::tick()
{
    ++ticks;
    if (!cooldowns.empty())
    {

        for (auto entry = cooldowns.begin(); entry != cooldowns.end(); ++entry)
        {
            if (entry->second.expireTicks <= ticks)
            {
                entry = cooldowns.erase(entry);
                notifyOnRemove(entry->first);
            }
        }
    }
}

void CooldownTracker::setCooldown(Item *itemIn, uint32_t ticksIn)
{
    cooldowns.emplace(itemIn, ticks, ticks + ticksIn);
    notifyOnSet(itemIn, ticksIn);
}

void CooldownTracker::removeCooldown(Item *itemIn)
{
    cooldowns.erase(*itemIn);
    notifyOnRemove(itemIn);
}

void CooldownTracker::notifyOnSet(Item *itemIn, uint32_t ticksIn)
{
}

void CooldownTracker::notifyOnRemove(Item *itemIn)
{
}
