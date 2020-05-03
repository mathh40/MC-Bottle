#include "ItemSword.h"

#include "ItemStack.h"
#include "material/Material.h"

ItemSword::ItemSword(ToolMaterial material)
    :material(material)
{
    maxStackSize = 1;
    setMaxDamage(material.getMaxUses());
    setCreativeTab(CreativeTabs::COMBAT);
    attackDamage = 3.0F + material.getAttackDamage();
}

float ItemSword::getAttackDamage() const
{
    return material.getAttackDamage();
}

float ItemSword::getDestroySpeed(ItemStack stack, IBlockState *state)
{
    Block block = state->getBlock();
    if (block == Blocks::WEB) 
    {
        return 15.0F;
    }
    else 
    {
        Material material = state->getMaterial();
        return material != Material::PLANTS && material != Material::VINE && material != Material::CORAL && material != Material::LEAVES && material != Material::GOURD ? 1.0F : 1.5F;
    }
}

bool ItemSword::hitEntity(ItemStack stack, EntityLivingBase *target, EntityLivingBase *attacker)
{
    stack.damageItem(1, attacker);
    return true;
}

bool ItemSword::onBlockDestroyed(ItemStack stack, World *worldIn, IBlockState *state, BlockPos pos,
    EntityLivingBase *entityLiving)
{
    if ((double)state->getBlockHardness(worldIn, pos) != 0.0) 
    {
        stack.damageItem(2, entityLiving);
    }

    return true;
}

bool ItemSword::isFull3D() const
{
    return true;
}

bool ItemSword::canHarvestBlock(IBlockState* blockIn)
{
    return blockIn->getBlock() == Blocks::WEB;
}

int32_t ItemSword::getItemEnchantability()
{
    return material.getEnchantability();
}

std::string ItemSword::getToolMaterialName() const
{
    return material.toString();
}

bool ItemSword::getIsRepairable(ItemStack toRepair, ItemStack repair)
{
    return material.getRepairItem() == repair.getItem() ? true : Item::getIsRepairable(toRepair, repair);
}

std::unordered_multimap<std::string,AttributeModifier> ItemSword::getItemAttributeModifiers(EntityEquipmentSlot equipmentSlot)
{
    std::unordered_multimap<std::string,AttributeModifier> multimap = Item::getItemAttributeModifiers(equipmentSlot);
    if (equipmentSlot == EntityEquipmentSlot.MAINHAND) 
    {
        multimap.emplace(SharedMonsterAttributes::ATTACK_DAMAGE.getName(), AttributeModifier(ATTACK_DAMAGE_MODIFIER, "Weapon modifier", (double)attackDamage, 0));
        multimap.emplace(SharedMonsterAttributes::ATTACK_SPEED.getName(), AttributeModifier(ATTACK_SPEED_MODIFIER, "Weapon modifier", -2.4000000953674316, 0));
    }

    return multimap;
}