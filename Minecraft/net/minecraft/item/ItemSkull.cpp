#include "ItemSkull.h"


#include "ItemStack.h"
#include "StringUtils.h"
#include "../nbt/NBTUtil.h"
#include "../tileentity/TileEntity.h"
#include "../tileentity/TileEntitySkull.h"
#include "text/translation/I18n.h"

ItemSkull::ItemSkull()
{
    setCreativeTab(CreativeTabs::DECORATIONS);
    setMaxDamage(0);
    setHasSubtypes(true);
}

EnumActionResult ItemSkull::onItemUse(EntityPlayer *player, World *worldIn, BlockPos pos, EnumHand hand,
    EnumFacing facing, float hitX, float hitY, float hitZ)
{
    if (facing == EnumFacing::DOWN) 
    {
        return EnumActionResult::FAIL;
    }
    else 
    {
        IBlockState* iblockstate = worldIn->getBlockState(pos);
        Block* block = iblockstate->getBlock();
        bool flag = block->isReplaceable(worldIn, pos);
        if (!flag) 
        {
            if (!worldIn->getBlockState(pos).getMaterial().isSolid()) 
            {
                return EnumActionResult::FAIL;
            }

            pos = pos.offset(facing);
        }

        ItemStack itemstack = player->getHeldItem(hand);
        if (player->canPlayerEdit(pos, facing, itemstack) && Blocks::SKULL.canPlaceBlockAt(worldIn, pos)) 
        {
            if (worldIn->isRemote) 
            {
                return EnumActionResult.SUCCESS;
            }
            else 
            {
                worldIn->setBlockState(pos, Blocks::SKULL.getDefaultState().withProperty(BlockSkull::FACING, facing), 11);
                auto i = 0;
                if (facing == EnumFacing::UP) 
                {
                    i = MathHelper::floor((double)(player->rotationYaw * 16.0F / 360.0F) + 0.5) & 15;
                }

                TileEntity* tileentity = worldIn->getTileEntity(pos);
                if (Util::instanceof<TileEntitySkull>(tileentity)) 
                {
                    TileEntitySkull* tileentityskull = (TileEntitySkull*)tileentity;
                    if (itemstack.getMetadata() == 3) 
                    {
                        GameProfile gameprofile = nullptr;
                        if (itemstack.hasTagCompound()) 
                        {
                            NBTTagCompound* nbttagcompound = itemstack.getTagCompound();
                            if (nbttagcompound->hasKey("SkullOwner", 10)) 
                            {
                                gameprofile = NBTUtil::readGameProfileFromNBT(nbttagcompound->getCompoundTag("SkullOwner"));
                            }
                            else if (nbttagcompound->hasKey("SkullOwner", 8) && !StringUtils::isBlank(nbttagcompound->getString("SkullOwner"))) 
                            {
                                gameprofile = GameProfile(std::nullopt, nbttagcompound->getString("SkullOwner"));
                            }
                        }

                        tileentityskull.setPlayerProfile(gameprofile);
                    }
                    else 
                    {
                        tileentityskull.setType(itemstack.getMetadata());
                    }

                    tileentityskull.setSkullRotation(i);
                    Blocks::SKULL.checkWitherSpawn(worldIn, pos, tileentityskull);
                }

                if (Util::instanceof<EntityPlayerMP>(player)) 
                {
                    CriteriaTriggers::PLACED_BLOCK.trigger((EntityPlayerMP*)player, pos, itemstack);
                }

                itemstack.shrink(1);
                return EnumActionResult::SUCCESS;
            }
        } else {
            return EnumActionResult::FAIL;
        }
    }
}

void ItemSkull::getSubItems(const CreativeTabs& tab, std::vector<ItemStack>& items)
{
    if (isInCreativeTab(tab)) 
    {
        for(auto i = 0; i < SKULL_TYPES.size(); ++i) 
        {
            items.emplace_back(ItemStack(this, 1, i));
        }
      }
}

int32_t ItemSkull::getMetadata(int32_t damage) const
{
    return damage;
}

std::string ItemSkull::getItemStackDisplayName(ItemStack stack) const
{
    if (stack.getMetadata() == 3 && stack.hasTagCompound()) 
    {
        if (stack.getTagCompound()->hasKey("SkullOwner", 8)) 
        {
            return I18n::translateToLocalFormatted("item.skull.player.name", stack.getTagCompound()->getString("SkullOwner"));
        }

        if (stack.getTagCompound()->hasKey("SkullOwner", 10)) 
        {
            NBTTagCompound* nbttagcompound = stack.getTagCompound()->getCompoundTag("SkullOwner");
            if (nbttagcompound->hasKey("Name", 8)) 
            {
                return I18n::translateToLocalFormatted("item.skull.player.name", nbttagcompound->getString("Name"));
            }
        }
      }

    return Item::getItemStackDisplayName(stack);
}

bool ItemSkull::updateItemStackNBT(NBTTagCompound *nbt)
{
    Item::updateItemStackNBT(nbt);
    if (nbt->hasKey("SkullOwner", 8) && !StringUtils::isBlank(nbt->getString("SkullOwner"))) 
    {
        GameProfile gameprofile = GameProfile(std::nullopt, nbt->getString("SkullOwner"));
        gameprofile = TileEntitySkull::updateGameProfile(gameprofile);
        nbt->setTag("SkullOwner", NBTUtil::writeGameProfile(new NBTTagCompound(), gameprofile));
        return true;
    }
    else 
    {
        return false;
    }
}

std::string ItemSkull::getTranslationKey(ItemStack stack)
{
    auto i = stack.getMetadata();
    if (i < 0 || i >= SKULL_TYPES.size()) 
    {
        i = 0;
    }

    return Item::getTranslationKey() + "." + SKULL_TYPES[i];
}
