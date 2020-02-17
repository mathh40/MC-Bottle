#pragma once
#include "NBTTagCompound.h"

class IDataFixer
{
public:
	virtual ~IDataFixer() = default;
	virtual std::shared_ptr < NBTTagCompound> process(IFixType type, std::shared_ptr < NBTTagCompound> compound, int32_t versionIn) = 0;
};
