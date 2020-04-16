#include "ItemHoe.h"


#include "EnumFacing.h"
#include "ItemStack.h"
#include "SoundCategory.h"
#include "../world/biome/BiomeMesa.h"

ItemHoe::ItemHoe(const ToolMaterial &material)
    :toolMaterial(material)
{
    maxStackSize = 1;
    setMaxDamage(material.getMaxUses());
    setCreativeTab(CreativeTabs::TOOLS);
    speed = material.getAttackDamage() + 1.0F;
}

EnumActionResult ItemHoe::onItemUse(EntityPlayer *player, World *worldIn, BlockPos pos, EnumHand hand,
    EnumFacing facing, float hitX, float hitY, float hitZ)
{
    ItemStack itemstack = player->getHeldItem(hand);
    if (!player->canPlayerEdit(pos.offset(facing), facing, itemstack)) 
    {
        return EnumActionResult::FAIL;
    }
    else 
    {
        IBlockState* iblockstate = worldIn->getBlockState(pos);
        Block* block = iblockstate->getBlock();
        if (facing != EnumFacing::DOWN && worldIn->getBlockState(pos.up()).getMaterial() != Material::AIR) 
        {
            if (block == Blocks::GRASS || block == Blocks::GRASS_PATH) 
            {
                setBlock(itemstack, player, worldIn, pos, Blocks::FARMLAND.getDefaultState());
                return EnumActionResult::SUCCESS;
            }

            if (block == Blocks::DIRT) 
            {
                switch((BlockDirt::DirtType**)iblockstate->getValue(BlockDirt::VARIANT))
                {
                case BlockDirt::DirtType::DIRT:
                    setBlock(itemstack, player, worldIn, pos, Blocks::FARMLAND.getDefaultState());
                    return EnumActionResult::SUCCESS;
                case BlockDirt::DirtType::COARSE_DIRT:
                    setBlock(itemstack, player, worldIn, pos, Blocks::DIRT.getDefaultState().withProperty(BlockDirt::VARIANT, BlockDirt::DirtType::DIRT));
                    return EnumActionResult::SUCCESS;
                }
            }
        }

        return EnumActionResult::PASS;
    }
}

bool ItemHoe::hitEntity(ItemStack stack, EntityLivingBase *target, EntityLivingBase *attacker)
{
    stack.damageItem(1, attacker);
    return true;
}

bool ItemHoe::isFull3D() const
{
    return true;
}

std::string ItemHoe::getMaterialName() const
{
    return toolMaterial.toString();
}

void ItemHoe::setBlock(ItemStack stack, EntityPlayer *player, World *worldIn, BlockPos pos, IBlockState *state)
{
    worldIn->playSound(player, pos, SoundEvents::ITEM_HOE_TILL, SoundCategory::BLOCKS, 1.0F, 1.0F);
    if (!worldIn->isRemote) 
    {
        worldIn->setBlockState(pos, state, 11);
        stack.damageItem(1, player);
    }
}

std::unordered_multimap<std::string,AttributeModifier> ItemHoe::getItemAttributeModifiers(EntityEquipmentSlot equipmentSlot)
{
    std::unordered_multimap<std::string,AttributeModifier> multimap = super.getItemAttributeModifiers(equipmentSlot);
    if (equipmentSlot == EntityEquipmentSlot::MAINHAND) 
    {
        multimap.emplace(SharedMonsterAttributes::ATTACK_DAMAGE.getName(), AttributeModifier(ATTACK_DAMAGE_MODIFIER, "Weapon modifier", 0.0, 0));
        multimap.emplace(SharedMonsterAttributes::ATTACK_SPEED.getName(), AttributeModifier(ATTACK_SPEED_MODIFIER, "Weapon modifier", (double)(speed - 4.0F), 0));
    }

    return multimap;
}
