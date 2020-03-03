#pragma once
#include "CooldownTracker.h"

class EntityPlayerMP;

class CooldownTrackerServer : public CooldownTracker
{
public:
	CooldownTrackerServer(EntityPlayerMP playerIn);
protected:
	void notifyOnSet(Item itemIn, int ticksIn) override;
	void notifyOnRemove(Item itemIn) override;
private:
	EntityPlayerMP player;
};
