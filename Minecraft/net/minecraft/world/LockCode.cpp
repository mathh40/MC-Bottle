#include "LockCode.h"
#include "NBTTagCompound.h"

LockCode::LockCode(std::string code)
	:lock(code)
{
}

bool LockCode::isEmpty() const
{
	return lock.empty();
}

std::string LockCode::getLock() const
{
	return lock;
}

void LockCode::toNBT(NBTTagCompound* nbt) const
{
	nbt->setString("Lock", lock);
}

LockCode LockCode::fromNBT(NBTTagCompound* nbt)
{
	if (nbt->hasKey("Lock", 8)) 
	{
		auto s = nbt->getString("Lock");
		return LockCode(s);
	}
	else 
	{
		return EMPTY_CODE;
	}
}
