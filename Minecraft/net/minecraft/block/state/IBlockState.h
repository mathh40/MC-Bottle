#pragma once
#include <unordered_map>
#include <functional>
#include "IBlockProperties.h"
#include "IBlockBehaviors.h"

class IProperty;
class Block;

class IBlockState :public IBlockBehaviors, public IBlockProperties
{
protected:
	~IBlockState() = default;
public:
	virtual std::vector<IProperty*> getPropertyKeys() = 0;
	virtual std::function<bool()> getValue(IProperty* var1) = 0;
	virtual IBlockState* withProperty(IProperty* var1, std::function<bool()> var2) = 0;
	virtual IBlockState* cycleProperty(IProperty* var1) = 0;
	virtual std::unordered_map<int,IProperty*> getProperties() = 0;
	virtual Block* getBlock() = 0;
};
