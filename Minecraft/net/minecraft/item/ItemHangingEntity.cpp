#include "ItemHangingEntity.h"

#include "EnumFacing.h"
#include "ItemCompass.h"
#include "ItemStack.h"

ItemHangingEntity::ItemHangingEntity(const std::type_index& entityClass)
    :hangingEntityClass(entityClass)
{
    setCreativeTab(CreativeTabs::DECORATIONS);
}

EnumActionResult ItemHangingEntity::onItemUse(EntityPlayer *player, World *worldIn, BlockPos pos, EnumHand hand,
    EnumFacing facing, float hitX, float hitY, float hitZ)
{
    ItemStack itemstack = player->getHeldItem(hand);
    BlockPos blockpos = pos.offset(facing);
    if (facing != EnumFacing::DOWN && facing != EnumFacing::UP && player->canPlayerEdit(blockpos, facing, itemstack)) 
    {
        EntityHanging* entityhanging = createEntity(worldIn, blockpos, facing);
        if (entityhanging != nullptr && entityhanging.onValidSurface()) 
        {
            if (!worldIn->isRemote) 
            {
                entityhanging.playPlaceSound();
                worldIn->spawnEntity(entityhanging);
            }

            itemstack.shrink(1);
        }

        return EnumActionResult::SUCCESS;
    }
    else 
    {
        return EnumActionResult::FAIL;
    }
}

EntityHanging* ItemHangingEntity::createEntity(World* worldIn, BlockPos pos, EnumFacing clickedSide)
{
    if (hangingEntityClass == typeid(EntityPainting)) 
    {
        return new EntityPainting(worldIn, pos, clickedSide);
    }
    else
    {
        return hangingEntityClass == typeid(EntityItemFrame) ? new EntityItemFrame(worldIn, pos, clickedSide) : nullptr;
    }
}
