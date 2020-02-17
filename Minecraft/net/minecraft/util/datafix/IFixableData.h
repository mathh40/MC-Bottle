#pragma once
#include "NBTTagCompound.h"

 class IFixableData {
	virtual int32_t getFixVersion() = 0;

	virtual std::shared_ptr<NBTTagCompound> fixTagCompound(std::shared_ptr<NBTTagCompound> var1) = 0;
 };
