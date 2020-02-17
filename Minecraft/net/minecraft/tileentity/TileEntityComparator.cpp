#include "TileEntityComparator.h"

NBTTagCompound* TileEntityComparator::writeToNBT(NBTTagCompound* compound)
{
	TileEntity::writeToNBT(compound);
	compound->setInteger("OutputSignal", outputSignal);
	return compound;
}

void TileEntityComparator::readFromNBT(NBTTagCompound* compound)
{
	TileEntity::readFromNBT(compound);
	outputSignal = compound->getInteger("OutputSignal");
}

int32_t TileEntityComparator::getOutputSignal() const
{
	return outputSignal;
}

void TileEntityComparator::setOutputSignal(int32_t outputSignalIn)
{
	outputSignal = outputSignalIn;
}
