#pragma once
#include "IBlockProperties.h"
#include "IBlockBehaviors.h"

class IProperty;

class IBlockState :public IBlockBehaviors, IBlockProperties
{
protected:
	~IBlockState() = default;
public:
	virtual std::vector<IProperty*> getPropertyKeys() = 0;
	virtual Comparable getValue(IProperty* var1) = 0;
	virtual IBlockState* withProperty(IProperty* var1, Comparable var2) = 0;
	virtual IBlockState* cycleProperty(IProperty* var1) = 0;
	virtual std::unordered_map<> getProperties() = 0;
	virtual Block* getBlock() = 0;
};
