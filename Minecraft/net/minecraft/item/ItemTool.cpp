#include "ItemTool.h"

#include "ItemStack.h"

float ItemTool::getDestroySpeed(ItemStack stack, IBlockState *state)
{
    return effectiveBlocks.contains(state->getBlock()) ? efficiency : 1.0F;
}

bool ItemTool::hitEntity(ItemStack stack, EntityLivingBase *target, EntityLivingBase *attacker)
{
    stack.damageItem(2, attacker);
    return true;
}

bool ItemTool::onBlockDestroyed(ItemStack stack, World *worldIn, IBlockState *state, BlockPos pos,
                                EntityLivingBase *entityLiving)
{
    if (!worldIn->isRemote && (double)state->getBlockHardness(worldIn, pos) != 0.0)
    {
        stack.damageItem(1, entityLiving);
    }

    return true;
}

bool ItemTool::isFull3D() const
{
    return true;
}

int32_t ItemTool::getItemEnchantability()
{
    return toolMaterial.getEnchantability();
}

std::string ItemTool::getToolMaterialName()
{
    return toolMaterial.toString();
}

std::unordered_multimap<std::string, AttributeModifier> ItemTool::getItemAttributeModifiers(
    EntityEquipmentSlot equipmentSlot)
{
    std::unordered_multimap<std::string, AttributeModifier> multimap = Item::getItemAttributeModifiers(equipmentSlot);
    if (equipmentSlot == EntityEquipmentSlot::MAINHAND)
    {
        multimap.emplace(SharedMonsterAttributes::ATTACK_DAMAGE.getName(),
                         AttributeModifier(ATTACK_DAMAGE_MODIFIER, "Tool modifier", (double)attackDamage, 0));
        multimap.emplace(SharedMonsterAttributes::ATTACK_SPEED.getName(),
                         AttributeModifier(ATTACK_SPEED_MODIFIER, "Tool modifier", (double)attackSpeed, 0));
    }

    return multimap;
}

bool ItemTool::getIsRepairable(ItemStack toRepair, ItemStack repair)
{
    return toolMaterial.getRepairItem() == repair.getItem() ? true : Item::getIsRepairable(toRepair, repair);
}

ItemTool::ItemTool(float attackDamageIn, float attackSpeedIn, Item, ToolMaterial materialIn,
                   std::unordered_set<Block *> &effectiveBlocksIn)
    : efficiency(materialIn.getEfficiency()), toolMaterial(materialIn), effectiveBlocks(effectiveBlocksIn),
      attackDamage(attackDamageIn + materialIn.getAttackDamage()), attackSpeed(attackSpeedIn)
{
    maxStackSize = 1;
    setMaxDamage(materialIn.getMaxUses());
    setCreativeTab(CreativeTabs::TOOLS);
}

ItemTool::ItemTool(ToolMaterial materialIn, std::unordered_set<Block *> &effectiveBlocksIn)
    : ItemTool(0.0F, 0.0F, materialIn, effectiveBlocksIn)
{
}
