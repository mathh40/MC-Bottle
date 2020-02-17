#pragma once
#include "NBTTagCompound.h"

class IDataFixer;

class IDataWalker
{
public:
	virtual ~IDataWalker() = default;
	virtual std::unique_ptr<NBTTagCompound> process(IDataFixer* type, std::unique_ptr < NBTTagCompound> compound, int32_t versionIn) = 0;
};
