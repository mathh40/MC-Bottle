#pragma once
#include <unordered_map>

class CooldownTracker
{
public:
	bool hasCooldown(Item itemIn);
	float getCooldown(Item itemIn, float partialTicks);
	void tick();
	void setCooldown(Item itemIn, int ticksIn);
	void removeCooldown(Item itemIn);
protected:
	virtual void notifyOnSet(Item itemIn, int ticksIn);
	virtual void notifyOnRemove(Item itemIn);

private:

	struct Cooldown 
	{
		int createTicks;
		int expireTicks;

		Cooldown(int createTicksIn, int expireTicksIn) {
			createTicks = createTicksIn;
			expireTicks = expireTicksIn;
		}
	};

	std::unordered_map<Item, Cooldown> cooldowns;
	int ticks = 0;
};