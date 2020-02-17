#include "TileEntityLockable.h"
#include "text/TextComponentTranslation.h"
#include "text/TextComponentString.h"

TileEntityLockable::TileEntityLockable()
	:code(LockCode::EMPTY_CODE)
{
}

void TileEntityLockable::readFromNBT(NBTTagCompound* compound)
{
	TileEntity::readFromNBT(compound);
	code = LockCode::fromNBT(compound);
}

NBTTagCompound* TileEntityLockable::writeToNBT(NBTTagCompound* compound)
{
	TileEntity::writeToNBT(compound);
	code.toNBT(compound);

	return compound;
}

bool TileEntityLockable::isLocked()
{
	return !code.isEmpty();
}

LockCode TileEntityLockable::getLockCode()
{
	return code;
}

void TileEntityLockable::setLockCode(LockCode& code)
{
	code = code;
}

ITextComponent* TileEntityLockable::getDisplayName()
{
	return (ITextComponent)(hasCustomName() ? new TextComponentString(getName()) : new TextComponentTranslation(getName(), new Object[0]));
}
