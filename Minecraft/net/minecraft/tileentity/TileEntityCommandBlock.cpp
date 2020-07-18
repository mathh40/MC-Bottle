#include "TileEntityCommandBlock.h"

#include "ByteBuffer.h"

NBTTagCompound* TileEntityCommandBlock::writeToNBT(NBTTagCompound* compound)
{
	TileEntity::writeToNBT(compound);
	commandBlockLogic.writeToNBT(compound);
	compound->setBoolean("powered", isPowered());
	compound->setBoolean("conditionMet", isConditionMet());
	compound->setBoolean("auto", isAuto());
	return compound;
}

void TileEntityCommandBlock::readFromNBT(NBTTagCompound* compound)
{
	TileEntity::readFromNBT(compound);
	commandBlockLogic.readDataFromNBT(compound);
	powered = compound->getBoolean("powered");
	conditionMet = compound->getBoolean("conditionMet");
	setAuto(compound->getBoolean("auto"));
}

std::optional<SPacketUpdateTileEntity> TileEntityCommandBlock::getUpdatePacket()
{
	if (isSendToClient()) 
	{
		setSendToClient(false);
		auto nbttagcompound = writeToNBT(new NBTTagCompound());
		return SPacketUpdateTileEntity(pos, 2, nbttagcompound);
	}
	else 
	{
		return std::nullopt;
	}
}

bool TileEntityCommandBlock::onlyOpsCanSetNbt()
{
	return true;
}

CommandResultStats TileEntityCommandBlock::getCommandResultStats()
{
	return commandBlockLogic.getCommandResultStats();
}

void TileEntityCommandBlock::setPowered(bool poweredIn)
{
	powered = poweredIn;
}

bool TileEntityCommandBlock::isPowered() const
{
	return powered;
}

bool TileEntityCommandBlock::isAuto() const
{
	return automat;
}

void TileEntityCommandBlock::setAuto(bool autoIn)
{
	bool flag = automat;
	automat = autoIn;
	if (!flag && autoIn && !powered && world != nullptr && getMode() != TileEntityCommandBlock::Mode::SEQUENCE) 
	{
		Block* block = getBlockType();
		if (Util::instanceof <BlockCommandBlock>(block)) 
		{
			setConditionMet();
			world->scheduleUpdate(pos, block, block->tickRate(world));
		}
	}
}

bool TileEntityCommandBlock::isConditionMet() const
{
	return conditionMet;
}

bool TileEntityCommandBlock::setConditionMet()
{
	conditionMet = true;
	if (isConditional()) 
	{
		BlockPos* blockpos = pos.offset(((EnumFacing)world->getBlockState(pos).getValue(BlockCommandBlock::FACING)).getOpposite());
		if (world->getBlockState(blockpos).getBlock() instanceof BlockCommandBlock) 
		{
			TileEntity* tileentity = world->getTileEntity(blockpos);
			conditionMet = tileentity instanceof TileEntityCommandBlock && ((TileEntityCommandBlock)tileentity).getCommandBlockLogic().getSuccessCount() > 0;
		}
		else 
		{
			conditionMet = false;
		}
	}

	return conditionMet;
}

bool TileEntityCommandBlock::isSendToClient() const
{
	return sendToClient;
}

TileEntityCommandBlock::Mode getMode()
{
	Block* block = getBlockType();
	if (block == Blocks::COMMAND_BLOCK) 
	{
		return TileEntityCommandBlock::Mode::REDSTONE;
	}
	else if (block == Blocks::REPEATING_COMMAND_BLOCK) 
	{
		return TileEntityCommandBlock::Mode::AUTO;
	}
	else 
	{
		return block == Blocks::CHAIN_COMMAND_BLOCK ? TileEntityCommandBlock::Mode::SEQUENCE : TileEntityCommandBlock::Mode::REDSTONE;
	}
}

bool TileEntityCommandBlock::isConditional()
{
	IBlockState* iblockstate = world->getBlockState(getPos());
	return Util::instanceof< BlockCommandBlock>(iblockstate->getBlock()) ? iblockstate->getValue(BlockCommandBlock::CONDITIONAL) : false;
}

void TileEntityCommandBlock::validate()
{
	blockType = nullptr;
	TileEntity::validate();
}

void TileEntityCommandBlock::setSendToClient(bool p_184252_1_)
{
	sendToClient = p_184252_1_;
}

CommandBlockBaseLogic& TileEntityCommandBlock::getCommandBlockLogic() const
{
	return commandBlockLogic;
}

TileEntityCommandBlock::CommandBlockLogic::CommandBlockLogic(TileEntityCommandBlock* iblock)
	:block(iblock)
{
}

BlockPos TileEntityCommandBlock::CommandBlockLogic::getPosition() const
{
	return block->getPos();
}

Vec3d TileEntityCommandBlock::CommandBlockLogic::getPositionVector() const
{
	return Vec3d((double)block->getPos().getx() + 0.5, (double)block->getPos().gety() + 0.5, (double)block->getPos().getz() + 0.5);
}

World* TileEntityCommandBlock::CommandBlockLogic::getEntityWorld() const
{
	return block->getWorld();
}

void TileEntityCommandBlock::CommandBlockLogic::setCommand(std::string_view command)
{
	CommandBlockBaseLogic::setCommand(command);
	block->markDirty();
}

void TileEntityCommandBlock::CommandBlockLogic::updateCommand()
{
	IBlockState* iblockstate = block->getWorld()->getBlockState(block->getPos());
	block->getWorld()->notifyBlockUpdate(block->getPos(), iblockstate, iblockstate, 3);
}

void TileEntityCommandBlock::CommandBlockLogic::fillInInfo(ByteBuffer& buf)
{
	buf.putInt(block->getPos().getx());
	buf.putInt(block->getPos().gety());
	buf.putInt(block->getPos().getz());
}

MinecraftServer* TileEntityCommandBlock::CommandBlockLogic::getServer()
{
	return block->getWorld()->getMinecraftServer();
}

int32_t TileEntityCommandBlock::CommandBlockLogic::getCommandBlockType()
{
	return 0;
}
