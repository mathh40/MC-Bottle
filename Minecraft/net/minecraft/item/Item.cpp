#include "Item.h"
#include "EnumFacing.h"
#include "EnumHandSide.h"
#include "ItemAir.h"
#include "ItemBlock.h"
#include "ItemStack.h"
#include "ResourceLocation.h"
#include "Util.h"
#include "math/BlockPos.h"
#include "math/MathHelper.h"
#include "text/translation/I18n.h"

xg::Guid Item::ATTACK_DAMAGE_MODIFIER = xg::Guid("CB3F55D3-645C-4F38-A497-9C13A33DB5CF");
xg::Guid Item::ATTACK_SPEED_MODIFIER = xg::Guid("FA233E1C-4180-4865-B01B-BCCE9785ACA3");

IItemPropertyGetter Item::DAMAGED_GETTER = [](ItemStack stack, World *worldIn, EntityLivingBase *entityIn)->float
{
    return stack.isItemDamaged() ? 1.0F : 0.0F;
};
IItemPropertyGetter Item::DAMAGE_GETTER = [](ItemStack stack, World *worldIn, EntityLivingBase *entityIn)->float
{
    return MathHelper::clamp((float)stack.getItemDamage() / (float)stack.getMaxDamage(), 0.0F, 1.0F);
};
IItemPropertyGetter Item::LEFTHANDED_GETTER = [](ItemStack stack, World *worldIn, EntityLivingBase *entityIn)->float
{
    return entityIn != nullptr && entityIn->getPrimaryHand() != EnumHandSide::RIGHT ? 1.0F : 0.0F;
};
IItemPropertyGetter Item::COOLDOWN_GETTER = [](ItemStack stack, World *worldIn, EntityLivingBase *entityIn)->float
{
    return Util::instanceof<EntityPlayer>(entityIn) ? ((EntityPlayer)entityIn)->getCooldownTracker().getCooldown(stack.getItem(), 0.0F) : 0.0F;
};

Item::Item()
{
    addPropertyOverride(ResourceLocation("lefthanded"), LEFTHANDED_GETTER);
    addPropertyOverride(ResourceLocation("cooldown"), COOLDOWN_GETTER);
}

Item* Item::setMaxStackSize(int32_t maxStackSize)
{
    maxStackSize = maxStackSize;
    return this;
}

EnumActionResult Item::onItemUse(EntityPlayer *player, World *worldIn, BlockPos pos, EnumHand hand, EnumFacing facing,
    float hitX, float hitY, float hitZ)
{
    return EnumActionResult::PASS;
}

float Item::getDestroySpeed(ItemStack stack, IBlockState *state)
{
    return 1.0F;
}

ActionResult Item::onItemRightClick(World *worldIn, EntityPlayer *playerIn, EnumHand handIn)
{
    return ActionResult(EnumActionResult::PASS, playerIn->getHeldItem(handIn));
}

ItemStack Item::onItemUseFinish(ItemStack stack, World *worldIn, EntityLivingBase *entityLiving)
{
    return stack;
}

int32_t Item::getItemStackLimit() const
{
    return maxStackSize;
}

int32_t Item::getMetadata(int32_t damage) const
{
    return 0;
}

bool Item::getHasSubtypes() const
{
    return hasSubtypes;
}

int32_t Item::getMaxDamage() const
{
    return maxDamage;
}

bool Item::isDamageable() const
{
    return maxDamage > 0 && (!hasSubtypes || maxStackSize == 1);
}

bool Item::hitEntity(ItemStack stack, EntityLivingBase *target, EntityLivingBase *attacker)
{
    return false;
}

bool Item::onBlockDestroyed(ItemStack stack, World *worldIn, IBlockState *state, BlockPos pos,
    EntityLivingBase *entityLiving)
{
    return false;
}

bool Item::canHarvestBlock(IBlockState *blockIn)
{
    return false;
}

bool Item::itemInteractionForEntity(ItemStack stack, EntityPlayer *playerIn, EntityLivingBase *target, EnumHand hand)
{
    return false;
}

Item* Item::setFull3D()
{
    bFull3D = true;
    return this;
}

bool Item::isFull3D() const
{
    return bFull3D;
}

bool Item::shouldRotateAroundWhenRendering()
{
    return false;
}

Item * Item::setTranslationKey(std::string_view key)
{
    translationKey = key;
    return this;
}

std::string Item::getUnlocalizedNameInefficiently(ItemStack stack) const
{
    return I18n::translateToLocal(getTranslationKey(stack));
}

std::string Item::getTranslationKey() const
{
    return "item." + translationKey;
}

std::string Item::getTranslationKey(ItemStack stack) const
{
    return "item." + translationKey;
}

Item * Item::setContainerItem(Item *containerItem)
{
    containerItem = containerItem;
    return this;
}

bool Item::getShareTag()
{
    return true;
}

Item * Item::getContainerItem() const
{
    return containerItem;
}

bool Item::hasContainerItem() const
{
    return containerItem != nullptr;
}

void Item::onUpdate(ItemStack stack, World *worldIn, Entity *entityIn, int32_t itemSlot, bool isSelected)
{
}

void Item::onCreated(ItemStack stack, World *worldIn, EntityPlayer *playerIn)
{

}

bool Item::isMap()
{
    return false;
}

EnumAction Item::getItemUseAction(ItemStack stack)
{
    return EnumAction::NONE;
}

int32_t Item::getMaxItemUseDuration(ItemStack stack)
{
    return 0;
}

void Item::onPlayerStoppedUsing(ItemStack stack, World *worldIn, EntityLivingBase *entityLiving, int32_t timeLeft)
{

}

void Item::addInformation(ItemStack stack, World* worldIn, std::vector<std::string>& tooltip, ITooltipFlag flagIn)
{
    
}

std::string Item::getItemStackDisplayName(ItemStack stack) const
{
    return I18n::translateToLocal(getUnlocalizedNameInefficiently(stack) + ".name");
}

bool Item::hasEffect(ItemStack stack)
{
    return stack.isItemEnchanted();
}

EnumRarity Item::getRarity(ItemStack stack)
{
    return stack.isItemEnchanted() ? EnumRarity::RARE : EnumRarity::COMMON;
}

bool Item::isEnchantable(ItemStack stack) const
{
    return getItemStackLimit() == 1 && isDamageable();
}

int Item::getIdFromItem(Item *itemIn)
{
    return itemIn == nullptr ? 0 : REGISTRY.getIDForObject(itemIn);
}

Item* Item::getItemById(int32_t id)
{
    return REGISTRY.getObjectById(id).value();
}

Item * Item::getItemFromBlock(Block *blockIn)
{
    Item* item = BLOCK_TO_ITEM[blockIn];
    return item == nullptr ? Items::AIR : item;
}

Item * Item::getByNameOrId(std::string_view id)
{
    Item* item = REGISTRY.getObject(ResourceLocation(id)).value();
    if (item == nullptr) 
    {
        return getItemById(std::atoi(id.data()));
    }

    return item;
}

void Item::addPropertyOverride(ResourceLocation key, IItemPropertyGetter getter)
{
    properties->putObject(key, getter);
}

IItemPropertyGetter Item::getPropertyGetter(ResourceLocation key)
{
    return properties->getObject(key);
}

bool Item::hasCustomProperties()
{
    return !properties->getKeys().isEmpty();
}

bool Item::updateItemStackNBT(NBTTagCompound *nbt)
{
    return false;
}

int32_t Item::getItemEnchantability()
{
    return 0;
}

void Item::getSubItems(const CreativeTabs& tab, std::vector<ItemStack>& items)
{
    if (isInCreativeTab(tab)) 
    {
        items.push_back(ItemStack(this));
    }
}

bool Item::isInCreativeTab(const CreativeTabs& targetTab)
{
    auto creativetabs = getCreativeTab();
    return (targetTab == CreativeTabs::SEARCH || targetTab == creativetabs);
}

void Item::registerItemBlock(Block *blockIn, Item *itemIn)
{
    registerItem(Block::getIdFromBlock(blockIn), (ResourceLocation)Block::REGISTRY::getNameForObject(blockIn), itemIn);
    BLOCK_TO_ITEM.emplace(blockIn, itemIn);
}

void Item::registerItemBlock(Block *blockIn)
{
    registerItemBlock(blockIn, ItemBlock(blockIn));
}

void Item::registerItem(int32_t id, std::string textualID, Item *itemIn)
{
    registerItem(id, ResourceLocation(textualID), itemIn);
}

void Item::registerItem(int32_t id, ResourceLocation textualID, Item *itemIn)
{
    REGISTRY.registe(id, textualID, itemIn);
}

CreativeTabs& Item::getCreativeTab()
{
    return tabToDisplayOn;
}

Item* Item::setCreativeTab(CreativeTabs& tab)
{
    tabToDisplayOn = tab;
    return this;
}

bool Item::canItemEditBlocks()
{
    return false;
}

bool Item::getIsRepairable(ItemStack toRepair, ItemStack repair)
{
    return false;
}

std::unordered_multimap<std::string,AttributeModifier> Item::getItemAttributeModifiers(EntityEquipmentSlot equipmentSlot)
{
    return std::unordered_multimap<std::string,AttributeModifier>();
}

ItemStack Item::getDefaultInstance()
{
    return ItemStack(this);
}

void Item::registerItems()
{
    registerItemBlock(Blocks.AIR, new ItemAir(Blocks.AIR));
      registerItemBlock(Blocks.STONE, (new ItemMultiTexture(Blocks.STONE, Blocks.STONE, new ItemMultiTexture.Mapper() {
         public String apply(ItemStack p_apply_1_) {
            return BlockStone.EnumType.byMetadata(p_apply_1_.getMetadata()).getTranslationKey();
         }
      })).setTranslationKey("stone"));
      registerItemBlock(Blocks.GRASS, new ItemColored(Blocks.GRASS, false));
      registerItemBlock(Blocks.DIRT, (new ItemMultiTexture(Blocks.DIRT, Blocks.DIRT, new ItemMultiTexture.Mapper() {
         public String apply(ItemStack p_apply_1_) {
            return BlockDirt.DirtType.byMetadata(p_apply_1_.getMetadata()).getTranslationKey();
         }
      })).setTranslationKey("dirt"));
      registerItemBlock(Blocks.COBBLESTONE);
      registerItemBlock(Blocks.PLANKS, (new ItemMultiTexture(Blocks.PLANKS, Blocks.PLANKS, new ItemMultiTexture.Mapper() {
         public String apply(ItemStack p_apply_1_) {
            return BlockPlanks.EnumType.byMetadata(p_apply_1_.getMetadata()).getTranslationKey();
         }
      })).setTranslationKey("wood"));
      registerItemBlock(Blocks.SAPLING, (new ItemMultiTexture(Blocks.SAPLING, Blocks.SAPLING, new ItemMultiTexture.Mapper() {
         public String apply(ItemStack p_apply_1_) {
            return BlockPlanks.EnumType.byMetadata(p_apply_1_.getMetadata()).getTranslationKey();
         }
      })).setTranslationKey("sapling"));
      registerItemBlock(Blocks.BEDROCK);
      registerItemBlock(Blocks.SAND, (new ItemMultiTexture(Blocks.SAND, Blocks.SAND, new ItemMultiTexture.Mapper() {
         public String apply(ItemStack p_apply_1_) {
            return BlockSand.EnumType.byMetadata(p_apply_1_.getMetadata()).getTranslationKey();
         }
      })).setTranslationKey("sand"));
      registerItemBlock(Blocks.GRAVEL);
      registerItemBlock(Blocks.GOLD_ORE);
      registerItemBlock(Blocks.IRON_ORE);
      registerItemBlock(Blocks.COAL_ORE);
      registerItemBlock(Blocks.LOG, (new ItemMultiTexture(Blocks.LOG, Blocks.LOG, new ItemMultiTexture.Mapper() {
         public String apply(ItemStack p_apply_1_) {
            return BlockPlanks.EnumType.byMetadata(p_apply_1_.getMetadata()).getTranslationKey();
         }
      })).setTranslationKey("log"));
      registerItemBlock(Blocks.LOG2, (new ItemMultiTexture(Blocks.LOG2, Blocks.LOG2, new ItemMultiTexture.Mapper() {
         public String apply(ItemStack p_apply_1_) {
            return BlockPlanks.EnumType.byMetadata(p_apply_1_.getMetadata() + 4).getTranslationKey();
         }
      })).setTranslationKey("log"));
      registerItemBlock(Blocks.LEAVES, (new ItemLeaves(Blocks.LEAVES)).setTranslationKey("leaves"));
      registerItemBlock(Blocks.LEAVES2, (new ItemLeaves(Blocks.LEAVES2)).setTranslationKey("leaves"));
      registerItemBlock(Blocks.SPONGE, (new ItemMultiTexture(Blocks.SPONGE, Blocks.SPONGE, new ItemMultiTexture.Mapper() {
         public String apply(ItemStack p_apply_1_) {
            return (p_apply_1_.getMetadata() & 1) == 1 ? "wet" : "dry";
         }
      })).setTranslationKey("sponge"));
      registerItemBlock(Blocks.GLASS);
      registerItemBlock(Blocks.LAPIS_ORE);
      registerItemBlock(Blocks.LAPIS_BLOCK);
      registerItemBlock(Blocks.DISPENSER);
      registerItemBlock(Blocks.SANDSTONE, (new ItemMultiTexture(Blocks.SANDSTONE, Blocks.SANDSTONE, new ItemMultiTexture.Mapper() {
         public String apply(ItemStack p_apply_1_) {
            return BlockSandStone.EnumType.byMetadata(p_apply_1_.getMetadata()).getTranslationKey();
         }
      })).setTranslationKey("sandStone"));
      registerItemBlock(Blocks.NOTEBLOCK);
      registerItemBlock(Blocks.GOLDEN_RAIL);
      registerItemBlock(Blocks.DETECTOR_RAIL);
      registerItemBlock(Blocks.STICKY_PISTON, new ItemPiston(Blocks.STICKY_PISTON));
      registerItemBlock(Blocks.WEB);
      registerItemBlock(Blocks.TALLGRASS, (new ItemColored(Blocks.TALLGRASS, true)).setSubtypeNames(new String[]{"shrub", "grass", "fern"}));
      registerItemBlock(Blocks.DEADBUSH);
      registerItemBlock(Blocks.PISTON, new ItemPiston(Blocks.PISTON));
      registerItemBlock(Blocks.WOOL, (new ItemCloth(Blocks.WOOL)).setTranslationKey("cloth"));
      registerItemBlock(Blocks.YELLOW_FLOWER, (new ItemMultiTexture(Blocks.YELLOW_FLOWER, Blocks.YELLOW_FLOWER, new ItemMultiTexture.Mapper() {
         public String apply(ItemStack p_apply_1_) {
            return BlockFlower.EnumFlowerType.getType(BlockFlower.EnumFlowerColor.YELLOW, p_apply_1_.getMetadata()).getTranslationKey();
         }
      })).setTranslationKey("flower"));
      registerItemBlock(Blocks.RED_FLOWER, (new ItemMultiTexture(Blocks.RED_FLOWER, Blocks.RED_FLOWER, new ItemMultiTexture.Mapper() {
         public String apply(ItemStack p_apply_1_) {
            return BlockFlower.EnumFlowerType.getType(BlockFlower.EnumFlowerColor.RED, p_apply_1_.getMetadata()).getTranslationKey();
         }
      })).setTranslationKey("rose"));
      registerItemBlock(Blocks.BROWN_MUSHROOM);
      registerItemBlock(Blocks.RED_MUSHROOM);
      registerItemBlock(Blocks.GOLD_BLOCK);
      registerItemBlock(Blocks.IRON_BLOCK);
      registerItemBlock(Blocks.STONE_SLAB, (new ItemSlab(Blocks.STONE_SLAB, Blocks.STONE_SLAB, Blocks.DOUBLE_STONE_SLAB)).setTranslationKey("stoneSlab"));
      registerItemBlock(Blocks.BRICK_BLOCK);
      registerItemBlock(Blocks.TNT);
      registerItemBlock(Blocks.BOOKSHELF);
      registerItemBlock(Blocks.MOSSY_COBBLESTONE);
      registerItemBlock(Blocks.OBSIDIAN);
      registerItemBlock(Blocks.TORCH);
      registerItemBlock(Blocks.END_ROD);
      registerItemBlock(Blocks.CHORUS_PLANT);
      registerItemBlock(Blocks.CHORUS_FLOWER);
      registerItemBlock(Blocks.PURPUR_BLOCK);
      registerItemBlock(Blocks.PURPUR_PILLAR);
      registerItemBlock(Blocks.PURPUR_STAIRS);
      registerItemBlock(Blocks.PURPUR_SLAB, (new ItemSlab(Blocks.PURPUR_SLAB, Blocks.PURPUR_SLAB, Blocks.PURPUR_DOUBLE_SLAB)).setTranslationKey("purpurSlab"));
      registerItemBlock(Blocks.MOB_SPAWNER);
      registerItemBlock(Blocks.OAK_STAIRS);
      registerItemBlock(Blocks.CHEST);
      registerItemBlock(Blocks.DIAMOND_ORE);
      registerItemBlock(Blocks.DIAMOND_BLOCK);
      registerItemBlock(Blocks.CRAFTING_TABLE);
      registerItemBlock(Blocks.FARMLAND);
      registerItemBlock(Blocks.FURNACE);
      registerItemBlock(Blocks.LADDER);
      registerItemBlock(Blocks.RAIL);
      registerItemBlock(Blocks.STONE_STAIRS);
      registerItemBlock(Blocks.LEVER);
      registerItemBlock(Blocks.STONE_PRESSURE_PLATE);
      registerItemBlock(Blocks.WOODEN_PRESSURE_PLATE);
      registerItemBlock(Blocks.REDSTONE_ORE);
      registerItemBlock(Blocks.REDSTONE_TORCH);
      registerItemBlock(Blocks.STONE_BUTTON);
      registerItemBlock(Blocks.SNOW_LAYER, new ItemSnow(Blocks.SNOW_LAYER));
      registerItemBlock(Blocks.ICE);
      registerItemBlock(Blocks.SNOW);
      registerItemBlock(Blocks.CACTUS);
      registerItemBlock(Blocks.CLAY);
      registerItemBlock(Blocks.JUKEBOX);
      registerItemBlock(Blocks.OAK_FENCE);
      registerItemBlock(Blocks.SPRUCE_FENCE);
      registerItemBlock(Blocks.BIRCH_FENCE);
      registerItemBlock(Blocks.JUNGLE_FENCE);
      registerItemBlock(Blocks.DARK_OAK_FENCE);
      registerItemBlock(Blocks.ACACIA_FENCE);
      registerItemBlock(Blocks.PUMPKIN);
      registerItemBlock(Blocks.NETHERRACK);
      registerItemBlock(Blocks.SOUL_SAND);
      registerItemBlock(Blocks.GLOWSTONE);
      registerItemBlock(Blocks.LIT_PUMPKIN);
      registerItemBlock(Blocks.TRAPDOOR);
      registerItemBlock(Blocks.MONSTER_EGG, (new ItemMultiTexture(Blocks.MONSTER_EGG, Blocks.MONSTER_EGG, new ItemMultiTexture.Mapper() {
         public String apply(ItemStack p_apply_1_) {
            return BlockSilverfish.EnumType.byMetadata(p_apply_1_.getMetadata()).getTranslationKey();
         }
      })).setTranslationKey("monsterStoneEgg"));
      registerItemBlock(Blocks.STONEBRICK, (new ItemMultiTexture(Blocks.STONEBRICK, Blocks.STONEBRICK, new ItemMultiTexture.Mapper() {
         public String apply(ItemStack p_apply_1_) {
            return BlockStoneBrick.EnumType.byMetadata(p_apply_1_.getMetadata()).getTranslationKey();
         }
      })).setTranslationKey("stonebricksmooth"));
      registerItemBlock(Blocks.BROWN_MUSHROOM_BLOCK);
      registerItemBlock(Blocks.RED_MUSHROOM_BLOCK);
      registerItemBlock(Blocks.IRON_BARS);
      registerItemBlock(Blocks.GLASS_PANE);
      registerItemBlock(Blocks.MELON_BLOCK);
      registerItemBlock(Blocks.VINE, new ItemColored(Blocks.VINE, false));
      registerItemBlock(Blocks.OAK_FENCE_GATE);
      registerItemBlock(Blocks.SPRUCE_FENCE_GATE);
      registerItemBlock(Blocks.BIRCH_FENCE_GATE);
      registerItemBlock(Blocks.JUNGLE_FENCE_GATE);
      registerItemBlock(Blocks.DARK_OAK_FENCE_GATE);
      registerItemBlock(Blocks.ACACIA_FENCE_GATE);
      registerItemBlock(Blocks.BRICK_STAIRS);
      registerItemBlock(Blocks.STONE_BRICK_STAIRS);
      registerItemBlock(Blocks.MYCELIUM);
      registerItemBlock(Blocks.WATERLILY, new ItemLilyPad(Blocks.WATERLILY));
      registerItemBlock(Blocks.NETHER_BRICK);
      registerItemBlock(Blocks.NETHER_BRICK_FENCE);
      registerItemBlock(Blocks.NETHER_BRICK_STAIRS);
      registerItemBlock(Blocks.ENCHANTING_TABLE);
      registerItemBlock(Blocks.END_PORTAL_FRAME);
      registerItemBlock(Blocks.END_STONE);
      registerItemBlock(Blocks.END_BRICKS);
      registerItemBlock(Blocks.DRAGON_EGG);
      registerItemBlock(Blocks.REDSTONE_LAMP);
      registerItemBlock(Blocks.WOODEN_SLAB, (new ItemSlab(Blocks.WOODEN_SLAB, Blocks.WOODEN_SLAB, Blocks.DOUBLE_WOODEN_SLAB)).setTranslationKey("woodSlab"));
      registerItemBlock(Blocks.SANDSTONE_STAIRS);
      registerItemBlock(Blocks.EMERALD_ORE);
      registerItemBlock(Blocks.ENDER_CHEST);
      registerItemBlock(Blocks.TRIPWIRE_HOOK);
      registerItemBlock(Blocks.EMERALD_BLOCK);
      registerItemBlock(Blocks.SPRUCE_STAIRS);
      registerItemBlock(Blocks.BIRCH_STAIRS);
      registerItemBlock(Blocks.JUNGLE_STAIRS);
      registerItemBlock(Blocks.COMMAND_BLOCK);
      registerItemBlock(Blocks.BEACON);
      registerItemBlock(Blocks.COBBLESTONE_WALL, (new ItemMultiTexture(Blocks.COBBLESTONE_WALL, Blocks.COBBLESTONE_WALL, new ItemMultiTexture.Mapper() {
         public String apply(ItemStack p_apply_1_) {
            return BlockWall.EnumType.byMetadata(p_apply_1_.getMetadata()).getTranslationKey();
         }
      })).setTranslationKey("cobbleWall"));
      registerItemBlock(Blocks.WOODEN_BUTTON);
      registerItemBlock(Blocks.ANVIL, (new ItemAnvilBlock(Blocks.ANVIL)).setTranslationKey("anvil"));
      registerItemBlock(Blocks.TRAPPED_CHEST);
      registerItemBlock(Blocks.LIGHT_WEIGHTED_PRESSURE_PLATE);
      registerItemBlock(Blocks.HEAVY_WEIGHTED_PRESSURE_PLATE);
      registerItemBlock(Blocks.DAYLIGHT_DETECTOR);
      registerItemBlock(Blocks.REDSTONE_BLOCK);
      registerItemBlock(Blocks.QUARTZ_ORE);
      registerItemBlock(Blocks.HOPPER);
      registerItemBlock(Blocks.QUARTZ_BLOCK, (new ItemMultiTexture(Blocks.QUARTZ_BLOCK, Blocks.QUARTZ_BLOCK, new String[]{"default", "chiseled", "lines"})).setTranslationKey("quartzBlock"));
      registerItemBlock(Blocks.QUARTZ_STAIRS);
      registerItemBlock(Blocks.ACTIVATOR_RAIL);
      registerItemBlock(Blocks.DROPPER);
      registerItemBlock(Blocks.STAINED_HARDENED_CLAY, (new ItemCloth(Blocks.STAINED_HARDENED_CLAY)).setTranslationKey("clayHardenedStained"));
      registerItemBlock(Blocks.BARRIER);
      registerItemBlock(Blocks.IRON_TRAPDOOR);
      registerItemBlock(Blocks.HAY_BLOCK);
      registerItemBlock(Blocks.CARPET, (new ItemCloth(Blocks.CARPET)).setTranslationKey("woolCarpet"));
      registerItemBlock(Blocks.HARDENED_CLAY);
      registerItemBlock(Blocks.COAL_BLOCK);
      registerItemBlock(Blocks.PACKED_ICE);
      registerItemBlock(Blocks.ACACIA_STAIRS);
      registerItemBlock(Blocks.DARK_OAK_STAIRS);
      registerItemBlock(Blocks.SLIME_BLOCK);
      registerItemBlock(Blocks.GRASS_PATH);
      registerItemBlock(Blocks.DOUBLE_PLANT, (new ItemMultiTexture(Blocks.DOUBLE_PLANT, Blocks.DOUBLE_PLANT, new ItemMultiTexture.Mapper() {
         public String apply(ItemStack p_apply_1_) {
            return BlockDoublePlant.EnumPlantType.byMetadata(p_apply_1_.getMetadata()).getTranslationKey();
         }
      })).setTranslationKey("doublePlant"));
      registerItemBlock(Blocks.STAINED_GLASS, (new ItemCloth(Blocks.STAINED_GLASS)).setTranslationKey("stainedGlass"));
      registerItemBlock(Blocks.STAINED_GLASS_PANE, (new ItemCloth(Blocks.STAINED_GLASS_PANE)).setTranslationKey("stainedGlassPane"));
      registerItemBlock(Blocks.PRISMARINE, (new ItemMultiTexture(Blocks.PRISMARINE, Blocks.PRISMARINE, new ItemMultiTexture.Mapper() {
         public String apply(ItemStack p_apply_1_) {
            return BlockPrismarine.EnumType.byMetadata(p_apply_1_.getMetadata()).getTranslationKey();
         }
      })).setTranslationKey("prismarine"));
      registerItemBlock(Blocks.SEA_LANTERN);
      registerItemBlock(Blocks.RED_SANDSTONE, (new ItemMultiTexture(Blocks.RED_SANDSTONE, Blocks.RED_SANDSTONE, new ItemMultiTexture.Mapper() {
         public String apply(ItemStack p_apply_1_) {
            return BlockRedSandstone.EnumType.byMetadata(p_apply_1_.getMetadata()).getTranslationKey();
         }
      })).setTranslationKey("redSandStone"));
      registerItemBlock(Blocks.RED_SANDSTONE_STAIRS);
      registerItemBlock(Blocks.STONE_SLAB2, (new ItemSlab(Blocks.STONE_SLAB2, Blocks.STONE_SLAB2, Blocks.DOUBLE_STONE_SLAB2)).setTranslationKey("stoneSlab2"));
      registerItemBlock(Blocks.REPEATING_COMMAND_BLOCK);
      registerItemBlock(Blocks.CHAIN_COMMAND_BLOCK);
      registerItemBlock(Blocks.MAGMA);
      registerItemBlock(Blocks.NETHER_WART_BLOCK);
      registerItemBlock(Blocks.RED_NETHER_BRICK);
      registerItemBlock(Blocks.BONE_BLOCK);
      registerItemBlock(Blocks.STRUCTURE_VOID);
      registerItemBlock(Blocks.OBSERVER);
      registerItemBlock(Blocks.WHITE_SHULKER_BOX, new ItemShulkerBox(Blocks.WHITE_SHULKER_BOX));
      registerItemBlock(Blocks.ORANGE_SHULKER_BOX, new ItemShulkerBox(Blocks.ORANGE_SHULKER_BOX));
      registerItemBlock(Blocks.MAGENTA_SHULKER_BOX, new ItemShulkerBox(Blocks.MAGENTA_SHULKER_BOX));
      registerItemBlock(Blocks.LIGHT_BLUE_SHULKER_BOX, new ItemShulkerBox(Blocks.LIGHT_BLUE_SHULKER_BOX));
      registerItemBlock(Blocks.YELLOW_SHULKER_BOX, new ItemShulkerBox(Blocks.YELLOW_SHULKER_BOX));
      registerItemBlock(Blocks.LIME_SHULKER_BOX, new ItemShulkerBox(Blocks.LIME_SHULKER_BOX));
      registerItemBlock(Blocks.PINK_SHULKER_BOX, new ItemShulkerBox(Blocks.PINK_SHULKER_BOX));
      registerItemBlock(Blocks.GRAY_SHULKER_BOX, new ItemShulkerBox(Blocks.GRAY_SHULKER_BOX));
      registerItemBlock(Blocks.SILVER_SHULKER_BOX, new ItemShulkerBox(Blocks.SILVER_SHULKER_BOX));
      registerItemBlock(Blocks.CYAN_SHULKER_BOX, new ItemShulkerBox(Blocks.CYAN_SHULKER_BOX));
      registerItemBlock(Blocks.PURPLE_SHULKER_BOX, new ItemShulkerBox(Blocks.PURPLE_SHULKER_BOX));
      registerItemBlock(Blocks.BLUE_SHULKER_BOX, new ItemShulkerBox(Blocks.BLUE_SHULKER_BOX));
      registerItemBlock(Blocks.BROWN_SHULKER_BOX, new ItemShulkerBox(Blocks.BROWN_SHULKER_BOX));
      registerItemBlock(Blocks.GREEN_SHULKER_BOX, new ItemShulkerBox(Blocks.GREEN_SHULKER_BOX));
      registerItemBlock(Blocks.RED_SHULKER_BOX, new ItemShulkerBox(Blocks.RED_SHULKER_BOX));
      registerItemBlock(Blocks.BLACK_SHULKER_BOX, new ItemShulkerBox(Blocks.BLACK_SHULKER_BOX));
      registerItemBlock(Blocks.WHITE_GLAZED_TERRACOTTA);
      registerItemBlock(Blocks.ORANGE_GLAZED_TERRACOTTA);
      registerItemBlock(Blocks.MAGENTA_GLAZED_TERRACOTTA);
      registerItemBlock(Blocks.LIGHT_BLUE_GLAZED_TERRACOTTA);
      registerItemBlock(Blocks.YELLOW_GLAZED_TERRACOTTA);
      registerItemBlock(Blocks.LIME_GLAZED_TERRACOTTA);
      registerItemBlock(Blocks.PINK_GLAZED_TERRACOTTA);
      registerItemBlock(Blocks.GRAY_GLAZED_TERRACOTTA);
      registerItemBlock(Blocks.SILVER_GLAZED_TERRACOTTA);
      registerItemBlock(Blocks.CYAN_GLAZED_TERRACOTTA);
      registerItemBlock(Blocks.PURPLE_GLAZED_TERRACOTTA);
      registerItemBlock(Blocks.BLUE_GLAZED_TERRACOTTA);
      registerItemBlock(Blocks.BROWN_GLAZED_TERRACOTTA);
      registerItemBlock(Blocks.GREEN_GLAZED_TERRACOTTA);
      registerItemBlock(Blocks.RED_GLAZED_TERRACOTTA);
      registerItemBlock(Blocks.BLACK_GLAZED_TERRACOTTA);
      registerItemBlock(Blocks.CONCRETE, (new ItemCloth(Blocks.CONCRETE)).setTranslationKey("concrete"));
      registerItemBlock(Blocks.CONCRETE_POWDER, (new ItemCloth(Blocks.CONCRETE_POWDER)).setTranslationKey("concrete_powder"));
      registerItemBlock(Blocks.STRUCTURE_BLOCK);
      registerItem(256, (String)"iron_shovel", (new ItemSpade(Item.ToolMaterial.IRON)).setTranslationKey("shovelIron"));
      registerItem(257, (String)"iron_pickaxe", (new ItemPickaxe(Item.ToolMaterial.IRON)).setTranslationKey("pickaxeIron"));
      registerItem(258, (String)"iron_axe", (new ItemAxe(Item.ToolMaterial.IRON)).setTranslationKey("hatchetIron"));
      registerItem(259, (String)"flint_and_steel", (new ItemFlintAndSteel()).setTranslationKey("flintAndSteel"));
      registerItem(260, (String)"apple", (new ItemFood(4, 0.3F, false)).setTranslationKey("apple"));
      registerItem(261, (String)"bow", (new ItemBow()).setTranslationKey("bow"));
      registerItem(262, (String)"arrow", (new ItemArrow()).setTranslationKey("arrow"));
      registerItem(263, (String)"coal", (new ItemCoal()).setTranslationKey("coal"));
      registerItem(264, (String)"diamond", (new Item()).setTranslationKey("diamond").setCreativeTab(CreativeTabs.MATERIALS));
      registerItem(265, (String)"iron_ingot", (new Item()).setTranslationKey("ingotIron").setCreativeTab(CreativeTabs.MATERIALS));
      registerItem(266, (String)"gold_ingot", (new Item()).setTranslationKey("ingotGold").setCreativeTab(CreativeTabs.MATERIALS));
      registerItem(267, (String)"iron_sword", (new ItemSword(Item.ToolMaterial.IRON)).setTranslationKey("swordIron"));
      registerItem(268, (String)"wooden_sword", (new ItemSword(Item.ToolMaterial.WOOD)).setTranslationKey("swordWood"));
      registerItem(269, (String)"wooden_shovel", (new ItemSpade(Item.ToolMaterial.WOOD)).setTranslationKey("shovelWood"));
      registerItem(270, (String)"wooden_pickaxe", (new ItemPickaxe(Item.ToolMaterial.WOOD)).setTranslationKey("pickaxeWood"));
      registerItem(271, (String)"wooden_axe", (new ItemAxe(Item.ToolMaterial.WOOD)).setTranslationKey("hatchetWood"));
      registerItem(272, (String)"stone_sword", (new ItemSword(Item.ToolMaterial.STONE)).setTranslationKey("swordStone"));
      registerItem(273, (String)"stone_shovel", (new ItemSpade(Item.ToolMaterial.STONE)).setTranslationKey("shovelStone"));
      registerItem(274, (String)"stone_pickaxe", (new ItemPickaxe(Item.ToolMaterial.STONE)).setTranslationKey("pickaxeStone"));
      registerItem(275, (String)"stone_axe", (new ItemAxe(Item.ToolMaterial.STONE)).setTranslationKey("hatchetStone"));
      registerItem(276, (String)"diamond_sword", (new ItemSword(Item.ToolMaterial.DIAMOND)).setTranslationKey("swordDiamond"));
      registerItem(277, (String)"diamond_shovel", (new ItemSpade(Item.ToolMaterial.DIAMOND)).setTranslationKey("shovelDiamond"));
      registerItem(278, (String)"diamond_pickaxe", (new ItemPickaxe(Item.ToolMaterial.DIAMOND)).setTranslationKey("pickaxeDiamond"));
      registerItem(279, (String)"diamond_axe", (new ItemAxe(Item.ToolMaterial.DIAMOND)).setTranslationKey("hatchetDiamond"));
      registerItem(280, (String)"stick", (new Item()).setFull3D().setTranslationKey("stick").setCreativeTab(CreativeTabs.MATERIALS));
      registerItem(281, (String)"bowl", (new Item()).setTranslationKey("bowl").setCreativeTab(CreativeTabs.MATERIALS));
      registerItem(282, (String)"mushroom_stew", (new ItemSoup(6)).setTranslationKey("mushroomStew"));
      registerItem(283, (String)"golden_sword", (new ItemSword(Item.ToolMaterial.GOLD)).setTranslationKey("swordGold"));
      registerItem(284, (String)"golden_shovel", (new ItemSpade(Item.ToolMaterial.GOLD)).setTranslationKey("shovelGold"));
      registerItem(285, (String)"golden_pickaxe", (new ItemPickaxe(Item.ToolMaterial.GOLD)).setTranslationKey("pickaxeGold"));
      registerItem(286, (String)"golden_axe", (new ItemAxe(Item.ToolMaterial.GOLD)).setTranslationKey("hatchetGold"));
      registerItem(287, (String)"string", (new ItemBlockSpecial(Blocks.TRIPWIRE)).setTranslationKey("string").setCreativeTab(CreativeTabs.MATERIALS));
      registerItem(288, (String)"feather", (new Item()).setTranslationKey("feather").setCreativeTab(CreativeTabs.MATERIALS));
      registerItem(289, (String)"gunpowder", (new Item()).setTranslationKey("sulphur").setCreativeTab(CreativeTabs.MATERIALS));
      registerItem(290, (String)"wooden_hoe", (new ItemHoe(Item.ToolMaterial.WOOD)).setTranslationKey("hoeWood"));
      registerItem(291, (String)"stone_hoe", (new ItemHoe(Item.ToolMaterial.STONE)).setTranslationKey("hoeStone"));
      registerItem(292, (String)"iron_hoe", (new ItemHoe(Item.ToolMaterial.IRON)).setTranslationKey("hoeIron"));
      registerItem(293, (String)"diamond_hoe", (new ItemHoe(Item.ToolMaterial.DIAMOND)).setTranslationKey("hoeDiamond"));
      registerItem(294, (String)"golden_hoe", (new ItemHoe(Item.ToolMaterial.GOLD)).setTranslationKey("hoeGold"));
      registerItem(295, (String)"wheat_seeds", (new ItemSeeds(Blocks.WHEAT, Blocks.FARMLAND)).setTranslationKey("seeds"));
      registerItem(296, (String)"wheat", (new Item()).setTranslationKey("wheat").setCreativeTab(CreativeTabs.MATERIALS));
      registerItem(297, (String)"bread", (new ItemFood(5, 0.6F, false)).setTranslationKey("bread"));
      registerItem(298, (String)"leather_helmet", (new ItemArmor(ItemArmor.ArmorMaterial.LEATHER, 0, EntityEquipmentSlot.HEAD)).setTranslationKey("helmetCloth"));
      registerItem(299, (String)"leather_chestplate", (new ItemArmor(ItemArmor.ArmorMaterial.LEATHER, 0, EntityEquipmentSlot.CHEST)).setTranslationKey("chestplateCloth"));
      registerItem(300, (String)"leather_leggings", (new ItemArmor(ItemArmor.ArmorMaterial.LEATHER, 0, EntityEquipmentSlot.LEGS)).setTranslationKey("leggingsCloth"));
      registerItem(301, (String)"leather_boots", (new ItemArmor(ItemArmor.ArmorMaterial.LEATHER, 0, EntityEquipmentSlot.FEET)).setTranslationKey("bootsCloth"));
      registerItem(302, (String)"chainmail_helmet", (new ItemArmor(ItemArmor.ArmorMaterial.CHAIN, 1, EntityEquipmentSlot.HEAD)).setTranslationKey("helmetChain"));
      registerItem(303, (String)"chainmail_chestplate", (new ItemArmor(ItemArmor.ArmorMaterial.CHAIN, 1, EntityEquipmentSlot.CHEST)).setTranslationKey("chestplateChain"));
      registerItem(304, (String)"chainmail_leggings", (new ItemArmor(ItemArmor.ArmorMaterial.CHAIN, 1, EntityEquipmentSlot.LEGS)).setTranslationKey("leggingsChain"));
      registerItem(305, (String)"chainmail_boots", (new ItemArmor(ItemArmor.ArmorMaterial.CHAIN, 1, EntityEquipmentSlot.FEET)).setTranslationKey("bootsChain"));
      registerItem(306, (String)"iron_helmet", (new ItemArmor(ItemArmor.ArmorMaterial.IRON, 2, EntityEquipmentSlot.HEAD)).setTranslationKey("helmetIron"));
      registerItem(307, (String)"iron_chestplate", (new ItemArmor(ItemArmor.ArmorMaterial.IRON, 2, EntityEquipmentSlot.CHEST)).setTranslationKey("chestplateIron"));
      registerItem(308, (String)"iron_leggings", (new ItemArmor(ItemArmor.ArmorMaterial.IRON, 2, EntityEquipmentSlot.LEGS)).setTranslationKey("leggingsIron"));
      registerItem(309, (String)"iron_boots", (new ItemArmor(ItemArmor.ArmorMaterial.IRON, 2, EntityEquipmentSlot.FEET)).setTranslationKey("bootsIron"));
      registerItem(310, (String)"diamond_helmet", (new ItemArmor(ItemArmor.ArmorMaterial.DIAMOND, 3, EntityEquipmentSlot.HEAD)).setTranslationKey("helmetDiamond"));
      registerItem(311, (String)"diamond_chestplate", (new ItemArmor(ItemArmor.ArmorMaterial.DIAMOND, 3, EntityEquipmentSlot.CHEST)).setTranslationKey("chestplateDiamond"));
      registerItem(312, (String)"diamond_leggings", (new ItemArmor(ItemArmor.ArmorMaterial.DIAMOND, 3, EntityEquipmentSlot.LEGS)).setTranslationKey("leggingsDiamond"));
      registerItem(313, (String)"diamond_boots", (new ItemArmor(ItemArmor.ArmorMaterial.DIAMOND, 3, EntityEquipmentSlot.FEET)).setTranslationKey("bootsDiamond"));
      registerItem(314, (String)"golden_helmet", (new ItemArmor(ItemArmor.ArmorMaterial.GOLD, 4, EntityEquipmentSlot.HEAD)).setTranslationKey("helmetGold"));
      registerItem(315, (String)"golden_chestplate", (new ItemArmor(ItemArmor.ArmorMaterial.GOLD, 4, EntityEquipmentSlot.CHEST)).setTranslationKey("chestplateGold"));
      registerItem(316, (String)"golden_leggings", (new ItemArmor(ItemArmor.ArmorMaterial.GOLD, 4, EntityEquipmentSlot.LEGS)).setTranslationKey("leggingsGold"));
      registerItem(317, (String)"golden_boots", (new ItemArmor(ItemArmor.ArmorMaterial.GOLD, 4, EntityEquipmentSlot.FEET)).setTranslationKey("bootsGold"));
      registerItem(318, (String)"flint", (new Item()).setTranslationKey("flint").setCreativeTab(CreativeTabs.MATERIALS));
      registerItem(319, (String)"porkchop", (new ItemFood(3, 0.3F, true)).setTranslationKey("porkchopRaw"));
      registerItem(320, (String)"cooked_porkchop", (new ItemFood(8, 0.8F, true)).setTranslationKey("porkchopCooked"));
      registerItem(321, (String)"painting", (new ItemHangingEntity(EntityPainting.class)).setTranslationKey("painting"));
      registerItem(322, (String)"golden_apple", (new ItemAppleGold(4, 1.2F, false)).setAlwaysEdible().setTranslationKey("appleGold"));
      registerItem(323, (String)"sign", (new ItemSign()).setTranslationKey("sign"));
      registerItem(324, (String)"wooden_door", (new ItemDoor(Blocks.OAK_DOOR)).setTranslationKey("doorOak"));
      Item item = (new ItemBucket(Blocks.AIR)).setTranslationKey("bucket").setMaxStackSize(16);
      registerItem(325, (String)"bucket", item);
      registerItem(326, (String)"water_bucket", (new ItemBucket(Blocks.FLOWING_WATER)).setTranslationKey("bucketWater").setContainerItem(item));
      registerItem(327, (String)"lava_bucket", (new ItemBucket(Blocks.FLOWING_LAVA)).setTranslationKey("bucketLava").setContainerItem(item));
      registerItem(328, (String)"minecart", (new ItemMinecart(EntityMinecart.Type.RIDEABLE)).setTranslationKey("minecart"));
      registerItem(329, (String)"saddle", (new ItemSaddle()).setTranslationKey("saddle"));
      registerItem(330, (String)"iron_door", (new ItemDoor(Blocks.IRON_DOOR)).setTranslationKey("doorIron"));
      registerItem(331, (String)"redstone", (new ItemRedstone()).setTranslationKey("redstone"));
      registerItem(332, (String)"snowball", (new ItemSnowball()).setTranslationKey("snowball"));
      registerItem(333, (String)"boat", new ItemBoat(EntityBoat.Type.OAK));
      registerItem(334, (String)"leather", (new Item()).setTranslationKey("leather").setCreativeTab(CreativeTabs.MATERIALS));
      registerItem(335, (String)"milk_bucket", (new ItemBucketMilk()).setTranslationKey("milk").setContainerItem(item));
      registerItem(336, (String)"brick", (new Item()).setTranslationKey("brick").setCreativeTab(CreativeTabs.MATERIALS));
      registerItem(337, (String)"clay_ball", (new Item()).setTranslationKey("clay").setCreativeTab(CreativeTabs.MATERIALS));
      registerItem(338, (String)"reeds", (new ItemBlockSpecial(Blocks.REEDS)).setTranslationKey("reeds").setCreativeTab(CreativeTabs.MATERIALS));
      registerItem(339, (String)"paper", (new Item()).setTranslationKey("paper").setCreativeTab(CreativeTabs.MISC));
      registerItem(340, (String)"book", (new ItemBook()).setTranslationKey("book").setCreativeTab(CreativeTabs.MISC));
      registerItem(341, (String)"slime_ball", (new Item()).setTranslationKey("slimeball").setCreativeTab(CreativeTabs.MISC));
      registerItem(342, (String)"chest_minecart", (new ItemMinecart(EntityMinecart.Type.CHEST)).setTranslationKey("minecartChest"));
      registerItem(343, (String)"furnace_minecart", (new ItemMinecart(EntityMinecart.Type.FURNACE)).setTranslationKey("minecartFurnace"));
      registerItem(344, (String)"egg", (new ItemEgg()).setTranslationKey("egg"));
      registerItem(345, (String)"compass", (new ItemCompass()).setTranslationKey("compass").setCreativeTab(CreativeTabs.TOOLS));
      registerItem(346, (String)"fishing_rod", (new ItemFishingRod()).setTranslationKey("fishingRod"));
      registerItem(347, (String)"clock", (new ItemClock()).setTranslationKey("clock").setCreativeTab(CreativeTabs.TOOLS));
      registerItem(348, (String)"glowstone_dust", (new Item()).setTranslationKey("yellowDust").setCreativeTab(CreativeTabs.MATERIALS));
      registerItem(349, (String)"fish", (new ItemFishFood(false)).setTranslationKey("fish").setHasSubtypes(true));
      registerItem(350, (String)"cooked_fish", (new ItemFishFood(true)).setTranslationKey("fish").setHasSubtypes(true));
      registerItem(351, (String)"dye", (new ItemDye()).setTranslationKey("dyePowder"));
      registerItem(352, (String)"bone", (new Item()).setTranslationKey("bone").setFull3D().setCreativeTab(CreativeTabs.MISC));
      registerItem(353, (String)"sugar", (new Item()).setTranslationKey("sugar").setCreativeTab(CreativeTabs.MATERIALS));
      registerItem(354, (String)"cake", (new ItemBlockSpecial(Blocks.CAKE)).setMaxStackSize(1).setTranslationKey("cake").setCreativeTab(CreativeTabs.FOOD));
      registerItem(355, (String)"bed", (new ItemBed()).setMaxStackSize(1).setTranslationKey("bed"));
      registerItem(356, (String)"repeater", (new ItemBlockSpecial(Blocks.UNPOWERED_REPEATER)).setTranslationKey("diode").setCreativeTab(CreativeTabs.REDSTONE));
      registerItem(357, (String)"cookie", (new ItemFood(2, 0.1F, false)).setTranslationKey("cookie"));
      registerItem(358, (String)"filled_map", (new ItemMap()).setTranslationKey("map"));
      registerItem(359, (String)"shears", (new ItemShears()).setTranslationKey("shears"));
      registerItem(360, (String)"melon", (new ItemFood(2, 0.3F, false)).setTranslationKey("melon"));
      registerItem(361, (String)"pumpkin_seeds", (new ItemSeeds(Blocks.PUMPKIN_STEM, Blocks.FARMLAND)).setTranslationKey("seeds_pumpkin"));
      registerItem(362, (String)"melon_seeds", (new ItemSeeds(Blocks.MELON_STEM, Blocks.FARMLAND)).setTranslationKey("seeds_melon"));
      registerItem(363, (String)"beef", (new ItemFood(3, 0.3F, true)).setTranslationKey("beefRaw"));
      registerItem(364, (String)"cooked_beef", (new ItemFood(8, 0.8F, true)).setTranslationKey("beefCooked"));
      registerItem(365, (String)"chicken", (new ItemFood(2, 0.3F, true)).setPotionEffect(new PotionEffect(MobEffects.HUNGER, 600, 0), 0.3F).setTranslationKey("chickenRaw"));
      registerItem(366, (String)"cooked_chicken", (new ItemFood(6, 0.6F, true)).setTranslationKey("chickenCooked"));
      registerItem(367, (String)"rotten_flesh", (new ItemFood(4, 0.1F, true)).setPotionEffect(new PotionEffect(MobEffects.HUNGER, 600, 0), 0.8F).setTranslationKey("rottenFlesh"));
      registerItem(368, (String)"ender_pearl", (new ItemEnderPearl()).setTranslationKey("enderPearl"));
      registerItem(369, (String)"blaze_rod", (new Item()).setTranslationKey("blazeRod").setCreativeTab(CreativeTabs.MATERIALS).setFull3D());
      registerItem(370, (String)"ghast_tear", (new Item()).setTranslationKey("ghastTear").setCreativeTab(CreativeTabs.BREWING));
      registerItem(371, (String)"gold_nugget", (new Item()).setTranslationKey("goldNugget").setCreativeTab(CreativeTabs.MATERIALS));
      registerItem(372, (String)"nether_wart", (new ItemSeeds(Blocks.NETHER_WART, Blocks.SOUL_SAND)).setTranslationKey("netherStalkSeeds"));
      registerItem(373, (String)"potion", (new ItemPotion()).setTranslationKey("potion"));
      Item item1 = (new ItemGlassBottle()).setTranslationKey("glassBottle");
      registerItem(374, (String)"glass_bottle", item1);
      registerItem(375, (String)"spider_eye", (new ItemFood(2, 0.8F, false)).setPotionEffect(new PotionEffect(MobEffects.POISON, 100, 0), 1.0F).setTranslationKey("spiderEye"));
      registerItem(376, (String)"fermented_spider_eye", (new Item()).setTranslationKey("fermentedSpiderEye").setCreativeTab(CreativeTabs.BREWING));
      registerItem(377, (String)"blaze_powder", (new Item()).setTranslationKey("blazePowder").setCreativeTab(CreativeTabs.BREWING));
      registerItem(378, (String)"magma_cream", (new Item()).setTranslationKey("magmaCream").setCreativeTab(CreativeTabs.BREWING));
      registerItem(379, (String)"brewing_stand", (new ItemBlockSpecial(Blocks.BREWING_STAND)).setTranslationKey("brewingStand").setCreativeTab(CreativeTabs.BREWING));
      registerItem(380, (String)"cauldron", (new ItemBlockSpecial(Blocks.CAULDRON)).setTranslationKey("cauldron").setCreativeTab(CreativeTabs.BREWING));
      registerItem(381, (String)"ender_eye", (new ItemEnderEye()).setTranslationKey("eyeOfEnder"));
      registerItem(382, (String)"speckled_melon", (new Item()).setTranslationKey("speckledMelon").setCreativeTab(CreativeTabs.BREWING));
      registerItem(383, (String)"spawn_egg", (new ItemMonsterPlacer()).setTranslationKey("monsterPlacer"));
      registerItem(384, (String)"experience_bottle", (new ItemExpBottle()).setTranslationKey("expBottle"));
      registerItem(385, (String)"fire_charge", (new ItemFireball()).setTranslationKey("fireball"));
      registerItem(386, (String)"writable_book", (new ItemWritableBook()).setTranslationKey("writingBook").setCreativeTab(CreativeTabs.MISC));
      registerItem(387, (String)"written_book", (new ItemWrittenBook()).setTranslationKey("writtenBook").setMaxStackSize(16));
      registerItem(388, (String)"emerald", (new Item()).setTranslationKey("emerald").setCreativeTab(CreativeTabs.MATERIALS));
      registerItem(389, (String)"item_frame", (new ItemHangingEntity(EntityItemFrame.class)).setTranslationKey("frame"));
      registerItem(390, (String)"flower_pot", (new ItemBlockSpecial(Blocks.FLOWER_POT)).setTranslationKey("flowerPot").setCreativeTab(CreativeTabs.DECORATIONS));
      registerItem(391, (String)"carrot", (new ItemSeedFood(3, 0.6F, Blocks.CARROTS, Blocks.FARMLAND)).setTranslationKey("carrots"));
      registerItem(392, (String)"potato", (new ItemSeedFood(1, 0.3F, Blocks.POTATOES, Blocks.FARMLAND)).setTranslationKey("potato"));
      registerItem(393, (String)"baked_potato", (new ItemFood(5, 0.6F, false)).setTranslationKey("potatoBaked"));
      registerItem(394, (String)"poisonous_potato", (new ItemFood(2, 0.3F, false)).setPotionEffect(new PotionEffect(MobEffects.POISON, 100, 0), 0.6F).setTranslationKey("potatoPoisonous"));
      registerItem(395, (String)"map", (new ItemEmptyMap()).setTranslationKey("emptyMap"));
      registerItem(396, (String)"golden_carrot", (new ItemFood(6, 1.2F, false)).setTranslationKey("carrotGolden").setCreativeTab(CreativeTabs.BREWING));
      registerItem(397, (String)"skull", (new ItemSkull()).setTranslationKey("skull"));
      registerItem(398, (String)"carrot_on_a_stick", (new ItemCarrotOnAStick()).setTranslationKey("carrotOnAStick"));
      registerItem(399, (String)"nether_star", (new ItemSimpleFoiled()).setTranslationKey("netherStar").setCreativeTab(CreativeTabs.MATERIALS));
      registerItem(400, (String)"pumpkin_pie", (new ItemFood(8, 0.3F, false)).setTranslationKey("pumpkinPie").setCreativeTab(CreativeTabs.FOOD));
      registerItem(401, (String)"fireworks", (new ItemFirework()).setTranslationKey("fireworks"));
      registerItem(402, (String)"firework_charge", (new ItemFireworkCharge()).setTranslationKey("fireworksCharge").setCreativeTab(CreativeTabs.MISC));
      registerItem(403, (String)"enchanted_book", (new ItemEnchantedBook()).setMaxStackSize(1).setTranslationKey("enchantedBook"));
      registerItem(404, (String)"comparator", (new ItemBlockSpecial(Blocks.UNPOWERED_COMPARATOR)).setTranslationKey("comparator").setCreativeTab(CreativeTabs.REDSTONE));
      registerItem(405, (String)"netherbrick", (new Item()).setTranslationKey("netherbrick").setCreativeTab(CreativeTabs.MATERIALS));
      registerItem(406, (String)"quartz", (new Item()).setTranslationKey("netherquartz").setCreativeTab(CreativeTabs.MATERIALS));
      registerItem(407, (String)"tnt_minecart", (new ItemMinecart(EntityMinecart.Type.TNT)).setTranslationKey("minecartTnt"));
      registerItem(408, (String)"hopper_minecart", (new ItemMinecart(EntityMinecart.Type.HOPPER)).setTranslationKey("minecartHopper"));
      registerItem(409, (String)"prismarine_shard", (new Item()).setTranslationKey("prismarineShard").setCreativeTab(CreativeTabs.MATERIALS));
      registerItem(410, (String)"prismarine_crystals", (new Item()).setTranslationKey("prismarineCrystals").setCreativeTab(CreativeTabs.MATERIALS));
      registerItem(411, (String)"rabbit", (new ItemFood(3, 0.3F, true)).setTranslationKey("rabbitRaw"));
      registerItem(412, (String)"cooked_rabbit", (new ItemFood(5, 0.6F, true)).setTranslationKey("rabbitCooked"));
      registerItem(413, (String)"rabbit_stew", (new ItemSoup(10)).setTranslationKey("rabbitStew"));
      registerItem(414, (String)"rabbit_foot", (new Item()).setTranslationKey("rabbitFoot").setCreativeTab(CreativeTabs.BREWING));
      registerItem(415, (String)"rabbit_hide", (new Item()).setTranslationKey("rabbitHide").setCreativeTab(CreativeTabs.MATERIALS));
      registerItem(416, (String)"armor_stand", (new ItemArmorStand()).setTranslationKey("armorStand").setMaxStackSize(16));
      registerItem(417, (String)"iron_horse_armor", (new Item()).setTranslationKey("horsearmormetal").setMaxStackSize(1).setCreativeTab(CreativeTabs.MISC));
      registerItem(418, (String)"golden_horse_armor", (new Item()).setTranslationKey("horsearmorgold").setMaxStackSize(1).setCreativeTab(CreativeTabs.MISC));
      registerItem(419, (String)"diamond_horse_armor", (new Item()).setTranslationKey("horsearmordiamond").setMaxStackSize(1).setCreativeTab(CreativeTabs.MISC));
      registerItem(420, (String)"lead", (new ItemLead()).setTranslationKey("leash"));
      registerItem(421, (String)"name_tag", (new ItemNameTag()).setTranslationKey("nameTag"));
      registerItem(422, (String)"command_block_minecart", (new ItemMinecart(EntityMinecart.Type.COMMAND_BLOCK)).setTranslationKey("minecartCommandBlock").setCreativeTab((CreativeTabs)null));
      registerItem(423, (String)"mutton", (new ItemFood(2, 0.3F, true)).setTranslationKey("muttonRaw"));
      registerItem(424, (String)"cooked_mutton", (new ItemFood(6, 0.8F, true)).setTranslationKey("muttonCooked"));
      registerItem(425, (String)"banner", (new ItemBanner()).setTranslationKey("banner"));
      registerItem(426, (String)"end_crystal", new ItemEndCrystal());
      registerItem(427, (String)"spruce_door", (new ItemDoor(Blocks.SPRUCE_DOOR)).setTranslationKey("doorSpruce"));
      registerItem(428, (String)"birch_door", (new ItemDoor(Blocks.BIRCH_DOOR)).setTranslationKey("doorBirch"));
      registerItem(429, (String)"jungle_door", (new ItemDoor(Blocks.JUNGLE_DOOR)).setTranslationKey("doorJungle"));
      registerItem(430, (String)"acacia_door", (new ItemDoor(Blocks.ACACIA_DOOR)).setTranslationKey("doorAcacia"));
      registerItem(431, (String)"dark_oak_door", (new ItemDoor(Blocks.DARK_OAK_DOOR)).setTranslationKey("doorDarkOak"));
      registerItem(432, (String)"chorus_fruit", (new ItemChorusFruit(4, 0.3F)).setAlwaysEdible().setTranslationKey("chorusFruit").setCreativeTab(CreativeTabs.MATERIALS));
      registerItem(433, (String)"chorus_fruit_popped", (new Item()).setTranslationKey("chorusFruitPopped").setCreativeTab(CreativeTabs.MATERIALS));
      registerItem(434, (String)"beetroot", (new ItemFood(1, 0.6F, false)).setTranslationKey("beetroot"));
      registerItem(435, (String)"beetroot_seeds", (new ItemSeeds(Blocks.BEETROOTS, Blocks.FARMLAND)).setTranslationKey("beetroot_seeds"));
      registerItem(436, (String)"beetroot_soup", (new ItemSoup(6)).setTranslationKey("beetroot_soup"));
      registerItem(437, (String)"dragon_breath", (new Item()).setCreativeTab(CreativeTabs.BREWING).setTranslationKey("dragon_breath").setContainerItem(item1));
      registerItem(438, (String)"splash_potion", (new ItemSplashPotion()).setTranslationKey("splash_potion"));
      registerItem(439, (String)"spectral_arrow", (new ItemSpectralArrow()).setTranslationKey("spectral_arrow"));
      registerItem(440, (String)"tipped_arrow", (new ItemTippedArrow()).setTranslationKey("tipped_arrow"));
      registerItem(441, (String)"lingering_potion", (new ItemLingeringPotion()).setTranslationKey("lingering_potion"));
      registerItem(442, (String)"shield", (new ItemShield()).setTranslationKey("shield"));
      registerItem(443, (String)"elytra", (new ItemElytra()).setTranslationKey("elytra"));
      registerItem(444, (String)"spruce_boat", new ItemBoat(EntityBoat.Type.SPRUCE));
      registerItem(445, (String)"birch_boat", new ItemBoat(EntityBoat.Type.BIRCH));
      registerItem(446, (String)"jungle_boat", new ItemBoat(EntityBoat.Type.JUNGLE));
      registerItem(447, (String)"acacia_boat", new ItemBoat(EntityBoat.Type.ACACIA));
      registerItem(448, (String)"dark_oak_boat", new ItemBoat(EntityBoat.Type.DARK_OAK));
      registerItem(449, (String)"totem_of_undying", (new Item()).setTranslationKey("totem").setMaxStackSize(1).setCreativeTab(CreativeTabs.COMBAT));
      registerItem(450, (String)"shulker_shell", (new Item()).setTranslationKey("shulkerShell").setCreativeTab(CreativeTabs.MATERIALS));
      registerItem(452, (String)"iron_nugget", (new Item()).setTranslationKey("ironNugget").setCreativeTab(CreativeTabs.MATERIALS));
      registerItem(453, (String)"knowledge_book", (new ItemKnowledgeBook()).setTranslationKey("knowledgeBook"));
      registerItem(2256, (String)"record_13", (new ItemRecord("13", SoundEvents.RECORD_13)).setTranslationKey("record"));
      registerItem(2257, (String)"record_cat", (new ItemRecord("cat", SoundEvents.RECORD_CAT)).setTranslationKey("record"));
      registerItem(2258, (String)"record_blocks", (new ItemRecord("blocks", SoundEvents.RECORD_BLOCKS)).setTranslationKey("record"));
      registerItem(2259, (String)"record_chirp", (new ItemRecord("chirp", SoundEvents.RECORD_CHIRP)).setTranslationKey("record"));
      registerItem(2260, (String)"record_far", (new ItemRecord("far", SoundEvents.RECORD_FAR)).setTranslationKey("record"));
      registerItem(2261, (String)"record_mall", (new ItemRecord("mall", SoundEvents.RECORD_MALL)).setTranslationKey("record"));
      registerItem(2262, (String)"record_mellohi", (new ItemRecord("mellohi", SoundEvents.RECORD_MELLOHI)).setTranslationKey("record"));
      registerItem(2263, (String)"record_stal", (new ItemRecord("stal", SoundEvents.RECORD_STAL)).setTranslationKey("record"));
      registerItem(2264, (String)"record_strad", (new ItemRecord("strad", SoundEvents.RECORD_STRAD)).setTranslationKey("record"));
      registerItem(2265, (String)"record_ward", (new ItemRecord("ward", SoundEvents.RECORD_WARD)).setTranslationKey("record"));
      registerItem(2266, (String)"record_11", (new ItemRecord("11", SoundEvents.RECORD_11)).setTranslationKey("record"));
      registerItem(2267, (String)"record_wait", (new ItemRecord("wait", SoundEvents.RECORD_WAIT)).setTranslationKey("record"));
}

Item* Item::setHasSubtypes(bool hasSubtypes)
{
    hasSubtypes = hasSubtypes;
    return this;
}

Item * Item::setMaxDamage(int32_t maxDamageIn)
{
    maxDamage = maxDamageIn;
    if (maxDamageIn > 0) 
    {
        addPropertyOverride(ResourceLocation("damaged"), DAMAGED_GETTER);
        addPropertyOverride(ResourceLocation("damage"), DAMAGE_GETTER);
    }

    return this;
}

RayTraceResult Item::rayTrace(World* worldIn, EntityPlayer* playerIn, bool useLiquids)
{
    float f = playerIn->rotationPitch;
    float f1 = playerIn->rotationYaw;
    double d0 = playerIn->posX;
    double d1 = playerIn->posY + (double)playerIn->getEyeHeight();
    double d2 = playerIn->posZ;
    Vec3d vec3d = Vec3d(d0, d1, d2);
    float f2 = MathHelper::cos(-f1 * 0.017453292F - 3.1415927F);
    float f3 = MathHelper::sin(-f1 * 0.017453292F - 3.1415927F);
    float f4 = -MathHelper::cos(-f * 0.017453292F);
    float f5 = MathHelper::sin(-f * 0.017453292F);
    float f6 = f3 * f4;
    float f7 = f2 * f4;
    double d3 = 5.0;
    Vec3d vec3d1 = vec3d.add((double)f6 * 5.0, (double)f5 * 5.0, (double)f7 * 5.0);
    return worldIn->rayTraceBlocks(vec3d, vec3d1, useLiquids, !useLiquids, false);
}
