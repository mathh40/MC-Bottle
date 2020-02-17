#include "TileEntity.h"

class TileEntityNote :public TileEntity
{
public:

	NBTTagCompound* writeToNBT(NBTTagCompound* compound) override;
	void readFromNBT(NBTTagCompound* compound) override;
	void changePitch();
	void triggerNote(World* worldIn, BlockPos posIn);

	
	uint8_t note;
	bool previousRedstoneState;
private:
	
};
