#include "TileEntityDispenser.h"

TileEntityDispenser::TileEntityDispenser()
	:stacks(9,ItemStack::EMPTY)
{
}

int32_t TileEntityDispenser::getSizeInventory() const
{
	return 9;
}

bool TileEntityDispenser::isEmpty()
{
	bool empty = false;

	for(auto stack : stacks)
	{
		empty = stack.isEmpty();
	}
	return empty;
}

int32_t TileEntityDispenser::getDispenseSlot() const
{
	fillWithLoot(nullptr);
	int32_t i = -1;
	int32_t j = 1;

	for(int32_t k = 0; k < stacks.size(); ++k) 
	{
		if (!stacks[k].isEmpty() && RNG(j++) == 0) 
		{
			i = k;
		}
	}

	return i;
}

int32_t TileEntityDispenser::addItemStack(ItemStack stack)
{
	for(int32_t i = 0; i < stacks.size(); ++i) 
	{
		if (stacks[i].isEmpty()) 
		{
			setInventorySlotContents(i, stack);
			return i;
		}
	}

	return -1;
}

std::string TileEntityDispenser::getName() const
{
	return hasCustomName() ? customName : "container.dispenser";
}

void TileEntityDispenser::registerFixes(DataFixer fixer)
{
	fixer.registerWalker(FixTypes::BLOCK_ENTITY, new ItemStackDataLists(TileEntityDispenser.class, new String[]{"Items"}));
}

void TileEntityDispenser::readFromNBT(NBTTagCompound* compound)
{
	TileEntityLockableLoot::readFromNBT(compound);
	stacks = std::vector<ItemStack>(getSizeInventory(), ItemStack::EMPTY);
	if (!checkLootAndRead(compound)) {
		ItemStackHelper.loadAllItems(compound, stacks);
	}

	if (compound->hasKey("CustomName", 8)) {
		customName = compound->getString("CustomName");
	}
}

NBTTagCompound* TileEntityDispenser::writeToNBT(NBTTagCompound* compound)
{
	TileEntityLockableLoot::writeToNBT(compound);
	if (!checkLootAndWrite(compound)) 
	{
		ItemStackHelper.saveAllItems(compound, stacks);
	}

	if (hasCustomName()) 
	{
		compound->setString("CustomName", customName);
	}

	return compound;
}

int32_t TileEntityDispenser::getInventoryStackLimit() const
{
	return 64;
}

ontainer TileEntityDispenser::createContainer(InventoryPlayer* playerInventory, EntityPlayer* playerIn)
{
	fillWithLoot(playerIn);
	return ContainerDispenser(playerInventory, this);
}

std::vector<ItemStack> TileEntityDispenser::getItems() const
{
	return stacks;
}

std::string TileEntityDispenser::getGuiID() const
{
	return "minecraft:dispenser";
}
