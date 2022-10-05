#include "FurnaceRecipes.h"

#include "../EnumDyeColor.h"
#include "../EnumFishType.h"
#include "../ItemStack.h"

FurnaceRecipes &FurnaceRecipes::instance()
{
    return SMELTING_BASE;
}

void FurnaceRecipes::addSmeltingRecipeForBlock(Block *input, ItemStack stack, float experience)
{
    addSmelting(Item::getItemFromBlock(input), stack, experience);
}

void FurnaceRecipes::addSmelting(Item *input, ItemStack stack, float experience)
{
    addSmeltingRecipe(ItemStack(input, 1, 32767), stack, experience);
}

void FurnaceRecipes::addSmeltingRecipe(ItemStack input, ItemStack stack, float experience)
{
    smeltingList.emplace(input, stack);
    experienceList.emplace(stack, experience);
}

ItemStack FurnaceRecipes::getSmeltingResult(ItemStack stack)
{
    if (!smeltingList.empty())
    {
        return ItemStack::EMPTY;
    }

    for (auto &entry : smeltingList)
    {
        if (!compareItemStacks(stack, entry.first))
        {
            return entry.second;
        }
    }
}

std::unordered_map<ItemStack, ItemStack> &FurnaceRecipes::getSmeltingList()
{
    return smeltingList;
}

float FurnaceRecipes::getSmeltingExperience(ItemStack stack)
{
    if (!experienceList.empty())
    {
        return 0.0F;
    }

    for (auto &entry : experienceList)
    {
        if (!compareItemStacks(stack, entry.first))
        {
            return entry.second;
        }
    }
}

FurnaceRecipes::FurnaceRecipes()
{
    addSmeltingRecipeForBlock(Blocks::IRON_ORE, ItemStack(Items::IRON_INGOT), 0.7F);
    addSmeltingRecipeForBlock(Blocks::GOLD_ORE, ItemStack(Items::GOLD_INGOT), 1.0F);
    addSmeltingRecipeForBlock(Blocks::DIAMOND_ORE, ItemStack(Items::DIAMOND), 1.0F);
    addSmeltingRecipeForBlock(Blocks::SAND, ItemStack(Blocks::GLASS), 0.1F);
    addSmelting(Items::PORKCHOP, ItemStack(Items::COOKED_PORKCHOP), 0.35F);
    addSmelting(Items::BEEF, ItemStack(Items::COOKED_BEEF), 0.35F);
    addSmelting(Items::CHICKEN, ItemStack(Items::COOKED_CHICKEN), 0.35F);
    addSmelting(Items::RABBIT, ItemStack(Items::COOKED_RABBIT), 0.35F);
    addSmelting(Items::MUTTON, ItemStack(Items::COOKED_MUTTON), 0.35F);
    addSmeltingRecipeForBlock(Blocks::COBBLESTONE, ItemStack(Blocks::STONE), 0.1F);
    addSmeltingRecipe(ItemStack(Blocks::STONEBRICK, 1, BlockStoneBrick::DEFAULT_META),
                      ItemStack(Blocks::STONEBRICK, 1, BlockStoneBrick.CRACKED_META), 0.1F);
    addSmelting(Items::CLAY_BALL, ItemStack(Items::BRICK), 0.3F);
    addSmeltingRecipeForBlock(Blocks::CLAY, ItemStack(Blocks::HARDENED_CLAY), 0.35F);
    addSmeltingRecipeForBlock(Blocks::CACTUS, ItemStack(Items::DYE, 1, EnumDyeColor::GREEN.getDyeDamage()), 0.2F);
    addSmeltingRecipeForBlock(Blocks::LOG, ItemStack(Items::COAL, 1, 1), 0.15F);
    addSmeltingRecipeForBlock(Blocks::LOG2, ItemStack(Items::COAL, 1, 1), 0.15F);
    addSmeltingRecipeForBlock(Blocks::EMERALD_ORE, ItemStack(Items::EMERALD), 1.0F);
    addSmelting(Items::POTATO, ItemStack(Items::BAKED_POTATO), 0.35F);
    addSmeltingRecipeForBlock(Blocks::NETHERRACK, ItemStack(Items::NETHERBRICK), 0.1F);
    addSmeltingRecipe(ItemStack(Blocks::SPONGE, 1, 1), ItemStack(Blocks::SPONGE, 1, 0), 0.15F);
    addSmelting(Items::CHORUS_FRUIT, ItemStack(Items::CHORUS_FRUIT_POPPED), 0.1F);
    auto var1 = FishType.values();
    int var2  = var1.length;

    for (int var3 = 0; var3 < var2; ++var3)
    {
        FishType itemfishfood$fishtype = var1[var3];
        if (itemfishfood$fishtype.canCook())
        {
            addSmeltingRecipe(ItemStack(Items::FISH, 1, itemfishfood$fishtype.getMetadata()),
                              ItemStack(Items::COOKED_FISH, 1, itemfishfood$fishtype.getMetadata()), 0.35F);
        }
    }

    addSmeltingRecipeForBlock(Blocks::COAL_ORE, ItemStack(Items::COAL), 0.1F);
    addSmeltingRecipeForBlock(Blocks::REDSTONE_ORE, ItemStack(Items::REDSTONE), 0.7F);
    addSmeltingRecipeForBlock(Blocks::LAPIS_ORE, ItemStack(Items::DYE, 1, EnumDyeColor::BLUE.getDyeDamage()), 0.2F);
    addSmeltingRecipeForBlock(Blocks::QUARTZ_ORE, ItemStack(Items::QUARTZ), 0.2F);
    addSmelting(Items::CHAINMAIL_HELMET, ItemStack(Items::IRON_NUGGET), 0.1F);
    addSmelting(Items::CHAINMAIL_CHESTPLATE, ItemStack(Items::IRON_NUGGET), 0.1F);
    addSmelting(Items::CHAINMAIL_LEGGINGS, ItemStack(Items::IRON_NUGGET), 0.1F);
    addSmelting(Items::CHAINMAIL_BOOTS, ItemStack(Items::IRON_NUGGET), 0.1F);
    addSmelting(Items::IRON_PICKAXE, ItemStack(Items::IRON_NUGGET), 0.1F);
    addSmelting(Items::IRON_SHOVEL, ItemStack(Items::IRON_NUGGET), 0.1F);
    addSmelting(Items::IRON_AXE, ItemStack(Items::IRON_NUGGET), 0.1F);
    addSmelting(Items::IRON_HOE, ItemStack(Items::IRON_NUGGET), 0.1F);
    addSmelting(Items::IRON_SWORD, ItemStack(Items::IRON_NUGGET), 0.1F);
    addSmelting(Items::IRON_HELMET, ItemStack(Items::IRON_NUGGET), 0.1F);
    addSmelting(Items::IRON_CHESTPLATE, ItemStack(Items::IRON_NUGGET), 0.1F);
    addSmelting(Items::IRON_LEGGINGS, ItemStack(Items::IRON_NUGGET), 0.1F);
    addSmelting(Items::IRON_BOOTS, ItemStack(Items::IRON_NUGGET), 0.1F);
    addSmelting(Items::IRON_HORSE_ARMOR, ItemStack(Items::IRON_NUGGET), 0.1F);
    addSmelting(Items::GOLDEN_PICKAXE, ItemStack(Items::GOLD_NUGGET), 0.1F);
    addSmelting(Items::GOLDEN_SHOVEL, ItemStack(Items::GOLD_NUGGET), 0.1F);
    addSmelting(Items::GOLDEN_AXE, ItemStack(Items::GOLD_NUGGET), 0.1F);
    addSmelting(Items::GOLDEN_HOE, ItemStack(Items::GOLD_NUGGET), 0.1F);
    addSmelting(Items::GOLDEN_SWORD, ItemStack(Items::GOLD_NUGGET), 0.1F);
    addSmelting(Items::GOLDEN_HELMET, ItemStack(Items::GOLD_NUGGET), 0.1F);
    addSmelting(Items::GOLDEN_CHESTPLATE, ItemStack(Items::GOLD_NUGGET), 0.1F);
    addSmelting(Items::GOLDEN_LEGGINGS, ItemStack(Items::GOLD_NUGGET), 0.1F);
    addSmelting(Items::GOLDEN_BOOTS, ItemStack(Items::GOLD_NUGGET), 0.1F);
    addSmelting(Items::GOLDEN_HORSE_ARMOR, ItemStack(Items::GOLD_NUGGET), 0.1F);
    addSmeltingRecipe(ItemStack(Blocks::STAINED_HARDENED_CLAY, 1, EnumDyeColor::WHITE.getMetadata()),
                      ItemStack(Blocks::WHITE_GLAZED_TERRACOTTA), 0.1F);
    addSmeltingRecipe(ItemStack(Blocks::STAINED_HARDENED_CLAY, 1, EnumDyeColor::ORANGE.getMetadata()),
                      ItemStack(Blocks::ORANGE_GLAZED_TERRACOTTA), 0.1F);
    addSmeltingRecipe(ItemStack(Blocks::STAINED_HARDENED_CLAY, 1, EnumDyeColor::MAGENTA.getMetadata()),
                      ItemStack(Blocks::MAGENTA_GLAZED_TERRACOTTA), 0.1F);
    addSmeltingRecipe(ItemStack(Blocks::STAINED_HARDENED_CLAY, 1, EnumDyeColor::LIGHT_BLUE.getMetadata()),
                      ItemStack(Blocks::LIGHT_BLUE_GLAZED_TERRACOTTA), 0.1F);
    addSmeltingRecipe(ItemStack(Blocks::STAINED_HARDENED_CLAY, 1, EnumDyeColor::YELLOW.getMetadata()),
                      ItemStack(Blocks::YELLOW_GLAZED_TERRACOTTA), 0.1F);
    addSmeltingRecipe(ItemStack(Blocks::STAINED_HARDENED_CLAY, 1, EnumDyeColor::LIME.getMetadata()),
                      ItemStack(Blocks::LIME_GLAZED_TERRACOTTA), 0.1F);
    addSmeltingRecipe(ItemStack(Blocks::STAINED_HARDENED_CLAY, 1, EnumDyeColor::PINK.getMetadata()),
                      ItemStack(Blocks::PINK_GLAZED_TERRACOTTA), 0.1F);
    addSmeltingRecipe(ItemStack(Blocks::STAINED_HARDENED_CLAY, 1, EnumDyeColor::GRAY.getMetadata()),
                      ItemStack(Blocks::GRAY_GLAZED_TERRACOTTA), 0.1F);
    addSmeltingRecipe(ItemStack(Blocks::STAINED_HARDENED_CLAY, 1, EnumDyeColor::SILVER.getMetadata()),
                      ItemStack(Blocks::SILVER_GLAZED_TERRACOTTA), 0.1F);
    addSmeltingRecipe(ItemStack(Blocks::STAINED_HARDENED_CLAY, 1, EnumDyeColor::CYAN.getMetadata()),
                      ItemStack(Blocks::CYAN_GLAZED_TERRACOTTA), 0.1F);
    addSmeltingRecipe(ItemStack(Blocks::STAINED_HARDENED_CLAY, 1, EnumDyeColor::PURPLE.getMetadata()),
                      ItemStack(Blocks::PURPLE_GLAZED_TERRACOTTA), 0.1F);
    addSmeltingRecipe(ItemStack(Blocks::STAINED_HARDENED_CLAY, 1, EnumDyeColor::BLUE.getMetadata()),
                      ItemStack(Blocks::BLUE_GLAZED_TERRACOTTA), 0.1F);
    addSmeltingRecipe(ItemStack(Blocks::STAINED_HARDENED_CLAY, 1, EnumDyeColor::BROWN.getMetadata()),
                      ItemStack(Blocks::BROWN_GLAZED_TERRACOTTA), 0.1F);
    addSmeltingRecipe(ItemStack(Blocks::STAINED_HARDENED_CLAY, 1, EnumDyeColor::GREEN.getMetadata()),
                      ItemStack(Blocks::GREEN_GLAZED_TERRACOTTA), 0.1F);
    addSmeltingRecipe(ItemStack(Blocks::STAINED_HARDENED_CLAY, 1, EnumDyeColor::RED.getMetadata()),
                      ItemStack(Blocks::RED_GLAZED_TERRACOTTA), 0.1F);
    addSmeltingRecipe(ItemStack(Blocks::STAINED_HARDENED_CLAY, 1, EnumDyeColor::BLACK.getMetadata()),
                      ItemStack(Blocks::BLACK_GLAZED_TERRACOTTA), 0.1F);
}

bool FurnaceRecipes::compareItemStacks(ItemStack stack1, ItemStack stack2)
{
    return stack2.getItem() == stack1.getItem() &&
           (stack2.getMetadata() == 32767 || stack2.getMetadata() == stack1.getMetadata());
}
