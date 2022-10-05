#pragma once
#include <unordered_map>

class Item;

class CooldownTracker
{
  public:
    bool hasCooldown(Item *itemIn);
    float getCooldown(Item *itemIn, float partialTicks);
    void tick();
    void setCooldown(Item *itemIn, uint32_t ticksIn);
    void removeCooldown(Item *itemIn);

  protected:
    virtual void notifyOnSet(Item *itemIn, uint32_t ticksIn);
    virtual void notifyOnRemove(Item *itemIn);

  private:
    struct Cooldown
    {
        uint32_t createTicks;
        uint32_t expireTicks;

        Cooldown(uint32_t createTicksIn, uint32_t expireTicksIn)
        {
            createTicks = createTicksIn;
            expireTicks = expireTicksIn;
        }
    };

    std::unordered_map<Item, Cooldown> cooldowns;
    uint32_t ticks = 0;
};
