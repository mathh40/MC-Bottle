#pragma once
#include "../inventory/IInventory.h"
#include "IInteractionObject.h"

class LockCode;

class ILockableContainer : public IInventory, public IInteractionObject
{
  public:
    virtual ~ILockableContainer()            = default;
    virtual bool isLocked()                  = 0;
    virtual void setLockCode(LockCode &var1) = 0;
    virtual LockCode getLockCode()           = 0;
};
