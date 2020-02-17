#include "TileEntityChest.h"
#include "datafix/DataFixer.h"

TileEntityChest::TileEntityChest()
	:chestContents(27, ItemStack::EMPTY)
{
}

TileEntityChest::TileEntityChest(BlockChest::Type typeIn)
	:chestContents(27, ItemStack::EMPTY),cachedChestType(typeIn)
{
}

int32_t TileEntityChest::getSizeInventory() const
{
	return 27;
}

bool TileEntityChest::isEmpty()
{
	for(auto stack : chestContents)
	{
		if(!stack.isEmpty())
		{
			return false;
		}
	}
	return true;
}

std::string TileEntityChest::getName() const
{
	return hasCustomName() ? customName : "container.chest";
}

void TileEntityChest::registerFixesChest(DataFixer fixer)
{
	fixer.registerWalker(FixTypes::BLOCK_ENTITY, new ItemStackDataLists(TileEntityChest.class, new String[]{"Items"}));
}

void TileEntityChest::readFromNBT(NBTTagCompound* compound)
{
	TileEntityLockableLoot::readFromNBT(compound);
	chestContents = NonNullList.withSize(getSizeInventory(), ItemStack::EMPTY);
	if (!checkLootAndRead(compound)) 
	{
		ItemStackHelper.loadAllItems(compound, chestContents);
	}

	if (compound->hasKey("CustomName", 8)) 
	{
		customName = compound->getString("CustomName");
	}
}

NBTTagCompound* TileEntityChest::writeToNBT(NBTTagCompound* compound)
{
	TileEntityLockableLoot::writeToNBT(compound);
	if (!checkLootAndWrite(compound)) 
	{
		ItemStackHelper.saveAllItems(compound, chestContents);
	}

	if (hasCustomName()) 
	{
		compound->setString("CustomName", customName);
	}

	return compound;
}

int32_t TileEntityChest::getInventoryStackLimit() const
{
	return 64;
}

void TileEntityChest::updateContainingBlockInfo()
{
	TileEntityLockableLoot::updateContainingBlockInfo();
	adjacentChestChecked = false;
}

void TileEntityChest::checkForAdjacentChests()
{
	if (!adjacentChestChecked) 
	{
		adjacentChestChecked = true;
		adjacentChestXNeg = getAdjacentChest(EnumFacing::WEST);
		adjacentChestXPos = getAdjacentChest(EnumFacing::EAST);
		adjacentChestZNeg = getAdjacentChest(EnumFacing::NORTH);
		adjacentChestZPos = getAdjacentChest(EnumFacing::SOUTH);
	}
}

void TileEntityChest::update()
{
	checkForAdjacentChests();
	auto i = pos.getx();
	auto j = pos.gety();
	auto k = pos.getz();
	++ticksSinceSync;
	float f1;
	if (!world->isRemote && numPlayersUsing != 0 && (ticksSinceSync + i + j + k) % 200 == 0) 
	{
		numPlayersUsing = 0;
		f1 = 5.0F;
		Iterator var5 = world->getEntitiesWithinAABB(EntityPlayer.class, AxisAlignedBB((double)((float)i - 5.0F), (double)((float)j - 5.0F), (double)((float)k - 5.0F), (double)((float)(i + 1) + 5.0F), (double)((float)(j + 1) + 5.0F), (double)((float)(k + 1) + 5.0F))).iterator();

	label93:
		while(true) 
		{
			IInventory* iinventory;
			do 
			{
				EntityPlayer* entityplayer;
				do 
				{
					if (!var5.hasNext()) 
					{
						break label93;
					}

					entityplayer = (EntityPlayer*)var5.next();
				}
				while(!(Util::instanceof <ContainerChest>(entityplayer.openContainer)));

				iinventory = ((ContainerChest)entityplayer.openContainer).getLowerChestInventory();
			}
			while(iinventory != this && (!(Util::Util::instanceof<< InventoryLargeChest>(iinventory)) || !((InventoryLargeChest)iinventory).isPartOfLargeChest(this)));

			++numPlayersUsing;
		}
	}

	prevLidAngle = lidAngle;
	f1 = 0.1F;
	double d3;
	if (numPlayersUsing > 0 && lidAngle == 0.0F && adjacentChestZNeg == nullptr && adjacentChestXNeg == nullptr) 
	{
		double d1 = (double)i + 0.5;
		d3 = (double)k + 0.5;
		if (adjacentChestZPos != nullptr) 
		{
			d3 += 0.5;
		}

		if (adjacentChestXPos != nullptr) 
		{
			d1 += 0.5;
		}

		world->playSound((EntityPlayer)nullptr, d1, (double)j + 0.5, d3, SoundEvents.BLOCK_CHEST_OPEN, SoundCategory.BLOCKS, 0.5F, world->rand.nextFloat() * 0.1F + 0.9F);
	}

	if (numPlayersUsing == 0 && lidAngle > 0.0F || numPlayersUsing > 0 && lidAngle < 1.0F) 
	{
		float f2 = lidAngle;
		if (numPlayersUsing > 0) 
		{
			lidAngle += 0.1F;
		}
		else 
		{
			lidAngle -= 0.1F;
		}

		if (lidAngle > 1.0F) 
		{
			lidAngle = 1.0F;
		}

		float f3 = 0.5F;
		if (lidAngle < 0.5F && f2 >= 0.5F && adjacentChestZNeg == nullptr && adjacentChestXNeg == nullptr) 
		{
			d3 = (double)i + 0.5;
			double d0 = (double)k + 0.5;
			if (adjacentChestZPos != nullptr) 
			{
				d0 += 0.5;
			}

			if (adjacentChestXPos != nullptr) 
			{
				d3 += 0.5;
			}

			world->playSound(nullptr, d3, (double)j + 0.5, d0, SoundEvents::BLOCK_CHEST_CLOSE, SoundCategory::BLOCKS, 0.5F, MathHelper::nextFloat(world->rand) * 0.1F + 0.9F);
		}

		if (lidAngle < 0.0F) 
		{
			lidAngle = 0.0F;
		}
	}
}

bool TileEntityChest::receiveClientEvent(int32_t id, int32_t type)
{
	if (id == 1) 
	{
		numPlayersUsing = type;
		return true;
	}
	else 
	{
		return TileEntityLockableLoot::receiveClientEvent(id, type);
	}
}

void TileEntityChest::openInventory(EntityPlayer* player)
{
	if (!player->isSpectator()) 
	{
		if (numPlayersUsing < 0) 
		{
			numPlayersUsing = 0;
		}

		++numPlayersUsing;
		world->addBlockEvent(pos, getBlockType(), 1, numPlayersUsing);
		world->notifyNeighborsOfStateChange(pos, getBlockType(), false);
		if (getChestType() == BlockChest::Type::TRAP) 
		{
			world->notifyNeighborsOfStateChange(pos.down(), getBlockType(), false);
		}
	}
}

void TileEntityChest::closeInventory(EntityPlayer* player)
{
	if (!player->isSpectator() && Util::instanceof<BlockChest>(getBlockType())) 
	{
		--numPlayersUsing;
		world->addBlockEvent(pos, getBlockType(), 1, numPlayersUsing);
		world->notifyNeighborsOfStateChange(pos, getBlockType(), false);
		if (getChestType() == BlockChest::Type::TRAP) 
		{
			world->notifyNeighborsOfStateChange(pos.down(), getBlockType(), false);
		}
	}
}

BlockChest::Type TileEntityChest::getChestType()
{
	if (cachedChestType == nullptr) 
	{
		if (world == nullptr || !(Util::instanceof<BlockChest>(getBlockType())))
		{
			return BlockChest::Type::BASIC;
		}

		cachedChestType = ((BlockChest*)getBlockType()).chestType;
	}

	return cachedChestType;
}

std::string TileEntityChest::getGuiID() const
{
	return "minecraft:chest";
}

Container TileEntityChest::createContainer(InventoryPlayer* playerInventory, EntityPlayer* playerIn)
{
	fillWithLoot(playerIn);
	return ContainerChest(playerInventory, this, playerIn);
}

void TileEntityChest::invalidate()
{
	TileEntityLockableLoot::invalidate();
	updateContainingBlockInfo();
	checkForAdjacentChests();
}

TileEntityChest* TileEntityChest::getAdjacentChest(EnumFacing side)
{
	BlockPos blockpos = pos.offset(side);
	if (isChestAt(blockpos)) 
	{
		TileEntity* tileentity = world->getTileEntity(blockpos);
		if (Util::instanceof<TileEntityChest>(tileentity)) 
		{
			TileEntityChest* tileentitychest = (TileEntityChest*)tileentity;
			tileentitychest->setNeighbor(this, side.getOpposite());
			return tileentitychest;
		}
	}

	return nullptr;
}

std::vector<ItemStack> TileEntityChest::getItems()
{
	return chestContents;
}

void TileEntityChest::setNeighbor(TileEntityChest* chestTe, EnumFacing side)
{
	if (chestTe.isInvalid()) 
	{
		adjacentChestChecked = false;
	}
	else if (adjacentChestChecked) 
	{
		switch(side)
		{
		case EnumFacing::NORTH:
			if (adjacentChestZNeg != chestTe) 
			{
				adjacentChestChecked = false;
			}
			break;
		case EnumFacing::SOUTH:
			if (adjacentChestZPos != chestTe) 
			{
				adjacentChestChecked = false;
			}
			break;
		case EnumFacing::EAST:
			if (adjacentChestXPos != chestTe) 
			{
				adjacentChestChecked = false;
			}
			break;
		case EnumFacing::WEST:
			if (adjacentChestXNeg != chestTe) 
			{
				adjacentChestChecked = false;
			}
		}
	}
}

bool TileEntityChest::isChestAt(BlockPos posIn)
{
	if (world == nullptr) 
	{
		return false;
	}
	else 
	{
		Block block = world->getBlockState(posIn).getBlock();
		return Util::instanceof< BlockChest>(block) && ((BlockChest)block).chestType == getChestType();
	}
}
