#pragma once
#include "CooldownTracker.h"

class EntityPlayerMP;

class CooldownTrackerServer : public CooldownTracker
{
  public:
    explicit CooldownTrackerServer(EntityPlayerMP *playerIn);

  protected:
    void notifyOnSet(Item *itemIn, uint32_t ticksIn) override;
    void notifyOnRemove(Item *itemIn) override;

  private:
    EntityPlayerMP *player;
};
