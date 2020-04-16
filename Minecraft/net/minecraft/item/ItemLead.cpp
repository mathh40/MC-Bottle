#include "ItemLead.h"

#include "EnumFacing.h"
#include "../pathfinding/NodeProcessor.h"
#include "../world/World.h"

ItemLead::ItemLead()
{
    setCreativeTab(CreativeTabs::TOOLS);
}

EnumActionResult ItemLead::onItemUse(EntityPlayer *player, World *worldIn, BlockPos pos, EnumHand hand,
    EnumFacing facing, float hitX, float hitY, float hitZ)
{
    Block* block = worldIn->getBlockState(pos).getBlock();
    if (!(Util::instanceof<BlockFence>(block))) 
    {
        return EnumActionResult::PASS;
    }
    else 
    {
        if (!worldIn->isRemote) 
        {
            attachToFence(player, worldIn, pos);
        }

        return EnumActionResult::SUCCESS;
    }
}

bool ItemLead::attachToFence(EntityPlayer *player, World *worldIn, BlockPos fence)
{
    EntityLeashKnot* entityleashknot = EntityLeashKnot::getKnotForPosition(worldIn, fence);
    bool flag = false;
    double d0 = 7.0;
    auto i = fence.getx();
    auto j = fence.gety();
    auto k = fence.getz();
    Iterator var10 = worldIn->getEntitiesWithinAABB(EntityLiving.class, new AxisAlignedBB((double)i - 7.0, (double)j - 7.0, (double)k - 7.0, (double)i + 7.0, (double)j + 7.0, (double)k + 7.0)).iterator();

    while(var10.hasNext()) 
    {
        EntityLiving* entityliving = (EntityLiving)var10.next();
        if (entityliving->getLeashed() && entityliving->getLeashHolder() == player) {
            if (entityleashknot == nullptr) 
            {
                entityleashknot = EntityLeashKnot::createKnot(worldIn, fence);
            }

            entityliving->setLeashHolder(entityleashknot, true);
            flag = true;
        }
    }

    return flag;
}
