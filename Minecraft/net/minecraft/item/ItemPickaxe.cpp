#include "ItemPickaxe.h"

#include "ItemStack.h"
#include "material/Material.h"

std::unordered_set<Block *> ItemPickaxe::EFFECTIVE_ON = {Blocks::ACTIVATOR_RAIL,
                                                         Blocks::COAL_ORE,
                                                         Blocks::COBBLESTONE,
                                                         Blocks::DETECTOR_RAIL,
                                                         Blocks::DIAMOND_BLOCK,
                                                         Blocks::DIAMOND_ORE,
                                                         Blocks::DOUBLE_STONE_SLAB,
                                                         Blocks::GOLDEN_RAIL,
                                                         Blocks::GOLD_BLOCK,
                                                         Blocks::GOLD_ORE,
                                                         Blocks::ICE,
                                                         Blocks::IRON_BLOCK,
                                                         Blocks::IRON_ORE,
                                                         Blocks::LAPIS_BLOCK,
                                                         Blocks::LAPIS_ORE,
                                                         Blocks::LIT_REDSTONE_ORE,
                                                         Blocks::MOSSY_COBBLESTONE,
                                                         Blocks::NETHERRACK,
                                                         Blocks::PACKED_ICE,
                                                         Blocks::RAIL,
                                                         Blocks::REDSTONE_ORE,
                                                         Blocks::SANDSTONE,
                                                         Blocks::RED_SANDSTONE,
                                                         Blocks::STONE,
                                                         Blocks::STONE_SLAB,
                                                         Blocks::STONE_BUTTON,
                                                         Blocks::STONE_PRESSURE_PLATE};

bool ItemPickaxe::canHarvestBlock(IBlockState *blockIn)
{
    Block *block = blockIn->getBlock();
    if (block == Blocks::OBSIDIAN)
    {
        return toolMaterial.getHarvestLevel() == 3;
    }
    else if (block != Blocks::DIAMOND_BLOCK && block != Blocks::DIAMOND_ORE)
    {
        if (block != Blocks::EMERALD_ORE && block != Blocks::EMERALD_BLOCK)
        {
            if (block != Blocks::GOLD_BLOCK && block != Blocks::GOLD_ORE)
            {
                if (block != Blocks::IRON_BLOCK && block != Blocks::IRON_ORE)
                {
                    if (block != Blocks::LAPIS_BLOCK && block != Blocks::LAPIS_ORE)
                    {
                        if (block != Blocks::REDSTONE_ORE && block != Blocks::LIT_REDSTONE_ORE)
                        {
                            Material material = blockIn->getMaterial();
                            if (material == Material::ROCK)
                            {
                                return true;
                            }
                            else if (material == Material::IRON)
                            {
                                return true;
                            }
                            else
                            {
                                return material == Material::ANVIL;
                            }
                        }
                        else
                        {
                            return toolMaterial.getHarvestLevel() >= 2;
                        }
                    }
                    else
                    {
                        return toolMaterial.getHarvestLevel() >= 1;
                    }
                }
                else
                {
                    return toolMaterial.getHarvestLevel() >= 1;
                }
            }
            else
            {
                return toolMaterial.getHarvestLevel() >= 2;
            }
        }
        else
        {
            return toolMaterial.getHarvestLevel() >= 2;
        }
    }
    else
    {
        return toolMaterial.getHarvestLevel() >= 2;
    }
}

float ItemPickaxe::getDestroySpeed(ItemStack stack, IBlockState *state)
{
    Material material = state->getMaterial();
    return material != Material::IRON && material != Material::ANVIL && material != Material::ROCK
               ? ItemTool::getDestroySpeed(stack, state)
               : efficiency;
}

ItemPickaxe::ItemPickaxe(ToolMaterial material) : ItemTool(1.0F, -2.8F, material, EFFECTIVE_ON)
{
}
