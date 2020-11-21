#pragma once
#include "../inventory/Container.h"
#include "IWorldNameable.h"
class IInteractionObject :public IWorldNameable
{
public:
	virtual Container* createContainer(InventoryPlayer* var1, EntityPlayer* var2) const = 0;
	virtual std::string getGuiID() const = 0;
protected:
    virtual ~IInteractionObject() = default;
};