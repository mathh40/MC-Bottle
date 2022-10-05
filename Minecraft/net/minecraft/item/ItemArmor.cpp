#include "ItemArmor.h"

#include "ItemStack.h"
#include "Util.h"

std::vector<xg::Guid> ItemArmor::ARMOR_MODIFIERS = {
    xg::Guid("845DB27C-C624-495F-8C9F-6020A9A58B6B"), xg::Guid("D8499B04-0E66-4726-AB29-64469D734E0D"),
    xg::Guid("9F3D476D-C118-4544-8365-64846904B48E"), xg::Guid("2AD3F246-FEE1-4E67-B886-69FD380BB150")};

ItemStack ItemArmor::dispenseArmor(IBlockSource *blockSource, ItemStack stack)
{
    BlockPos blockpos =
        blockSource->getBlockPos().offset(blockSource->getBlockState()->getValue(BlockDispenser::FACING));
    auto list = blockSource->getWorld()->getEntitiesWithinAABB(
        EntityLivingBase.class, AxisAlignedBB(blockpos),
        Predicates.and (EntitySelectors.NOT_SPECTATING, EntitySelectors.ArmoredMob(stack)));
    if (list.isEmpty())
    {
        return ItemStack::EMPTY;
    }
    else
    {
        EntityLivingBase *entitylivingbase      = list.get(0);
        EntityEquipmentSlot entityequipmentslot = EntityLiving::getSlotForItemStack(stack);
        ItemStack itemstack                     = stack.splitStack(1);
        entitylivingbase.setItemStackToSlot(entityequipmentslot, itemstack);
        if (Util:: instanceof <EntityLiving>(entitylivingbase))
        {
            ((EntityLiving)entitylivingbase)->setDropChance(entityequipmentslot, 2.0F);
        }

        return stack;
    }
}

ItemArmor::ItemArmor(ArmorMaterial materialIn, int32_t renderIndexIn, EntityEquipmentSlot equipmentSlotIn)
    : material(materialIn), armorType(equipmentSlotIn), renderIndex(renderIndexIn),
      damageReduceAmount(materialIn.getDamageReductionAmount(equipmentSlotIn)), toughness(materialIn.getToughness())
{
    setMaxDamage(materialIn.getDurability(equipmentSlotIn));
    setCreativeTab(CreativeTabs::COMBAT);
    BlockDispenser::DISPENSE_BEHAVIOR_REGISTRY.putObject(this, DISPENSER_BEHAVIOR);
    maxStackSize = 1;
}

EntityEquipmentSlot ItemArmor::getEquipmentSlot()
{
    return armorType;
}

int32_t ItemArmor::getItemEnchantability()
{
    return material.getEnchantability();
}

ArmorMaterial ItemArmor::getArmorMaterial() const
{
    return material;
}

bool ItemArmor::hasColor(ItemStack stack)
{
    if (material != ArmorMaterial::LEATHER)
    {
        return false;
    }
    else
    {
        NBTTagCompound *nbttagcompound = stack.getTagCompound();
        return nbttagcompound != nullptr && nbttagcompound->hasKey("display", 10)
                   ? nbttagcompound->getCompoundTag("display").hasKey("color", 3)
                   : false;
    }
}

int ItemArmor::getColor(ItemStack stack)
{
    if (material != ArmorMaterial::LEATHER)
    {
        return 16777215;
    }
    else
    {
        NBTTagCompound *nbttagcompound = stack.getTagCompound();
        if (nbttagcompound != nullptr)
        {
            NBTTagCompound *nbttagcompound1 = nbttagcompound->getCompoundTag("display");
            if (nbttagcompound1 != nullptr && nbttagcompound1->hasKey("color", 3))
            {
                return nbttagcompound1->getInteger("color");
            }
        }

        return 10511680;
    }
}

void ItemArmor::removeColor(ItemStack stack)
{
    if (material == ArmorMaterial::LEATHER)
    {
        NBTTagCompound *nbttagcompound = stack.getTagCompound();
        if (nbttagcompound != nullptr)
        {
            NBTTagCompound *nbttagcompound1 = nbttagcompound->getCompoundTag("display");
            if (nbttagcompound1->hasKey("color"))
            {
                nbttagcompound1->removeTag("color");
            }
        }
    }
}

void ItemArmor::setColor(ItemStack stack, int32_t color)
{
    if (material != ArmorMaterial::LEATHER)
    {
        throw std::logic_error("Can't dye non-leather!");
    }
    else
    {
        NBTTagCompound *nbttagcompound = stack.getTagCompound();
        if (nbttagcompound == nullptr)
        {
            nbttagcompound = new NBTTagCompound();
            stack.setTagCompound(nbttagcompound);
        }

        NBTTagCompound *nbttagcompound1 = nbttagcompound->getCompoundTag("display");
        if (!nbttagcompound->hasKey("display", 10))
        {
            nbttagcompound->setTag("display", nbttagcompound1);
        }

        nbttagcompound1->setInteger("color", color);
    }
}

bool ItemArmor::getIsRepairable(ItemStack toRepair, ItemStack repair)
{
    return material.getRepairItem() == repair.getItem() ? true : Item::getIsRepairable(toRepair, repair);
}

ActionResult ItemArmor::onItemRightClick(World *worldIn, EntityPlayer *playerIn, EnumHand handIn)
{
    ItemStack itemstack                     = playerIn->getHeldItem(handIn);
    EntityEquipmentSlot entityequipmentslot = EntityLiving::getSlotForItemStack(itemstack);
    ItemStack itemstack1                    = playerIn->getItemStackFromSlot(entityequipmentslot);
    if (itemstack1.isEmpty())
    {
        playerIn->setItemStackToSlot(entityequipmentslot, itemstack.copy());
        itemstack.setCount(0);
        return ActionResult(EnumActionResult::SUCCESS, itemstack);
    }
    else
    {
        return ActionResult(EnumActionResult::FAIL, itemstack);
    }
}

std::unordered_multimap<std::string, AttributeModifier> ItemArmor::getItemAttributeModifiers(
    EntityEquipmentSlot equipmentSlot)
{
    auto multimap = Item::getItemAttributeModifiers(equipmentSlot);
    if (equipmentSlot == armorType)
    {
        multimap.emplace(SharedMonsterAttributes::ARMOR.getName(),
                         AttributeModifier(ARMOR_MODIFIERS[equipmentSlot.getIndex()], "Armor modifier",
                                           (double)damageReduceAmount, 0));
        multimap.emplace(
            SharedMonsterAttributes::ARMOR_TOUGHNESS.getName(),
            AttributeModifier(ARMOR_MODIFIERS[equipmentSlot.getIndex()], "Armor toughness", (double)toughness, 0));
    }

    return multimap
}
