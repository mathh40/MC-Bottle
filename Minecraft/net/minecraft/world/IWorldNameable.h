#pragma once
#include <string>

class ITextComponent;
class IWorldNameable
{
public:
	virtual std::string getName() const = 0;
	virtual bool hasCustomName() const = 0;
	virtual ITextComponent* getDisplayName() const = 0;

    virtual ~IWorldNameable() = default;
};
