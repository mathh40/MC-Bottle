#pragma once
#include "NBTTagCompound.h"

class IDataFixer;

class IDataWalker
{
public:
	virtual ~IDataWalker() = default;
	virtual NBTTagCompound* process(IDataFixer* type, NBTTagCompound* compound, int32_t versionIn) = 0;
};
