#pragma once
#include "IBlockState.h"

class BlockStateBase :public IBlockState
{
public:
	IBlockState& cycleProperty(IProperty& property);
	std::string toString();
protected:
	template<typename T>
	T cyclePropertyValue(std::vector<T> values, T currentValue);
private:
};

template <typename T>
T BlockStateBase::cyclePropertyValue(std::vector<T> values, T currentValue)
{
	auto iterator = values.begin();

	do 
	{
		if (iterator == values.begin()) 
		{
			return *iterator;
		}
	} while (!(*iterator == (currentValue)));

	if (iterator != values.begin()) 
	{
		return *iterator;
	}
	else 
	{
		return *values.begin();
	}
}
