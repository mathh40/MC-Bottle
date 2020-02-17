#include "TileEntityPiston.h"

TileEntityPiston::TileEntityPiston(IBlockState* pistonStateIn, EnumFacing pistonFacingIn, bool extendingIn,
	bool shouldHeadBeRenderedIn)
	:pistonState(pistonStateIn),pistonFacing(pistonFacingIn),extending(extendingIn),shouldHeadBeRendered(shouldHeadBeRenderedIn)
{
}

IBlockState* TileEntityPiston::getPistonState() const
{
	return pistonState;
}

NBTTagCompound* TileEntityPiston::getUpdateTag()
{
	return writeToNBT(new NBTTagCompound());
}

int32_t TileEntityPiston::getBlockMetadata()
{
	return 0;
}

bool TileEntityPiston::isExtending() const
{
	return extending;
}

EnumFacing TileEntityPiston::getFacing() const
{
	return pistonFacing;
}

bool TileEntityPiston::shouldPistonHeadBeRendered() const
{
	return shouldHeadBeRendered;
}

float TileEntityPiston::getProgress(float ticks) const
{
	if (ticks > 1.0F) 
	{
		ticks = 1.0F;
	}

	return lastProgress + (progress - lastProgress) * ticks;
}

float TileEntityPiston::getOffsetX(float ticks) const
{
	return (float)pistonFacing.getXOffset() * getExtendedProgress(getProgress(ticks));
}

float TileEntityPiston::getOffsetY(float ticks) const
{
	return (float)pistonFacing.getYOffset() * getExtendedProgress(getProgress(ticks));
}

float TileEntityPiston::getOffsetZ(float ticks) const
{
	return (float)pistonFacing.getZOffset() * getExtendedProgress(getProgress(ticks));
}

AxisAlignedBB TileEntityPiston::getAABB(IBlockAccess* p_184321_1_, BlockPos p_184321_2_)
{
	return getAABB(p_184321_1_, p_184321_2_, progress).aunion(getAABB(p_184321_1_, p_184321_2_, lastProgress));
}

AxisAlignedBB TileEntityPiston::getAABB(IBlockAccess* p_184319_1_, BlockPos p_184319_2_, float p_184319_3_)
{
	p_184319_3_ = getExtendedProgress(p_184319_3_);
	IBlockState* iblockstate = getCollisionRelatedBlockState();
	return iblockstate->getBoundingBox(p_184319_1_, p_184319_2_).offset((double)(p_184319_3_ * (float)pistonFacing.getXOffset()), (double)(p_184319_3_ * (float)pistonFacing.getYOffset()), (double)(p_184319_3_ * (float)pistonFacing.getZOffset()));
}

void TileEntityPiston::clearPistonTileEntity()
{
	if (lastProgress < 1.0F && world != nullptr) 
	{
		progress = 1.0F;
		lastProgress = progress;
		world->removeTileEntity(pos);
		invalidate();
		if (world->getBlockState(pos).getBlock() == Blocks::PISTON_EXTENSION) 
		{
			world->setBlockState(pos, pistonState, 3);
			world->neighborChanged(pos, pistonState->getBlock(), pos);
		}
	}
}

void TileEntityPiston::update()
{
	lastProgress = progress;
	if (lastProgress >= 1.0F) 
	{
		world->removeTileEntity(pos);
		invalidate();
		if (world->getBlockState(pos).getBlock() == Blocks::PISTON_EXTENSION) 
		{
			world->setBlockState(pos, pistonState, 3);
			world->neighborChanged(pos, pistonState->getBlock(), pos);
		}
	}
	else 
	{
		float f = progress + 0.5F;
		moveCollidedEntities(f);
		progress = f;
		if (progress >= 1.0F) 
		{
			progress = 1.0F;
		}
	}
}

void TileEntityPiston::registerFixesPiston(DataFixer fixer)
{
}

void TileEntityPiston::readFromNBT(NBTTagCompound* compound)
{
	TileEntity::readFromNBT(compound);
	pistonState = Block::getBlockById(compound->getInteger("blockId")).getStateFromMeta(compound->getInteger("blockData"));
	pistonFacing = EnumFacing::byIndex(compound->getInteger("facing"));
	progress = compound->getFloat("progress");
	lastProgress = progress;
	extending = compound->getBoolean("extending");
	shouldHeadBeRendered = compound->getBoolean("source");
}

NBTTagCompound* TileEntityPiston::writeToNBT(NBTTagCompound* compound)
{
	TileEntity::writeToNBT(compound);
	compound->setInteger("blockId", Block::getIdFromBlock(pistonState->getBlock()));
	compound->setInteger("blockData", pistonState->getBlock()->getMetaFromState(pistonState));
	compound->setInteger("facing", pistonFacing.getIndex());
	compound->setFloat("progress", lastProgress);
	compound->setBoolean("extending", extending);
	compound->setBoolean("source", shouldHeadBeRendered);
	return compound;
}

void TileEntityPiston::addCollissionAABBs(World* p_190609_1_, BlockPos p_190609_2_, AxisAlignedBB p_190609_3_,
	std::vector<AxisAlignedBB> p_190609_4_, Entity* p_190609_5_)
{
	if (!extending && shouldHeadBeRendered) 
	{
		pistonState->withProperty(BlockPistonBase::EXTENDED, true).addCollisionBoxToList(p_190609_1_, p_190609_2_, p_190609_3_, p_190609_4_, p_190609_5_, false);
	}

	EnumFacing enumfacing = (EnumFacing)MOVING_ENTITY.get();
	if ((double)progress >= 1.0 || enumfacing != (extending ? pistonFacing : pistonFacing.getOpposite())) {
		int i = p_190609_4_.size();
		IBlockState* iblockstate;
		if (shouldPistonHeadBeRendered()) {
			iblockstate = Blocks::PISTON_HEAD::getDefaultState().withProperty(BlockPistonExtension::FACING, pistonFacing).withProperty(BlockPistonExtension::SHORT, extending != 1.0F - progress < 0.25F);
		} else {
			iblockstate = pistonState;
		}

		float f = getExtendedProgress(progress);
		double d0 = (double)((float)pistonFacing.getXOffset() * f);
		double d1 = (double)((float)pistonFacing.getYOffset() * f);
		double d2 = (double)((float)pistonFacing.getZOffset() * f);
		iblockstate->addCollisionBoxToList(p_190609_1_, p_190609_2_, p_190609_3_.offset(-d0, -d1, -d2), p_190609_4_, p_190609_5_, true);

		for(int j = i; j < p_190609_4_.size(); ++j) {
			p_190609_4_[j] =  ((AxisAlignedBB)p_190609_4_[j]).offset(d0, d1, d2);
		}
	}
}

float TileEntityPiston::getExtendedProgress(float p_184320_1_) const
{
	return extending ? p_184320_1_ - 1.0F : 1.0F - p_184320_1_;
}

IBlockState* TileEntityPiston::getCollisionRelatedBlockState()
{
	return !isExtending() && shouldPistonHeadBeRendered() ? Blocks::PISTON_HEAD.getDefaultState().withProperty(BlockPistonExtension::TYPE, pistonState.getBlock() == Blocks::STICKY_PISTON ? BlockPistonExtension::EnumPistonType::STICKY : BlockPistonExtension::EnumPistonType::DEFAULT).withProperty(BlockPistonExtension::FACING, pistonState.getValue(BlockPistonBase::FACING)) : pistonState;
}

void TileEntityPiston::moveCollidedEntities(float p_184322_1_)
{
	EnumFacing enumfacing = extending ? pistonFacing : pistonFacing.getOpposite();
	double d0 = (double)(p_184322_1_ - progress);
	std::vector<AxisAlignedBB> list;
	getCollisionRelatedBlockState().addCollisionBoxToList(world, BlockPos::ORIGIN, AxisAlignedBB(BlockPos::ORIGIN), list, nullptr, true);
	if (!list.empty()) 
	{
		AxisAlignedBB axisalignedbb = moveByPositionAndProgress(getMinMaxPiecesAABB(list));
		auto list1 = world->getEntitiesWithinAABBExcludingEntity(nullptr, getMovementArea(axisalignedbb, enumfacing, d0).aunion(axisalignedbb));
		if (!list1.isEmpty()) 
		{
			boolean flag = pistonState.getBlock() == Blocks::SLIME_BLOCK;

			for(int i = 0; i < list1.size(); ++i) 
			{
				Entity* entity = list1.get(i);
				if (entity->getPushReaction() != EnumPushReaction::IGNORE) 
				{
					if (flag) 
					{
						switch(enumfacing.getAxis())
						{
						case Axis::X:
							entity.motionX = (double)enumfacing.getXOffset();
							break;
						case Axis::Y:
							entity.motionY = (double)enumfacing.getYOffset();
							break;
						case Axis::Z:
							entity.motionZ = (double)enumfacing.getZOffset();
						}
					}

					double d1 = 0.0;

					for(int j = 0; j < list.size(); ++j) {
						AxisAlignedBB axisalignedbb1 = getMovementArea(moveByPositionAndProgress((AxisAlignedBB)list[j]), enumfacing, d0);
						AxisAlignedBB axisalignedbb2 = entity->getEntityBoundingBox();
						if (axisalignedbb1.intersects(axisalignedbb2)) 
						{
							d1 = MathHelper::max(d1, getMovement(axisalignedbb1, enumfacing, axisalignedbb2));
							if (d1 >= d0) 
							{
								break;
							}
						}
					}

					if (d1 > 0.0) 
					{
						d1 = MathHelper::min(d1, d0) + 0.01;
						MOVING_ENTITY.set(enumfacing);
						entity->move(MoverType::PISTON, d1 * (double)enumfacing.getXOffset(), d1 * (double)enumfacing.getYOffset(), d1 * (double)enumfacing.getZOffset());
						MOVING_ENTITY.set((Object)nullptr);
						if (!extending && shouldHeadBeRendered) {
							fixEntityWithinPistonBase(entity, enumfacing, d0);
						}
					}
				}
			}
		}
	}
}

AxisAlignedBB TileEntityPiston::getMinMaxPiecesAABB(std::vector<AxisAlignedBB> p_191515_1_)
{
	double d0 = 0.0;
	double d1 = 0.0;
	double d2 = 0.0;
	double d3 = 1.0;
	double d4 = 1.0;
	double d5 = 1.0;

	AxisAlignedBB axisalignedbb;
	for(auto var14 = p_191515_1_.begin(); ; d5 = MathHelper::max(axisalignedbb.getmaxZ(), d5)) 
	{
		axisalignedbb = *var14;
		d0 = MathHelper::min(axisalignedbb.getminX(), d0);
		d1 = MathHelper::min(axisalignedbb.getminY(), d1);
		d2 = MathHelper::min(axisalignedbb.getminZ(), d2);
		d3 = MathHelper::max(axisalignedbb.getmaxX(), d3);
		d4 = MathHelper::max(axisalignedbb.getmaxY(), d4);
		var14++
	}

	return AxisAlignedBB(d0, d1, d2, d3, d4, d5);
}

double TileEntityPiston::getMovement(AxisAlignedBB p_190612_1_, EnumFacing facing, AxisAlignedBB p_190612_3_)
{
	switch(facing.getAxis())
	{
		case Axis::X:
			return getDeltaX(p_190612_1_, facing, p_190612_3_);
		case Axis::Y:
		default:
			return getDeltaY(p_190612_1_, facing, p_190612_3_);
		case Axis::Z:
			return getDeltaZ(p_190612_1_, facing, p_190612_3_);
	}
}

AxisAlignedBB TileEntityPiston::moveByPositionAndProgress(AxisAlignedBB p_190607_1_)
{
	double d0 = (double)getExtendedProgress(progress);
	return p_190607_1_.offset((double)pos.getx() + d0 * (double)pistonFacing.getXOffset(), (double)pos.gety() + d0 * (double)pistonFacing.getYOffset(), (double)pos.getz() + d0 * (double)pistonFacing.getZOffset());
}

AxisAlignedBB TileEntityPiston::getMovementArea(AxisAlignedBB p_190610_1_, EnumFacing p_190610_2_, double p_190610_3_)
{
	double d0 = p_190610_3_ * (double)p_190610_2_.getAxisDirection().getOffset();
	double d1 = MathHelper::min(d0, 0.0);
	double d2 = MathHelper::max(d0, 0.0);
	switch(p_190610_2_)
	{
	case EnumFacing::WEST:
		return AxisAlignedBB(p_190610_1_.getminX() + d1, p_190610_1_.getminY(), p_190610_1_.getminZ(), p_190610_1_.getminX() + d2, p_190610_1_.getmaxY(), p_190610_1_.getmaxZ());
	case EnumFacing::EAST:
		return AxisAlignedBB(p_190610_1_.getmaxX() + d1, p_190610_1_.getminY(), p_190610_1_.getminZ(), p_190610_1_.getmaxX() + d2, p_190610_1_.getmaxY(), p_190610_1_.getmaxZ());
	case EnumFacing::DOWN:
		return AxisAlignedBB(p_190610_1_.getminX(), p_190610_1_.getminY() + d1, p_190610_1_.getminZ(), p_190610_1_.getmaxX(), p_190610_1_.getminY() + d2, p_190610_1_.getmaxZ());
	case EnumFacing::UP:
	default:
		return AxisAlignedBB(p_190610_1_.getminX(), p_190610_1_.getmaxY() + d1, p_190610_1_.getminZ(), p_190610_1_.getmaxX(), p_190610_1_.getmaxY() + d2, p_190610_1_.getmaxZ());
	case EnumFacing::NORTH:
		return AxisAlignedBB(p_190610_1_.getminX(), p_190610_1_.getminY(), p_190610_1_.getminZ() + d1, p_190610_1_.getmaxX(), p_190610_1_.getmaxY(), p_190610_1_.getminZ() + d2);
	case EnumFacing::SOUTH:
		return AxisAlignedBB(p_190610_1_.getminX(), p_190610_1_.getminY(), p_190610_1_.getmaxZ() + d1, p_190610_1_.getmaxX(), p_190610_1_.getmaxY(), p_190610_1_.getmaxZ() + d2);
	}
}

void TileEntityPiston::fixEntityWithinPistonBase(Entity* p_190605_1_, EnumFacing p_190605_2_, double p_190605_3_)
{
	AxisAlignedBB axisalignedbb = p_190605_1_->getEntityBoundingBox();
	AxisAlignedBB axisalignedbb1 = Block::FULL_BLOCK_AABB.offset(pos);
	if (axisalignedbb.intersects(axisalignedbb1)) 
	{
		EnumFacing enumfacing = p_190605_2_.getOpposite();
		double d0 = getMovement(axisalignedbb1, enumfacing, axisalignedbb) + 0.01;
		double d1 = getMovement(axisalignedbb1, enumfacing, axisalignedbb.intersect(axisalignedbb1)) + 0.01;
		if (MathHelper::abs(d0 - d1) < 0.01) 
		{
			d0 = MathHelper::min(d0, p_190605_3_) + 0.01;
			MOVING_ENTITY.set(p_190605_2_);
			p_190605_1_->move(MoverType::PISTON, d0 * (double)enumfacing.getXOffset(), d0 * (double)enumfacing.getYOffset(), d0 * (double)enumfacing.getZOffset());
			MOVING_ENTITY.set((Object)nullptr);
		}
	}
}

double TileEntityPiston::getDeltaX(AxisAlignedBB p_190611_0_, EnumFacing facing, AxisAlignedBB p_190611_2_)
{
	return facing.getAxisDirection() == AxisDirection::POSITIVE ? p_190611_0_.getmaxX() - p_190611_2_.getminX() : p_190611_2_.getmaxX() - p_190611_0_.getminX();
}

double TileEntityPiston::getDeltaY(AxisAlignedBB p_190608_0_, EnumFacing facing, AxisAlignedBB p_190608_2_)
{
	return facing.getAxisDirection() == AxisDirection::POSITIVE ? p_190608_0_.getmaxY() - p_190608_2_.getminY() : p_190608_2_.getmaxY() - p_190608_0_.getminY();
}

double TileEntityPiston::getDeltaZ(AxisAlignedBB p_190604_0_, EnumFacing facing, AxisAlignedBB p_190604_2_)
{
	return facing.getAxisDirection() == AxisDirection::POSITIVE ? p_190604_0_.getmaxZ() - p_190604_2_.getminZ() : p_190604_2_.getmaxZ() - p_190604_0_.getminZ();
}
