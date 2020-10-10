#pragma once
#include "../util/text/ITextComponent.h"
#include <string>

class IWorldNameable
{
protected:
	~IWorldNameable() = default;
public:
	virtual std::string getName() const = 0;
	virtual bool hasCustomName() const = 0;
	virtual std::unique_ptr<ITextComponent> getDisplayName() const = 0;
};
