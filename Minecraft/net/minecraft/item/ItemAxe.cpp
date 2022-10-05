#include "ItemAxe.h"

#include "../block/material/Material.h"
#include "ItemStack.h"

std::unordered_set<Block *> ItemAxe::EFFECTIVE_ON = {Blocks::PLANKS,
                                                     Blocks::BOOKSHELF,
                                                     Blocks::LOG,
                                                     Blocks::LOG2,
                                                     Blocks::CHEST,
                                                     Blocks::PUMPKIN,
                                                     Blocks::LIT_PUMPKIN,
                                                     Blocks::MELON_BLOCK,
                                                     Blocks::LADDER,
                                                     Blocks::WOODEN_BUTTON,
                                                     Blocks::WOODEN_PRESSURE_PLATE};

float ItemAxe::getDestroySpeed(ItemStack stack, IBlockState *state)
{
    Material material = state->getMaterial();
    return material != Material::WOOD && material != Material::PLANTS && material != Material::VINE
               ? ItemTool::getDestroySpeed(stack, state)
               : efficiency;
}

ItemAxe::ItemAxe(ToolMaterial material) : ItemTool(material, EFFECTIVE_ON)
{
    attackDamage = ATTACK_DAMAGES[material.ordinal()];
    attackSpeed  = ATTACK_SPEEDS[material.ordinal()];
}
