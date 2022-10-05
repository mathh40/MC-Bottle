#pragma once
#include "Item.h"
#include "ToolMaterial.h"

#include <unordered_set>

class ItemTool : public Item
{
  public:
    float getDestroySpeed(ItemStack stack, IBlockState *state) override;
    bool hitEntity(ItemStack stack, EntityLivingBase *target, EntityLivingBase *attacker) override;
    bool onBlockDestroyed(ItemStack stack, World *worldIn, IBlockState *state, BlockPos pos,
                          EntityLivingBase *entityLiving) override;
    bool isFull3D() const override;
    int32_t getItemEnchantability() override;
    std::string getToolMaterialName();
    bool getIsRepairable(ItemStack toRepair, ItemStack repair) override;
    std::unordered_multimap<std::string, AttributeModifier> getItemAttributeModifiers(
        EntityEquipmentSlot equipmentSlot) override;

  protected:
    ItemTool(float attackDamageIn, float attackSpeedIn, ToolMaterial materialIn,
             std::unordered_set<Block *> &effectiveBlocksIn);
    ItemTool(ToolMaterial materialIn, std::unordered_set<Block *> &effectiveBlocksIn);

    float efficiency;
    float attackDamage;
    float attackSpeed;
    ToolMaterial toolMaterial;

  private:
    std::unordered_set<> effectiveBlocks;
};
