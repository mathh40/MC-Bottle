#include "TileEntityLockableLoot.h"
#include "datafix/DataFixer.h"

class TileEntityDispenser :public TileEntityLockableLoot
{
public:
	TileEntityDispenser();
	int32_t getSizeInventory() const;
	bool isEmpty();
	int32_t getDispenseSlot() const;
	int32_t addItemStack(ItemStack stack);
	std::string getName() const override;
	static void registerFixes(DataFixer fixer);
	void readFromNBT(NBTTagCompound* compound) override;
	NBTTagCompound* writeToNBT(NBTTagCompound* compound) override;
	int32_t getInventoryStackLimit() const;
	std::string getGuiID() const;
	Container createContainer(InventoryPlayer* playerInventory, EntityPlayer* playerIn);
protected:
	std::vector<ItemStack> getItems() const;
private:
	static pcg32 RNG;
	std::vector<ItemStack> stacks;
};
