#pragma once
#include "IWorldNameable.h"
class IInteractionObject :public IWorldNameable
{
public:
	virtual Container createContainer(InventoryPlayer var1, EntityPlayer var2) const = 0;
	virtual std::string getGuiID() const = 0;
protected:
	~IInteractionObject() = default;
};