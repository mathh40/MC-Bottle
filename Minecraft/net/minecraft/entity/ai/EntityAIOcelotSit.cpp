#include "EntityAIOcelotSit.h"

#include "../../tileentity/TileEntity.h"
#include "../../tileentity/TileEntityChest.h"

EntityAIOcelotSit::EntityAIOcelotSit(EntityOcelot *ocelotIn, double p_i45315_2_)
    : EntityAIMoveToBlock(ocelotIn, p_i45315_2_, 8), ocelot(ocelotIn)
{
}

bool EntityAIOcelotSit::shouldExecute()
{
    return ocelot->isTamed() && !ocelot->isSitting() && EntityAIMoveToBlock::shouldExecute();
}

void EntityAIOcelotSit::startExecuting()
{
    EntityAIMoveToBlock::startExecuting();
    ocelot->getAISit().setSitting(false);
}

void EntityAIOcelotSit::resetTask()
{
    EntityAIMoveToBlock::resetTask();
    ocelot->setSitting(false);
}

void EntityAIOcelotSit::updateTask()
{
    EntityAIMoveToBlock::updateTask();
    ocelot->getAISit().setSitting(false);
    if (!getIsAboveDestination())
    {
        ocelot->setSitting(false);
    }
    else if (!ocelot->isSitting())
    {
        ocelot->setSitting(true);
    }
}

bool EntityAIOcelotSit::shouldMoveTo(World *worldIn, BlockPos pos)
{
    if (!worldIn->isAirBlock(pos.up()))
    {
        return false;
    }
    else
    {
        IBlockState *iblockstate = worldIn->getBlockState(pos);
        Block *block             = iblockstate->getBlock();
        if (block == Blocks::CHEST)
        {
            TileEntity *tileentity = worldIn->getTileEntity(pos);
            if (Util:: instanceof <TileEntityChest>(tileentity) && ((TileEntityChest *)tileentity)->numPlayersUsing < 1)
            {
                return true;
            }
        }
        else
        {
            if (block == Blocks::LIT_FURNACE)
            {
                return true;
            }

            if (block == Blocks::BED && iblockstate.getValue(BlockBed::PART) != BlockBed::EnumPartType::HEAD)
            {
                return true;
            }
        }

        return false;
    }
}
