#include "Block.h"
#include "../item/ItemBlock.h"
#include "../util/BlockRenderLayer.h"
#include "../util/Mirror.h"
#include "../util/Util.h"
#include "../util/math/AxisAlignedBB.h"
#include "../util/text/translation/I18n.h"
#include "../world/IBlockAccess.h"
#include "../world/chunk/BlockStateContainer.h"
#include <material/Material.h>

ResourceLocation Block::AIR_ID("air");

RegistryNamespacedDefaultedByKey< ResourceLocation, Block> Block::REGISTRY(AIR_ID);
AxisAlignedBB Block::FULL_BLOCK_AABB(0.0, 0.0, 0.0, 1.0, 1.0, 1.0);

int32_t Block::getIdFromBlock(Block* blockIn)
{
	return REGISTRY.getIDForObject(blockIn);
}

int32_t Block::getStateId(IBlockState* state)
{
	auto block = state->getBlock();
	return getIdFromBlock(block) + (block.getMetaFromState(state) << 12);
}

Block* Block::getBlockById(int32_t id)
{
	auto block = REGISTRY.getObjectById(id).value();
	return block;
}
IBlockState* Block::getStateById(int32_t id)
{
	int32_t i = id & 4095;
	int32_t j = id >> 12 & 15;
	return getBlockById(i)->getStateFromMeta(j);
}

Block* Block::getBlockFromItem(Item* itemIn)
{
	return Util::instanceof<ItemBlock>(itemIn) ? ((ItemBlock*)itemIn)->getBlock() : Blocks::AIR;
}

Block* Block::getBlockFromName(std::string_view name)
{
	ResourceLocation resourcelocation(name);
	if (REGISTRY.containsKey(resourcelocation)) {
		return REGISTRY.getObject(resourcelocation);
	}
	else 
	{
		return REGISTRY.getObjectById(std::stoi(name));
	}
}

bool Block::isTopSolid(IBlockState* state)
{
	return state->getMaterial().isOpaque() && state->isFullCube();
}

bool Block::isFullBlock(IBlockState* state) const
{
	return fullBlock;
}

bool Block::canEntitySpawn(IBlockState* state, Entity* entityIn)
{
	return true;
}

uint16_t Block::getLightOpacity(IBlockState* state) const
{
	return lightOpacity;
}

bool Block::isTranslucent(IBlockState* state) const
{
	return translucent;
}

uint16_t Block::getLightValue(IBlockState* state) const
{
	return lightValue;
}

bool Block::getUseNeighborBrightness(IBlockState* state) const
{
	return useNeighborBrightness;
}

Material& Block::getMaterial(IBlockState* state)
{
	return material;
}

MapColor& Block::getMapColor(IBlockState* state, IBlockAccess* worldIn, const BlockPos& pos)
{
	return blockMapColor;
}

IBlockState* Block::getStateFromMeta(uint16_t meta)
{
	return getDefaultState();
}

uint16_t Block::getMetaFromState(IBlockState* state)
{
	if (state->getPropertyKeys().empty()) {
		return 0;
	}
	else {
		throw std::logic_error("Don't know how to convert " + state + " back into data...");
	}
}

IBlockState* Block::getActualState(IBlockState* state, IBlockAccess* worldIn, const BlockPos& pos)
{
	return state;
}

IBlockState* Block::withRotation(IBlockState* state, Rotation& rot)
{
	return state;
}

IBlockState* Block::withMirror(IBlockState* state, Mirror& mirrorIn)
{
	return state;
}

int32_t Block::quantityDroppedWithBonus(int32_t fortune, pcg32& random)
{
	return quantityDropped(random);
}

void Block::registerBlocks()
{
	registerBlock(0, (ResourceLocation)AIR_ID, (BlockAir()).setTranslationKey("air"));
	registerBlock(1, "stone", (BlockStone()).setHardness(1.5F).setResistance(10.0F).setSoundType(SoundType::STONE).setTranslationKey("stone"));
	registerBlock(2, "grass", (BlockGrass()).setHardness(0.6F).setSoundType(SoundType::PLANT).setTranslationKey("grass"));
	registerBlock(3, "dirt", (BlockDirt()).setHardness(0.5F).setSoundType(SoundType::GROUND).setTranslationKey("dirt"));
	Block block = (Block(Material::ROCK)).setHardness(2.0F).setResistance(10.0F).setSoundType(SoundType::STONE).setTranslationKey("stonebrick").setCreativeTab(CreativeTabs.BUILDING_BLOCKS);
	registerBlock(4, "cobblestone", block);
	Block block1 = (BlockPlanks()).setHardness(2.0F).setResistance(5.0F).setSoundType(SoundType::WOOD).setTranslationKey("wood");
	registerBlock(5, "planks", block1);
	registerBlock(6, "sapling", (BlockSapling()).setHardness(0.0F).setSoundType(SoundType::PLANT).setTranslationKey("sapling"));
	registerBlock(7, "bedrock", (BlockEmptyDrops(Material.ROCK)).setBlockUnbreakable().setResistance(6000000.0F).setSoundType(SoundType.STONE).setTranslationKey("bedrock").disableStats().setCreativeTab(CreativeTabs.BUILDING_BLOCKS));
	registerBlock(8, "flowing_water", (BlockDynamicLiquid(Material.WATER)).setHardness(100.0F).setLightOpacity(3).setTranslationKey("water").disableStats());
	registerBlock(9, "water", (BlockStaticLiquid(Material.WATER)).setHardness(100.0F).setLightOpacity(3).setTranslationKey("water").disableStats());
	registerBlock(10, "flowing_lava", (BlockDynamicLiquid(Material.LAVA)).setHardness(100.0F).setLightLevel(1.0F).setTranslationKey("lava").disableStats());
	registerBlock(11, "lava", (BlockStaticLiquid(Material.LAVA)).setHardness(100.0F).setLightLevel(1.0F).setTranslationKey("lava").disableStats());
	registerBlock(12, "sand", (BlockSand()).setHardness(0.5F).setSoundType(SoundType.SAND).setTranslationKey("sand"));
	registerBlock(13, "gravel", (BlockGravel()).setHardness(0.6F).setSoundType(SoundType.GROUND).setTranslationKey("gravel"));
	registerBlock(14, "gold_ore", (BlockOre()).setHardness(3.0F).setResistance(5.0F).setSoundType(SoundType.STONE).setTranslationKey("oreGold"));
	registerBlock(15, "iron_ore", (BlockOre()).setHardness(3.0F).setResistance(5.0F).setSoundType(SoundType.STONE).setTranslationKey("oreIron"));
	registerBlock(16, "coal_ore", ( BlockOre()).setHardness(3.0F).setResistance(5.0F).setSoundType(SoundType.STONE).setTranslationKey("oreCoal"));
	registerBlock(17, "log", (BlockOldLog()).setTranslationKey("log"));
	registerBlock(18, "leaves", (BlockOldLeaf()).setTranslationKey("leaves"));
	registerBlock(19, "sponge", (BlockSponge()).setHardness(0.6F).setSoundType(SoundType.PLANT).setTranslationKey("sponge"));
	registerBlock(20, "glass", (BlockGlass(Material.GLASS, false)).setHardness(0.3F).setSoundType(SoundType.GLASS).setTranslationKey("glass"));
	registerBlock(21, "lapis_ore", (BlockOre()).setHardness(3.0F).setResistance(5.0F).setSoundType(SoundType.STONE).setTranslationKey("oreLapis"));
	registerBlock(22, "lapis_block", (Block(Material.IRON, MapColor.LAPIS)).setHardness(3.0F).setResistance(5.0F).setSoundType(SoundType.STONE).setTranslationKey("blockLapis").setCreativeTab(CreativeTabs.BUILDING_BLOCKS));
	registerBlock(23, "dispenser", (BlockDispenser()).setHardness(3.5F).setSoundType(SoundType.STONE).setTranslationKey("dispenser"));
	Block block2 = (BlockSandStone()).setSoundType(SoundType.STONE).setHardness(0.8F).setTranslationKey("sandStone");
	registerBlock(24, "sandstone", block2);
	registerBlock(25, "noteblock", (BlockNote()).setSoundType(SoundType.WOOD).setHardness(0.8F).setTranslationKey("musicBlock"));
	registerBlock(26, "bed", (BlockBed()).setSoundType(SoundType.WOOD).setHardness(0.2F).setTranslationKey("bed").disableStats());
	registerBlock(27, "golden_rail", (BlockRailPowered()).setHardness(0.7F).setSoundType(SoundType.METAL).setTranslationKey("goldenRail"));
	registerBlock(28, "detector_rail", (BlockRailDetector()).setHardness(0.7F).setSoundType(SoundType.METAL).setTranslationKey("detectorRail"));
	registerBlock(29, "sticky_piston", (BlockPistonBase(true)).setTranslationKey("pistonStickyBase"));
	registerBlock(30, "web", (BlockWeb()).setLightOpacity(1).setHardness(4.0F).setTranslationKey("web"));
	registerBlock(31, "tallgrass", (BlockTallGrass()).setHardness(0.0F).setSoundType(SoundType.PLANT).setTranslationKey("tallgrass"));
	registerBlock(32, "deadbush", (BlockDeadBush()).setHardness(0.0F).setSoundType(SoundType.PLANT).setTranslationKey("deadbush"));
	registerBlock(33, "piston", (BlockPistonBase(false)).setTranslationKey("pistonBase"));
	registerBlock(34, "piston_head", (BlockPistonExtension()).setTranslationKey("pistonBase"));
	registerBlock(35, "wool", (BlockColored(Material.CLOTH)).setHardness(0.8F).setSoundType(SoundType.CLOTH).setTranslationKey("cloth"));
	registerBlock(36, "piston_extension",BlockPistonMoving());
	registerBlock(37, "yellow_flower", ( BlockYellowFlower()).setHardness(0.0F).setSoundType(SoundType.PLANT).setTranslationKey("flower1"));
	registerBlock(38, "red_flower", ( BlockRedFlower()).setHardness(0.0F).setSoundType(SoundType.PLANT).setTranslationKey("flower2"));
	Block block3 = ( BlockMushroom()).setHardness(0.0F).setSoundType(SoundType.PLANT).setLightLevel(0.125F).setTranslationKey("mushroom");
	registerBlock(39, "brown_mushroom", block3);
	Block block4 = (BlockMushroom()).setHardness(0.0F).setSoundType(SoundType.PLANT).setTranslationKey("mushroom");
	registerBlock(40, "red_mushroom", block4);
	registerBlock(41, "gold_block", (Block(Material.IRON, MapColor.GOLD)).setHardness(3.0F).setResistance(10.0F).setSoundType(SoundType.METAL).setTranslationKey("blockGold").setCreativeTab(CreativeTabs.BUILDING_BLOCKS));
	registerBlock(42, "iron_block", (Block(Material.IRON, MapColor.IRON)).setHardness(5.0F).setResistance(10.0F).setSoundType(SoundType.METAL).setTranslationKey("blockIron").setCreativeTab(CreativeTabs.BUILDING_BLOCKS));
	registerBlock(43, "double_stone_slab", (BlockDoubleStoneSlab()).setHardness(2.0F).setResistance(10.0F).setSoundType(SoundType.STONE).setTranslationKey("stoneSlab"));
	registerBlock(44, "stone_slab", (BlockHalfStoneSlab()).setHardness(2.0F).setResistance(10.0F).setSoundType(SoundType.STONE).setTranslationKey("stoneSlab"));
	Block block5 = (Block(Material.ROCK, MapColor.RED)).setHardness(2.0F).setResistance(10.0F).setSoundType(SoundType.STONE).setTranslationKey("brick").setCreativeTab(CreativeTabs.BUILDING_BLOCKS);
	registerBlock(45, "brick_block", block5);
	registerBlock(46, "tnt", (BlockTNT()).setHardness(0.0F).setSoundType(SoundType.PLANT).setTranslationKey("tnt"));
	registerBlock(47, "bookshelf", (BlockBookshelf()).setHardness(1.5F).setSoundType(SoundType.WOOD).setTranslationKey("bookshelf"));
	registerBlock(48, "mossy_cobblestone", (Block(Material.ROCK)).setHardness(2.0F).setResistance(10.0F).setSoundType(SoundType.STONE).setTranslationKey("stoneMoss").setCreativeTab(CreativeTabs.BUILDING_BLOCKS));
	registerBlock(49, "obsidian", (BlockObsidian()).setHardness(50.0F).setResistance(2000.0F).setSoundType(SoundType.STONE).setTranslationKey("obsidian"));
	registerBlock(50, "torch", (BlockTorch()).setHardness(0.0F).setLightLevel(0.9375F).setSoundType(SoundType.WOOD).setTranslationKey("torch"));
	registerBlock(51, "fire", (BlockFire()).setHardness(0.0F).setLightLevel(1.0F).setSoundType(SoundType.CLOTH).setTranslationKey("fire").disableStats());
	registerBlock(52, "mob_spawner", (BlockMobSpawner()).setHardness(5.0F).setSoundType(SoundType.METAL).setTranslationKey("mobSpawner").disableStats());
	registerBlock(53, "oak_stairs", (BlockStairs(block1.getDefaultState().withProperty(BlockPlanks.VARIANT, BlockPlanks.EnumType.OAK))).setTranslationKey("stairsWood"));
	registerBlock(54, "chest", (BlockChest(BlockChest.Type.BASIC)).setHardness(2.5F).setSoundType(SoundType.WOOD).setTranslationKey("chest"));
	registerBlock(55, "redstone_wire", (BlockRedstoneWire()).setHardness(0.0F).setSoundType(SoundType.STONE).setTranslationKey("redstoneDust").disableStats());
	registerBlock(56, "diamond_ore", (BlockOre()).setHardness(3.0F).setResistance(5.0F).setSoundType(SoundType.STONE).setTranslationKey("oreDiamond"));
	registerBlock(57, "diamond_block", (Block(Material.IRON, MapColor.DIAMOND)).setHardness(5.0F).setResistance(10.0F).setSoundType(SoundType.METAL).setTranslationKey("blockDiamond").setCreativeTab(CreativeTabs.BUILDING_BLOCKS));
	registerBlock(58, "crafting_table", (BlockWorkbench()).setHardness(2.5F).setSoundType(SoundType.WOOD).setTranslationKey("workbench"));
	registerBlock(59, "wheat", (BlockCrops()).setTranslationKey("crops"));
	Block block6 = (BlockFarmland()).setHardness(0.6F).setSoundType(SoundType.GROUND).setTranslationKey("farmland");
	registerBlock(60, "farmland", block6);
	registerBlock(61, "furnace", (BlockFurnace(false)).setHardness(3.5F).setSoundType(SoundType.STONE).setTranslationKey("furnace").setCreativeTab(CreativeTabs.DECORATIONS));
	registerBlock(62, "lit_furnace", (BlockFurnace(true)).setHardness(3.5F).setSoundType(SoundType.STONE).setLightLevel(0.875F).setTranslationKey("furnace"));
	registerBlock(63, "standing_sign", (BlockStandingSign()).setHardness(1.0F).setSoundType(SoundType.WOOD).setTranslationKey("sign").disableStats());
	registerBlock(64, "wooden_door", (BlockDoor(Material.WOOD)).setHardness(3.0F).setSoundType(SoundType.WOOD).setTranslationKey("doorOak").disableStats());
	registerBlock(65, "ladder", (BlockLadder()).setHardness(0.4F).setSoundType(SoundType.LADDER).setTranslationKey("ladder"));
	registerBlock(66, "rail", (BlockRail()).setHardness(0.7F).setSoundType(SoundType.METAL).setTranslationKey("rail"));
	registerBlock(67, "stone_stairs", (BlockStairs(block.getDefaultState())).setTranslationKey("stairsStone"));
	registerBlock(68, "wall_sign", (BlockWallSign()).setHardness(1.0F).setSoundType(SoundType.WOOD).setTranslationKey("sign").disableStats());
	registerBlock(69, "lever", (BlockLever()).setHardness(0.5F).setSoundType(SoundType.WOOD).setTranslationKey("lever"));
	registerBlock(70, "stone_pressure_plate", (BlockPressurePlate(Material.ROCK, BlockPressurePlate.Sensitivity.MOBS)).setHardness(0.5F).setSoundType(SoundType.STONE).setTranslationKey("pressurePlateStone"));
	registerBlock(71, "iron_door", (BlockDoor(Material.IRON)).setHardness(5.0F).setSoundType(SoundType.METAL).setTranslationKey("doorIron").disableStats());
	registerBlock(72, "wooden_pressure_plate", (BlockPressurePlate(Material.WOOD, BlockPressurePlate.Sensitivity.EVERYTHING)).setHardness(0.5F).setSoundType(SoundType.WOOD).setTranslationKey("pressurePlateWood"));
	registerBlock(73, "redstone_ore", (BlockRedstoneOre(false)).setHardness(3.0F).setResistance(5.0F).setSoundType(SoundType.STONE).setTranslationKey("oreRedstone").setCreativeTab(CreativeTabs.BUILDING_BLOCKS));
	registerBlock(74, "lit_redstone_ore", (BlockRedstoneOre(true)).setLightLevel(0.625F).setHardness(3.0F).setResistance(5.0F).setSoundType(SoundType.STONE).setTranslationKey("oreRedstone"));
	registerBlock(75, "unlit_redstone_torch", (BlockRedstoneTorch(false)).setHardness(0.0F).setSoundType(SoundType.WOOD).setTranslationKey("notGate"));
	registerBlock(76, "redstone_torch", (BlockRedstoneTorch(true)).setHardness(0.0F).setLightLevel(0.5F).setSoundType(SoundType.WOOD).setTranslationKey("notGate").setCreativeTab(CreativeTabs.REDSTONE));
	registerBlock(77, "stone_button", (BlockButtonStone()).setHardness(0.5F).setSoundType(SoundType.STONE).setTranslationKey("button"));
	registerBlock(78, "snow_layer", (BlockSnow()).setHardness(0.1F).setSoundType(SoundType.SNOW).setTranslationKey("snow").setLightOpacity(0));
	registerBlock(79, "ice", (BlockIce()).setHardness(0.5F).setLightOpacity(3).setSoundType(SoundType.GLASS).setTranslationKey("ice"));
	registerBlock(80, "snow", (BlockSnowBlock()).setHardness(0.2F).setSoundType(SoundType.SNOW).setTranslationKey("snow"));
	registerBlock(81, "cactus", (BlockCactus()).setHardness(0.4F).setSoundType(SoundType.CLOTH).setTranslationKey("cactus"));
	registerBlock(82, "clay", (BlockClay()).setHardness(0.6F).setSoundType(SoundType.GROUND).setTranslationKey("clay"));
	registerBlock(83, "reeds", (BlockReed()).setHardness(0.0F).setSoundType(SoundType.PLANT).setTranslationKey("reeds").disableStats());
	registerBlock(84, "jukebox", (BlockJukebox()).setHardness(2.0F).setResistance(10.0F).setSoundType(SoundType.STONE).setTranslationKey("jukebox"));
	registerBlock(85, "fence", (BlockFence(Material.WOOD, BlockPlanks.EnumType.OAK.getMapColor())).setHardness(2.0F).setResistance(5.0F).setSoundType(SoundType.WOOD).setTranslationKey("fence"));
	Block block7 = (BlockPumpkin()).setHardness(1.0F).setSoundType(SoundType.WOOD).setTranslationKey("pumpkin");
	registerBlock(86, "pumpkin", block7);
	registerBlock(87, "netherrack", (BlockNetherrack()).setHardness(0.4F).setSoundType(SoundType.STONE).setTranslationKey("hellrock"));
	registerBlock(88, "soul_sand", (BlockSoulSand()).setHardness(0.5F).setSoundType(SoundType.SAND).setTranslationKey("hellsand"));
	registerBlock(89, "glowstone", (BlockGlowstone(Material.GLASS)).setHardness(0.3F).setSoundType(SoundType.GLASS).setLightLevel(1.0F).setTranslationKey("lightgem"));
	registerBlock(90, "portal", (BlockPortal()).setHardness(-1.0F).setSoundType(SoundType.GLASS).setLightLevel(0.75F).setTranslationKey("portal"));
	registerBlock(91, "lit_pumpkin", (BlockPumpkin()).setHardness(1.0F).setSoundType(SoundType.WOOD).setLightLevel(1.0F).setTranslationKey("litpumpkin"));
	registerBlock(92, "cake", (BlockCake()).setHardness(0.5F).setSoundType(SoundType.CLOTH).setTranslationKey("cake").disableStats());
	registerBlock(93, "unpowered_repeater", (BlockRedstoneRepeater(false)).setHardness(0.0F).setSoundType(SoundType.WOOD).setTranslationKey("diode").disableStats());
	registerBlock(94, "powered_repeater", (BlockRedstoneRepeater(true)).setHardness(0.0F).setSoundType(SoundType.WOOD).setTranslationKey("diode").disableStats());
	registerBlock(95, "stained_glass", (BlockStainedGlass(Material.GLASS)).setHardness(0.3F).setSoundType(SoundType.GLASS).setTranslationKey("stainedGlass"));
	registerBlock(96, "trapdoor", (BlockTrapDoor(Material.WOOD)).setHardness(3.0F).setSoundType(SoundType.WOOD).setTranslationKey("trapdoor").disableStats());
	registerBlock(97, "monster_egg", (BlockSilverfish()).setHardness(0.75F).setTranslationKey("monsterStoneEgg"));
	Block block8 = (BlockStoneBrick()).setHardness(1.5F).setResistance(10.0F).setSoundType(SoundType.STONE).setTranslationKey("stonebricksmooth");
	registerBlock(98, "stonebrick", block8);
	registerBlock(99, "brown_mushroom_block", (BlockHugeMushroom(Material.WOOD, MapColor.DIRT, block3)).setHardness(0.2F).setSoundType(SoundType.WOOD).setTranslationKey("mushroom"));
	registerBlock(100, "red_mushroom_block", (BlockHugeMushroom(Material.WOOD, MapColor.RED, block4)).setHardness(0.2F).setSoundType(SoundType.WOOD).setTranslationKey("mushroom"));
	registerBlock(101, "iron_bars", (BlockPane(Material.IRON, true)).setHardness(5.0F).setResistance(10.0F).setSoundType(SoundType.METAL).setTranslationKey("fenceIron"));
	registerBlock(102, "glass_pane", (BlockPane(Material.GLASS, false)).setHardness(0.3F).setSoundType(SoundType.GLASS).setTranslationKey("thinGlass"));
	Block block9 = (BlockMelon()).setHardness(1.0F).setSoundType(SoundType.WOOD).setTranslationKey("melon");
	registerBlock(103, "melon_block", block9);
	registerBlock(104, "pumpkin_stem", (BlockStem(block7)).setHardness(0.0F).setSoundType(SoundType.WOOD).setTranslationKey("pumpkinStem"));
	registerBlock(105, "melon_stem", (BlockStem(block9)).setHardness(0.0F).setSoundType(SoundType.WOOD).setTranslationKey("pumpkinStem"));
	registerBlock(106, "vine", (BlockVine()).setHardness(0.2F).setSoundType(SoundType.PLANT).setTranslationKey("vine"));
	registerBlock(107, "fence_gate", (BlockFenceGate(BlockPlanks.EnumType.OAK)).setHardness(2.0F).setResistance(5.0F).setSoundType(SoundType.WOOD).setTranslationKey("fenceGate"));
	registerBlock(108, "brick_stairs", (BlockStairs(block5.getDefaultState())).setTranslationKey("stairsBrick"));
	registerBlock(109, "stone_brick_stairs", (BlockStairs(block8.getDefaultState().withProperty(BlockStoneBrick.VARIANT, BlockStoneBrick.EnumType.DEFAULT))).setTranslationKey("stairsStoneBrickSmooth"));
	registerBlock(110, "mycelium", (BlockMycelium()).setHardness(0.6F).setSoundType(SoundType.PLANT).setTranslationKey("mycel"));
	registerBlock(111, "waterlily", (BlockLilyPad()).setHardness(0.0F).setSoundType(SoundType.PLANT).setTranslationKey("waterlily"));
	Block block10 = (BlockNetherBrick()).setHardness(2.0F).setResistance(10.0F).setSoundType(SoundType.STONE).setTranslationKey("netherBrick").setCreativeTab(CreativeTabs.BUILDING_BLOCKS);
	registerBlock(112, "nether_brick", block10);
	registerBlock(113, "nether_brick_fence", (BlockFence(Material.ROCK, MapColor.NETHERRACK)).setHardness(2.0F).setResistance(10.0F).setSoundType(SoundType.STONE).setTranslationKey("netherFence"));
	registerBlock(114, "nether_brick_stairs", (BlockStairs(block10.getDefaultState())).setTranslationKey("stairsNetherBrick"));
	registerBlock(115, "nether_wart", (BlockNetherWart()).setTranslationKey("netherStalk"));
	registerBlock(116, "enchanting_table", (BlockEnchantmentTable()).setHardness(5.0F).setResistance(2000.0F).setTranslationKey("enchantmentTable"));
	registerBlock(117, "brewing_stand", (BlockBrewingStand()).setHardness(0.5F).setLightLevel(0.125F).setTranslationKey("brewingStand"));
	registerBlock(118, "cauldron", (BlockCauldron()).setHardness(2.0F).setTranslationKey("cauldron"));
	registerBlock(119, "end_portal", (BlockEndPortal(Material.PORTAL)).setHardness(-1.0F).setResistance(6000000.0F));
	registerBlock(120, "end_portal_frame", (BlockEndPortalFrame()).setSoundType(SoundType.GLASS).setLightLevel(0.125F).setHardness(-1.0F).setTranslationKey("endPortalFrame").setResistance(6000000.0F).setCreativeTab(CreativeTabs.DECORATIONS));
	registerBlock(121, "end_stone", (Block(Material.ROCK, MapColor.SAND)).setHardness(3.0F).setResistance(15.0F).setSoundType(SoundType.STONE).setTranslationKey("whiteStone").setCreativeTab(CreativeTabs.BUILDING_BLOCKS));
	registerBlock(122, "dragon_egg", (BlockDragonEgg()).setHardness(3.0F).setResistance(15.0F).setSoundType(SoundType.STONE).setLightLevel(0.125F).setTranslationKey("dragonEgg"));
	registerBlock(123, "redstone_lamp", (BlockRedstoneLight(false)).setHardness(0.3F).setSoundType(SoundType.GLASS).setTranslationKey("redstoneLight").setCreativeTab(CreativeTabs.REDSTONE));
	registerBlock(124, "lit_redstone_lamp", (BlockRedstoneLight(true)).setHardness(0.3F).setSoundType(SoundType.GLASS).setTranslationKey("redstoneLight"));
	registerBlock(125, "double_wooden_slab", (BlockDoubleWoodSlab()).setHardness(2.0F).setResistance(5.0F).setSoundType(SoundType.WOOD).setTranslationKey("woodSlab"));
	registerBlock(126, "wooden_slab", (BlockHalfWoodSlab()).setHardness(2.0F).setResistance(5.0F).setSoundType(SoundType.WOOD).setTranslationKey("woodSlab"));
	registerBlock(127, "cocoa", (BlockCocoa()).setHardness(0.2F).setResistance(5.0F).setSoundType(SoundType.WOOD).setTranslationKey("cocoa"));
	registerBlock(128, "sandstone_stairs", (BlockStairs(block2.getDefaultState().withProperty(BlockSandStone.TYPE, BlockSandStone.EnumType.SMOOTH))).setTranslationKey("stairsSandStone"));
	registerBlock(129, "emerald_ore", (BlockOre()).setHardness(3.0F).setResistance(5.0F).setSoundType(SoundType.STONE).setTranslationKey("oreEmerald"));
	registerBlock(130, "ender_chest", (BlockEnderChest()).setHardness(22.5F).setResistance(1000.0F).setSoundType(SoundType.STONE).setTranslationKey("enderChest").setLightLevel(0.5F));
	registerBlock(131, "tripwire_hook", (BlockTripWireHook()).setTranslationKey("tripWireSource"));
	registerBlock(132, "tripwire", (BlockTripWire()).setTranslationKey("tripWire"));
	registerBlock(133, "emerald_block", (Block(Material.IRON, MapColor.EMERALD)).setHardness(5.0F).setResistance(10.0F).setSoundType(SoundType.METAL).setTranslationKey("blockEmerald").setCreativeTab(CreativeTabs.BUILDING_BLOCKS));
	registerBlock(134, "spruce_stairs", (BlockStairs(block1.getDefaultState().withProperty(BlockPlanks.VARIANT, BlockPlanks.EnumType.SPRUCE))).setTranslationKey("stairsWoodSpruce"));
	registerBlock(135, "birch_stairs", (BlockStairs(block1.getDefaultState().withProperty(BlockPlanks.VARIANT, BlockPlanks.EnumType.BIRCH))).setTranslationKey("stairsWoodBirch"));
	registerBlock(136, "jungle_stairs", (BlockStairs(block1.getDefaultState().withProperty(BlockPlanks.VARIANT, BlockPlanks.EnumType.JUNGLE))).setTranslationKey("stairsWoodJungle"));
	registerBlock(137, "command_block", (BlockCommandBlock(MapColor.BROWN)).setBlockUnbreakable().setResistance(6000000.0F).setTranslationKey("commandBlock"));
	registerBlock(138, "beacon", (BlockBeacon()).setTranslationKey("beacon").setLightLevel(1.0F));
	registerBlock(139, "cobblestone_wall", (BlockWall(block)).setTranslationKey("cobbleWall"));
	registerBlock(140, "flower_pot", (BlockFlowerPot()).setHardness(0.0F).setSoundType(SoundType.STONE).setTranslationKey("flowerPot"));
	registerBlock(141, "carrots", (BlockCarrot()).setTranslationKey("carrots"));
	registerBlock(142, "potatoes", (BlockPotato()).setTranslationKey("potatoes"));
	registerBlock(143, "wooden_button", (BlockButtonWood()).setHardness(0.5F).setSoundType(SoundType.WOOD).setTranslationKey("button"));
	registerBlock(144, "skull", (BlockSkull()).setHardness(1.0F).setSoundType(SoundType.STONE).setTranslationKey("skull"));
	registerBlock(145, "anvil", (BlockAnvil()).setHardness(5.0F).setSoundType(SoundType.ANVIL).setResistance(2000.0F).setTranslationKey("anvil"));
	registerBlock(146, "trapped_chest", (BlockChest(BlockChest.Type.TRAP)).setHardness(2.5F).setSoundType(SoundType.WOOD).setTranslationKey("chestTrap"));
	registerBlock(147, "light_weighted_pressure_plate", (BlockPressurePlateWeighted(Material.IRON, 15, MapColor.GOLD)).setHardness(0.5F).setSoundType(SoundType.WOOD).setTranslationKey("weightedPlate_light"));
	registerBlock(148, "heavy_weighted_pressure_plate", (BlockPressurePlateWeighted(Material.IRON, 150)).setHardness(0.5F).setSoundType(SoundType.WOOD).setTranslationKey("weightedPlate_heavy"));
	registerBlock(149, "unpowered_comparator", (BlockRedstoneComparator(false)).setHardness(0.0F).setSoundType(SoundType.WOOD).setTranslationKey("comparator").disableStats());
	registerBlock(150, "powered_comparator", (BlockRedstoneComparator(true)).setHardness(0.0F).setLightLevel(0.625F).setSoundType(SoundType.WOOD).setTranslationKey("comparator").disableStats());
	registerBlock(151, "daylight_detector", BlockDaylightDetector(false));
	registerBlock(152, "redstone_block", (BlockCompressedPowered(Material.IRON, MapColor.TNT)).setHardness(5.0F).setResistance(10.0F).setSoundType(SoundType.METAL).setTranslationKey("blockRedstone").setCreativeTab(CreativeTabs.REDSTONE));
	registerBlock(153, "quartz_ore", (BlockOre(MapColor.NETHERRACK)).setHardness(3.0F).setResistance(5.0F).setSoundType(SoundType.STONE).setTranslationKey("netherquartz"));
	registerBlock(154, "hopper", (BlockHopper()).setHardness(3.0F).setResistance(8.0F).setSoundType(SoundType.METAL).setTranslationKey("hopper"));
	Block block11 = (BlockQuartz()).setSoundType(SoundType.STONE).setHardness(0.8F).setTranslationKey("quartzBlock");
	registerBlock(155, "quartz_block", block11);
	registerBlock(156, "quartz_stairs", (BlockStairs(block11.getDefaultState().withProperty(BlockQuartz.VARIANT, BlockQuartz.EnumType.DEFAULT))).setTranslationKey("stairsQuartz"));
	registerBlock(157, "activator_rail", (BlockRailPowered()).setHardness(0.7F).setSoundType(SoundType.METAL).setTranslationKey("activatorRail"));
	registerBlock(158, "dropper", (BlockDropper()).setHardness(3.5F).setSoundType(SoundType.STONE).setTranslationKey("dropper"));
	registerBlock(159, "stained_hardened_clay", (BlockStainedHardenedClay()).setHardness(1.25F).setResistance(7.0F).setSoundType(SoundType.STONE).setTranslationKey("clayHardenedStained"));
	registerBlock(160, "stained_glass_pane", (BlockStainedGlassPane()).setHardness(0.3F).setSoundType(SoundType.GLASS).setTranslationKey("thinStainedGlass"));
	registerBlock(161, "leaves2", (BlockNewLeaf()).setTranslationKey("leaves"));
	registerBlock(162, "log2", (BlockNewLog()).setTranslationKey("log"));
	registerBlock(163, "acacia_stairs", (BlockStairs(block1.getDefaultState().withProperty(BlockPlanks.VARIANT, BlockPlanks.EnumType.ACACIA))).setTranslationKey("stairsWoodAcacia"));
	registerBlock(164, "dark_oak_stairs", (BlockStairs(block1.getDefaultState().withProperty(BlockPlanks.VARIANT, BlockPlanks.EnumType.DARK_OAK))).setTranslationKey("stairsWoodDarkOak"));
	registerBlock(165, "slime", (BlockSlime()).setTranslationKey("slime").setSoundType(SoundType.SLIME));
	registerBlock(166, "barrier", (BlockBarrier()).setTranslationKey("barrier"));
	registerBlock(167, "iron_trapdoor", (BlockTrapDoor(Material.IRON)).setHardness(5.0F).setSoundType(SoundType.METAL).setTranslationKey("ironTrapdoor").disableStats());
	registerBlock(168, "prismarine", (BlockPrismarine()).setHardness(1.5F).setResistance(10.0F).setSoundType(SoundType.STONE).setTranslationKey("prismarine"));
	registerBlock(169, "sea_lantern", (BlockSeaLantern(Material.GLASS)).setHardness(0.3F).setSoundType(SoundType.GLASS).setLightLevel(1.0F).setTranslationKey("seaLantern"));
	registerBlock(170, "hay_block", (BlockHay()).setHardness(0.5F).setSoundType(SoundType.PLANT).setTranslationKey("hayBlock").setCreativeTab(CreativeTabs.BUILDING_BLOCKS));
	registerBlock(171, "carpet", (BlockCarpet()).setHardness(0.1F).setSoundType(SoundType.CLOTH).setTranslationKey("woolCarpet").setLightOpacity(0));
	registerBlock(172, "hardened_clay", (BlockHardenedClay()).setHardness(1.25F).setResistance(7.0F).setSoundType(SoundType.STONE).setTranslationKey("clayHardened"));
	registerBlock(173, "coal_block", (Block(Material.ROCK, MapColor.BLACK)).setHardness(5.0F).setResistance(10.0F).setSoundType(SoundType.STONE).setTranslationKey("blockCoal").setCreativeTab(CreativeTabs.BUILDING_BLOCKS));
	registerBlock(174, "packed_ice", (BlockPackedIce()).setHardness(0.5F).setSoundType(SoundType.GLASS).setTranslationKey("icePacked"));
	registerBlock(175, "double_plant", BlockDoublePlant());
	registerBlock(176, "standing_banner", (BlockBanner.BlockBannerStanding()).setHardness(1.0F).setSoundType(SoundType.WOOD).setTranslationKey("banner").disableStats());
	registerBlock(177, "wall_banner", (BlockBanner.BlockBannerHanging()).setHardness(1.0F).setSoundType(SoundType.WOOD).setTranslationKey("banner").disableStats());
	registerBlock(178, "daylight_detector_inverted", BlockDaylightDetector(true));
	Block block12 = (BlockRedSandstone()).setSoundType(SoundType.STONE).setHardness(0.8F).setTranslationKey("redSandStone");
	registerBlock(179, "red_sandstone", block12);
	registerBlock(180, "red_sandstone_stairs", (BlockStairs(block12.getDefaultState().withProperty(BlockRedSandstone.TYPE, BlockRedSandstone.EnumType.SMOOTH))).setTranslationKey("stairsRedSandStone"));
	registerBlock(181, "double_stone_slab2", (BlockDoubleStoneSlabNew()).setHardness(2.0F).setResistance(10.0F).setSoundType(SoundType.STONE).setTranslationKey("stoneSlab2"));
	registerBlock(182, "stone_slab2", (BlockHalfStoneSlabNew()).setHardness(2.0F).setResistance(10.0F).setSoundType(SoundType.STONE).setTranslationKey("stoneSlab2"));
	registerBlock(183, "spruce_fence_gate", (BlockFenceGate(BlockPlanks.EnumType.SPRUCE)).setHardness(2.0F).setResistance(5.0F).setSoundType(SoundType.WOOD).setTranslationKey("spruceFenceGate"));
	registerBlock(184, "birch_fence_gate", (BlockFenceGate(BlockPlanks.EnumType.BIRCH)).setHardness(2.0F).setResistance(5.0F).setSoundType(SoundType.WOOD).setTranslationKey("birchFenceGate"));
	registerBlock(185, "jungle_fence_gate", (BlockFenceGate(BlockPlanks.EnumType.JUNGLE)).setHardness(2.0F).setResistance(5.0F).setSoundType(SoundType.WOOD).setTranslationKey("jungleFenceGate"));
	registerBlock(186, "dark_oak_fence_gate", (BlockFenceGate(BlockPlanks.EnumType.DARK_OAK)).setHardness(2.0F).setResistance(5.0F).setSoundType(SoundType.WOOD).setTranslationKey("darkOakFenceGate"));
	registerBlock(187, "acacia_fence_gate", (BlockFenceGate(BlockPlanks.EnumType.ACACIA)).setHardness(2.0F).setResistance(5.0F).setSoundType(SoundType.WOOD).setTranslationKey("acaciaFenceGate"));
	registerBlock(188, "spruce_fence", (BlockFence(Material.WOOD, BlockPlanks.EnumType.SPRUCE.getMapColor())).setHardness(2.0F).setResistance(5.0F).setSoundType(SoundType.WOOD).setTranslationKey("spruceFence"));
	registerBlock(189, "birch_fence", (BlockFence(Material.WOOD, BlockPlanks.EnumType.BIRCH.getMapColor())).setHardness(2.0F).setResistance(5.0F).setSoundType(SoundType.WOOD).setTranslationKey("birchFence"));
	registerBlock(190, "jungle_fence", (BlockFence(Material.WOOD, BlockPlanks.EnumType.JUNGLE.getMapColor())).setHardness(2.0F).setResistance(5.0F).setSoundType(SoundType.WOOD).setTranslationKey("jungleFence"));
	registerBlock(191, "dark_oak_fence", (BlockFence(Material.WOOD, BlockPlanks.EnumType.DARK_OAK.getMapColor())).setHardness(2.0F).setResistance(5.0F).setSoundType(SoundType.WOOD).setTranslationKey("darkOakFence"));
	registerBlock(192, "acacia_fence", (BlockFence(Material.WOOD, BlockPlanks.EnumType.ACACIA.getMapColor())).setHardness(2.0F).setResistance(5.0F).setSoundType(SoundType.WOOD).setTranslationKey("acaciaFence"));
	registerBlock(193, "spruce_door", (BlockDoor(Material.WOOD)).setHardness(3.0F).setSoundType(SoundType.WOOD).setTranslationKey("doorSpruce").disableStats());
	registerBlock(194, "birch_door", (BlockDoor(Material.WOOD)).setHardness(3.0F).setSoundType(SoundType.WOOD).setTranslationKey("doorBirch").disableStats());
	registerBlock(195, "jungle_door", (BlockDoor(Material.WOOD)).setHardness(3.0F).setSoundType(SoundType.WOOD).setTranslationKey("doorJungle").disableStats());
	registerBlock(196, "acacia_door", (BlockDoor(Material.WOOD)).setHardness(3.0F).setSoundType(SoundType.WOOD).setTranslationKey("doorAcacia").disableStats());
	registerBlock(197, "dark_oak_door", ( BlockDoor(Material.WOOD)).setHardness(3.0F).setSoundType(SoundType.WOOD).setTranslationKey("doorDarkOak").disableStats());
	registerBlock(198, "end_rod", (BlockEndRod()).setHardness(0.0F).setLightLevel(0.9375F).setSoundType(SoundType.WOOD).setTranslationKey("endRod"));
	registerBlock(199, "chorus_plant", (BlockChorusPlant()).setHardness(0.4F).setSoundType(SoundType.WOOD).setTranslationKey("chorusPlant"));
	registerBlock(200, "chorus_flower", (BlockChorusFlower()).setHardness(0.4F).setSoundType(SoundType.WOOD).setTranslationKey("chorusFlower"));
	Block block13 = (Block(Material.ROCK, MapColor.MAGENTA)).setHardness(1.5F).setResistance(10.0F).setSoundType(SoundType.STONE).setCreativeTab(CreativeTabs.BUILDING_BLOCKS).setTranslationKey("purpurBlock");
	registerBlock(201, "purpur_block", block13);
	registerBlock(202, "purpur_pillar", (BlockRotatedPillar(Material.ROCK, MapColor.MAGENTA)).setHardness(1.5F).setResistance(10.0F).setSoundType(SoundType.STONE).setCreativeTab(CreativeTabs.BUILDING_BLOCKS).setTranslationKey("purpurPillar"));
	registerBlock(203, "purpur_stairs", (BlockStairs(block13.getDefaultState())).setTranslationKey("stairsPurpur"));
	registerBlock(204, "purpur_double_slab", (BlockPurpurSlab.Double()).setHardness(2.0F).setResistance(10.0F).setSoundType(SoundType.STONE).setTranslationKey("purpurSlab"));
	registerBlock(205, "purpur_slab", (BlockPurpurSlab.Half()).setHardness(2.0F).setResistance(10.0F).setSoundType(SoundType.STONE).setTranslationKey("purpurSlab"));
	registerBlock(206, "end_bricks", (Block(Material.ROCK, MapColor.SAND)).setSoundType(SoundType.STONE).setHardness(0.8F).setCreativeTab(CreativeTabs.BUILDING_BLOCKS).setTranslationKey("endBricks"));
	registerBlock(207, "beetroots", (BlockBeetroot()).setTranslationKey("beetroots"));
	Block block14 = (BlockGrassPath()).setHardness(0.65F).setSoundType(SoundType.PLANT).setTranslationKey("grassPath").disableStats();
	registerBlock(208, "grass_path", block14);
	registerBlock(209, "end_gateway", (BlockEndGateway(Material.PORTAL)).setHardness(-1.0F).setResistance(6000000.0F));
	registerBlock(210, "repeating_command_block", (BlockCommandBlock(MapColor.PURPLE)).setBlockUnbreakable().setResistance(6000000.0F).setTranslationKey("repeatingCommandBlock"));
	registerBlock(211, "chain_command_block", (BlockCommandBlock(MapColor.GREEN)).setBlockUnbreakable().setResistance(6000000.0F).setTranslationKey("chainCommandBlock"));
	registerBlock(212, "frosted_ice", (BlockFrostedIce()).setHardness(0.5F).setLightOpacity(3).setSoundType(SoundType.GLASS).setTranslationKey("frostedIce"));
	registerBlock(213, "magma", (BlockMagma()).setHardness(0.5F).setSoundType(SoundType.STONE).setTranslationKey("magma"));
	registerBlock(214, "nether_wart_block", (Block(Material.GRASS, MapColor.RED)).setCreativeTab(CreativeTabs.BUILDING_BLOCKS).setHardness(1.0F).setSoundType(SoundType.WOOD).setTranslationKey("netherWartBlock"));
	registerBlock(215, "red_nether_brick", (BlockNetherBrick()).setHardness(2.0F).setResistance(10.0F).setSoundType(SoundType.STONE).setTranslationKey("redNetherBrick").setCreativeTab(CreativeTabs.BUILDING_BLOCKS));
	registerBlock(216, "bone_block", (BlockBone()).setTranslationKey("boneBlock"));
	registerBlock(217, "structure_void", (BlockStructureVoid()).setTranslationKey("structureVoid"));
	registerBlock(218, "observer", (BlockObserver()).setHardness(3.0F).setTranslationKey("observer"));
	registerBlock(219, "white_shulker_box", (BlockShulkerBox(EnumDyeColor.WHITE)).setHardness(2.0F).setSoundType(SoundType.STONE).setTranslationKey("shulkerBoxWhite"));
	registerBlock(220, "orange_shulker_box", (BlockShulkerBox(EnumDyeColor.ORANGE)).setHardness(2.0F).setSoundType(SoundType.STONE).setTranslationKey("shulkerBoxOrange"));
	registerBlock(221, "magenta_shulker_box", (BlockShulkerBox(EnumDyeColor.MAGENTA)).setHardness(2.0F).setSoundType(SoundType.STONE).setTranslationKey("shulkerBoxMagenta"));
	registerBlock(222, "light_blue_shulker_box", (BlockShulkerBox(EnumDyeColor.LIGHT_BLUE)).setHardness(2.0F).setSoundType(SoundType.STONE).setTranslationKey("shulkerBoxLightBlue"));
	registerBlock(223, "yellow_shulker_box", (BlockShulkerBox(EnumDyeColor.YELLOW)).setHardness(2.0F).setSoundType(SoundType.STONE).setTranslationKey("shulkerBoxYellow"));
	registerBlock(224, "lime_shulker_box", (BlockShulkerBox(EnumDyeColor.LIME)).setHardness(2.0F).setSoundType(SoundType.STONE).setTranslationKey("shulkerBoxLime"));
	registerBlock(225, "pink_shulker_box", (BlockShulkerBox(EnumDyeColor.PINK)).setHardness(2.0F).setSoundType(SoundType.STONE).setTranslationKey("shulkerBoxPink"));
	registerBlock(226, "gray_shulker_box", (BlockShulkerBox(EnumDyeColor.GRAY)).setHardness(2.0F).setSoundType(SoundType.STONE).setTranslationKey("shulkerBoxGray"));
	registerBlock(227, "silver_shulker_box", (BlockShulkerBox(EnumDyeColor.SILVER)).setHardness(2.0F).setSoundType(SoundType.STONE).setTranslationKey("shulkerBoxSilver"));
	registerBlock(228, "cyan_shulker_box", (BlockShulkerBox(EnumDyeColor.CYAN)).setHardness(2.0F).setSoundType(SoundType.STONE).setTranslationKey("shulkerBoxCyan"));
	registerBlock(229, "purple_shulker_box", (BlockShulkerBox(EnumDyeColor.PURPLE)).setHardness(2.0F).setSoundType(SoundType.STONE).setTranslationKey("shulkerBoxPurple"));
	registerBlock(230, "blue_shulker_box", (BlockShulkerBox(EnumDyeColor.BLUE)).setHardness(2.0F).setSoundType(SoundType.STONE).setTranslationKey("shulkerBoxBlue"));
	registerBlock(231, "brown_shulker_box", (BlockShulkerBox(EnumDyeColor.BROWN)).setHardness(2.0F).setSoundType(SoundType.STONE).setTranslationKey("shulkerBoxBrown"));
	registerBlock(232, "green_shulker_box", (BlockShulkerBox(EnumDyeColor.GREEN)).setHardness(2.0F).setSoundType(SoundType.STONE).setTranslationKey("shulkerBoxGreen"));
	registerBlock(233, "red_shulker_box", (BlockShulkerBox(EnumDyeColor.RED)).setHardness(2.0F).setSoundType(SoundType.STONE).setTranslationKey("shulkerBoxRed"));
	registerBlock(234, "black_shulker_box", (BlockShulkerBox(EnumDyeColor.BLACK)).setHardness(2.0F).setSoundType(SoundType.STONE).setTranslationKey("shulkerBoxBlack"));
	registerBlock(235, "white_glazed_terracotta", BlockGlazedTerracotta(EnumDyeColor.WHITE));
	registerBlock(236, "orange_glazed_terracotta", BlockGlazedTerracotta(EnumDyeColor.ORANGE));
	registerBlock(237, "magenta_glazed_terracotta", BlockGlazedTerracotta(EnumDyeColor.MAGENTA));
	registerBlock(238, "light_blue_glazed_terracotta", BlockGlazedTerracotta(EnumDyeColor.LIGHT_BLUE));
	registerBlock(239, "yellow_glazed_terracotta", BlockGlazedTerracotta(EnumDyeColor.YELLOW));
	registerBlock(240, "lime_glazed_terracotta", BlockGlazedTerracotta(EnumDyeColor.LIME));
	registerBlock(241, "pink_glazed_terracotta", BlockGlazedTerracotta(EnumDyeColor.PINK));
	registerBlock(242, "gray_glazed_terracotta", BlockGlazedTerracotta(EnumDyeColor.GRAY));
	registerBlock(243, "silver_glazed_terracotta", BlockGlazedTerracotta(EnumDyeColor.SILVER));
	registerBlock(244, "cyan_glazed_terracotta", BlockGlazedTerracotta(EnumDyeColor.CYAN));
	registerBlock(245, "purple_glazed_terracotta", BlockGlazedTerracotta(EnumDyeColor.PURPLE));
	registerBlock(246, "blue_glazed_terracotta", BlockGlazedTerracotta(EnumDyeColor.BLUE));
	registerBlock(247, "brown_glazed_terracotta", BlockGlazedTerracotta(EnumDyeColor.BROWN));
	registerBlock(248, "green_glazed_terracotta", BlockGlazedTerracotta(EnumDyeColor.GREEN));
	registerBlock(249, "red_glazed_terracotta", BlockGlazedTerracotta(EnumDyeColor.RED));
	registerBlock(250, "black_glazed_terracotta", BlockGlazedTerracotta(EnumDyeColor.BLACK));
	registerBlock(251, "concrete", (BlockColored(Material.ROCK)).setHardness(1.8F).setSoundType(SoundType.STONE).setTranslationKey("concrete"));
	registerBlock(252, "concrete_powder", (BlockConcretePowder()).setHardness(0.5F).setSoundType(SoundType.SAND).setTranslationKey("concretePowder"));
	registerBlock(255, "structure_block", (BlockStructure()).setBlockUnbreakable().setResistance(6000000.0F).setTranslationKey("structureBlock"));
	REGISTRY.validateKey();

	while (true) {
		for(auto reg : REGISTRY)
		{
			auto& block15 = reg.second;
			if (block15.material == Material::AIR) {
				block15.useNeighborBrightness = false;
			}
			else {
				bool flag = false;
				bool flag1 = Util::instanceof< BlockStairs>(block15);
				bool flag2 = Util::instanceof< BlockSlab>(block15);
				bool flag3 = block15 == block6 || block15 == block14;
				bool flag4 = block15.translucent;
				bool flag5 = block15.lightOpacity == 0;
				if (flag1 || flag2 || flag3 || flag4 || flag5) {
					flag = true;
				}

				block15.useNeighborBrightness = flag;
			}
		}

		std::array<Block,1> setarray = { REGISTRY.getObject(ResourceLocation("tripwire"))};
		auto set = std::set<Block>(setarray.begin(), setarray.end());

		while (true) {
			for (auto reg : REGISTRY)
			{
				auto block16 = reg.second;
				if (set.find(block16) != set.end()) {
					for (auto i = 0; i < 15; ++i) {
						auto j = REGISTRY.getIDForObject(block16) << 4 | i;
						BLOCK_STATE_IDS.emplace(block16.getStateFromMeta(i), j);
					}
				}
				else {
					auto unmodifiableiterator = block16.getBlockState().getValidStates();

					for (auto iblockstate : unmodifiableiterator)
					{
						auto k = REGISTRY.getIDForObject(block16) << 4 | block16.getMetaFromState(iblockstate);
						BLOCK_STATE_IDS.emplace(iblockstate, k);
					}
				}
			}

			return;
		}
	}
}

Block::Block(Material blockMaterialIn, MapColor blockMapColorIn)
{
	enableStats = true;
	blockSoundType = SoundType::STONE;
	blockParticleGravity = 1.0F;
	slipperiness = 0.6F;
	material = blockMaterialIn;
	blockMapColor = blockMapColorIn;
	blockState = createBlockState();
	setDefaultState(blockState.getBaseState());
	fullBlock = getDefaultState()->isOpaqueCube();
	lightOpacity = fullBlock ? 255 : 0;
	translucent = !blockMaterialIn.blocksLight();
}

Block::Block(Material materialIn)
	:Block(materialIn, materialIn.getMaterialMapColor())
{
	
}

Block& Block::setSoundType(SoundType sound)
{
	blockSoundType = sound;
	return *this;
}

Block& Block::setLightOpacity(uint16_t opacity)
{
	lightOpacity = opacity;
	return *this;
}

Block& Block::setLightLevel(float value)
{
	lightValue = (uint16_t)(15.0F * value);
	return *this;
}

Block& Block::setResistance(float resistance)
{
	blockResistance = resistance * 3.0F;
	return *this;
}

bool Block::isExceptionBlockForAttaching(const Block& attachBlock)
{
	return instanceof<BlockShulkerBox>(attachBlock)   || instanceof< BlockLeaves>(attachBlock) ||  instanceof< BlockTrapDoor>(attachBlock) || attachBlock == Blocks.BEACON || attachBlock == Blocks.CAULDRON || attachBlock == Blocks.GLASS || attachBlock == Blocks.GLOWSTONE || attachBlock == Blocks.ICE || attachBlock == Blocks.SEA_LANTERN || attachBlock == Blocks.STAINED_GLASS;
}

bool Block::isExceptBlockForAttachWithPiston(Block attachBlock)
{
	return isExceptionBlockForAttaching(attachBlock) || attachBlock == Blocks.PISTON || attachBlock == Blocks.STICKY_PISTON || attachBlock == Blocks.PISTON_HEAD;
}

Block& Block::setHardness(float hardness)
{
	blockHardness = hardness;
	if (blockResistance < hardness * 5.0F) {
		blockResistance = hardness * 5.0F;
	}

	return *this;
}

Block& Block::setBlockUnbreakable()
{
	setHardness(-1.0F);
	return *this;
}

bool Block::isBlockNormalCube(IBlockState* state)
{
	return state->getMaterial().blocksMovement() && state->isFullCube();
}

bool Block::isNormalCube(IBlockState* state)
{
	return state->getMaterial().isOpaque() && state->isFullCube() && !state->canProvidePower();
}

bool Block::causesSuffocation(IBlockState* state)
{
	return material.blocksMovement() && getDefaultState()->isFullCube();
}

bool Block::isFullCube(IBlockState* state)
{
	return true;
}

bool Block::hasCustomBreakingProgress(IBlockState* state)
{
	return false;
}

bool Block::isPassable(IBlockAccess* worldIn, const BlockPos& pos)
{
	return !material.blocksMovement();
}

EnumBlockRenderType Block::getRenderType(IBlockState* state)
{
	return EnumBlockRenderType::MODEL;
}

bool Block::isReplaceable(IBlockAccess* worldIn, const BlockPos& pos)
{
	return false;
}

float Block::getBlockHardness(IBlockState* blockState, World* worldIn, const BlockPos& pos)
{
	return blockHardness;
}

bool Block::getTickRandomly() const
{
	return needsRandomTick;
}

bool Block::hasTileEntity() const
{
	return bTileEntity;
}

Block& Block::setTickRandomly(bool shouldTick)
{
	needsRandomTick = shouldTick;
	return *this;
}

void Block::addCollisionBoxToList(BlockPos pos, AxisAlignedBB entityBox, std::vector<AxisAlignedBB>& collidingBoxes,
	std::optional<AxisAlignedBB> blockBox)
{
	if (blockBox) {
		auto axisalignedbb = blockBox.value().offset(pos);
		if (entityBox.intersects(axisalignedbb)) {
			collidingBoxes.emplace_back(axisalignedbb);
		}
	}
}

AxisAlignedBB& Block::getBoundingBox(IBlockState* state, IBlockAccess* source, const BlockPos& pos)
{
	return FULL_BLOCK_AABB;
}

int32_t Block::getPackedLightmapCoords(IBlockState* state, IBlockAccess* source, const BlockPos& pos)
{
	auto i = source->getCombinedLight(pos, state->getLightValue());
	if (i == 0 &&  Util::instanceof<BlockSlab>(state->getBlock())) {
		pos = pos.down();
		state = source->getBlockState(pos);
		return source->getCombinedLight(pos, state->getLightValue());
	}
	else {
		return i;
	}
}

bool Block::shouldSideBeRendered(IBlockState* blockState, IBlockAccess* blockAccess, const BlockPos& pos, EnumFacing side)
{
	auto axisalignedbb = blockState->getBoundingBox(blockAccess, pos);

	if (side == EnumFacing::DOWN)
	{
		if (axisalignedbb.getminY() > 0.0) {
			return true;
		}
	}

	if (side == EnumFacing::UP)
	{
		if (axisalignedbb.getmaxY() < 1.0) {
			return true;
		}
	}
	if (side == EnumFacing::NORTH)
	{
		if (axisalignedbb.getminZ() > 0.0) {
			return true;
		}
	}
	if (side == EnumFacing::SOUTH)
	{
		if (axisalignedbb.getmaxZ() < 1.0) {
			return true;
		}
	}
	if (side == EnumFacing::WEST)
	{
		if (axisalignedbb.getminX() > 0.0) {
			return true;
		}
	}
	if (side == EnumFacing::EAST)
	{
		if (axisalignedbb.getmaxX() < 1.0) {
			return true;
		}
	}
	return !blockAccess->getBlockState(pos.offset(side))->isOpaqueCube();
}

BlockFaceShape Block::getBlockFaceShape(IBlockAccess* worldIn, IBlockState* state, const BlockPos& pos, EnumFacing face)
{
	return BlockFaceShape::SOLID;
}

AxisAlignedBB& Block::getSelectedBoundingBox(IBlockState* state, World* worldIn, const BlockPos& pos)
{
	return state->getBoundingBox(worldIn, pos).offset(pos);
}

void Block::addCollisionBoxToList(IBlockState* state, World* worldIn, const BlockPos& pos, const AxisAlignedBB& entityBox, const std::vector<AxisAlignedBB>& collidingBoxes, Entity* entityIn, bool isActualState)
{
	addCollisionBoxToList(pos, entityBox, collidingBoxes, state->getCollisionBoundingBox(worldIn, pos));
}

std::optional<AxisAlignedBB> Block::getCollisionBoundingBox(IBlockState* blockState, IBlockAccess* worldIn, const BlockPos& pos)
{
	return blockState->getBoundingBox(worldIn, pos);
}

bool Block::isOpaqueCube(IBlockState* state)
{
	return true;
}

bool Block::canCollideCheck(IBlockState* state, bool hitIfLiquid)
{
	return isCollidable();
}

bool Block::isCollidable() const
{
	return true;
}

void Block::randomTick(World* worldIn, const BlockPos& pos, IBlockState* state, pcg32& random)
{
	updateTick(worldIn, pos, state, random);
}

void Block::updateTick(World* worldIn, const BlockPos& pos, IBlockState* state, pcg32& rand)
{
	
}

void Block::randomDisplayTick(IBlockState* stateIn, World* worldIn, const BlockPos& pos, pcg32& rand)
{
	
}

void Block::onPlayerDestroy(World* worldIn, const BlockPos& pos, IBlockState* state)
{
	
}

void Block::neighborChanged(IBlockState* state, World* worldIn, const BlockPos& pos, Block* blockIn, const BlockPos& fromPos)
{
	
}

int32_t Block::tickRate(World* worldIn)
{
	return 10;
}

void Block::onBlockAdded(World* worldIn, const BlockPos& pos, IBlockState* state)
{
	
}

void Block::breakBlock(World* worldIn, const BlockPos& pos, IBlockState* state)
{
	
}

int32_t Block::quantityDropped(pcg32& random) const
{
	return 1;
}

Item* Block::getItemDropped(IBlockState* state, pcg32& rand, int32_t fortune)
{
	return Item.getItemFromBlock(this);
}

float Block::getPlayerRelativeBlockHardness(IBlockState* state, EntityPlayer* player, World* worldIn, const BlockPos& pos)
{
	float f = state->getBlockHardness(worldIn, pos);
	if (f < 0.0F) {
		return 0.0F;
	}
	else {
		return !player->canHarvestBlock(state) ? player->getDigSpeed(state) / f / 100.0F : player.getDigSpeed(state) / f / 30.0F;
	}
}

void Block::dropBlockAsItem(World* worldIn, const BlockPos& pos, IBlockState* state, int32_t fortune)
{
	dropBlockAsItemWithChance(worldIn, pos, state, 1.0F, fortune);
}

void Block::dropBlockAsItemWithChance(World* worldIn, const BlockPos& pos, IBlockState* state, float chance, int32_t fortune)
{
	if (!worldIn->isRemote) {
		auto i = quantityDroppedWithBonus(fortune, worldIn->rand);

		for (auto j = 0; j < i; ++j) {
			if (MathHelper::nextFloat(worldIn->rand) <= chance) {
				auto *item = getItemDropped(state, worldIn->rand, fortune);
				if (item != Items::AIR) {
					spawnAsEntity(worldIn, pos, ItemStack(item, 1, damageDropped(state)));
				}
			}
		}
	}
}

void Block::spawnAsEntity(World* worldIn, const BlockPos& pos, ItemStack stack)
{
	if (!worldIn->isRemote && !stack.isEmpty() && worldIn->getGameRules().getBoolean("doTileDrops")) {
		float f = 0.5F;
		double d0 = (double)(MathHelper::nextFloat(worldIn->rand) * 0.5F) + 0.25;
		double d1 = (double)(MathHelper::nextFloat(worldIn->rand) * 0.5F) + 0.25;
		double d2 = (double)(MathHelper::nextFloat(worldIn->rand) * 0.5F) + 0.25;
		EntityItem* entityitem = new EntityItem(worldIn, (double)pos.getx() + d0, (double)pos.gety() + d1, (double)pos.getz() + d2, stack);
		entityitem->setDefaultPickupDelay();
		worldIn->spawnEntity(entityitem);
	}
}

void Block::dropXpOnBlockBreak(World* worldIn, const BlockPos& pos, int32_t amount)
{
	if (!worldIn->isRemote && worldIn->getGameRules().getBoolean("doTileDrops")) {
		while (amount > 0) {
			auto i = EntityXPOrb::getXPSplit(amount);
			amount -= i;
			worldIn->spawnEntity(EntityXPOrb(worldIn, (double)pos.getx() + 0.5, (double)pos.gety() + 0.5, (double)pos.getz() + 0.5, i));
		}
	}
}

std::optional<RayTraceResult> Block::rayTrace(BlockPos pos, Vec3d start, Vec3d end, AxisAlignedBB boundingBox)
{
	Vec3d vec3d = start.subtract((double)pos.getx(), (double)pos.gety(), (double)pos.getz());
	Vec3d vec3d1 = end.subtract((double)pos.getx(), (double)pos.gety(), (double)pos.getz());
	auto raytraceresult = boundingBox.calculateIntercept(vec3d, vec3d1);
	return (!raytraceresult) ? std::nullopt : RayTraceResult(raytraceresult.value().hitVec.add((double)pos.getx(), (double)pos.gety(), (double)pos.getz()), raytraceresult.value().sideHit, pos);
}

bool Block::canSilkHarvest()
{
	return getDefaultState()->isFullCube() && !bTileEntity;
}

int32_t Block::damageDropped(IBlockState* state)
{
	return 0;
}

float Block::getExplosionResistance(Entity* exploder)
{
	return blockResistance / 5.0F;
}

std::optional<RayTraceResult> Block::collisionRayTrace(IBlockState* blockState, World* worldIn, const BlockPos& pos, const Vec3d& start, const Vec3d& end)
{
	return rayTrace(pos, start, end, blockState->getBoundingBox(worldIn, pos));
}

void Block::onExplosionDestroy(World* worldIn, const BlockPos& pos, Explosion explosionIn)
{
	
}

BlockRenderLayer Block::getRenderLayer() const
{
	return BlockRenderLayer::Solid;
}

bool Block::canPlaceBlockOnSide(World* worldIn, const BlockPos& pos, EnumFacing side)
{
	return canPlaceBlockAt(worldIn, pos);
}

bool Block::canPlaceBlockAt(World* worldIn, const BlockPos& pos)
{
	return worldIn->getBlockState(pos).getBlock().material.isReplaceable();
}

bool Block::onBlockActivated(World* worldIn, const BlockPos& pos, IBlockState* state, EntityPlayer* playerIn, EnumHand hand, EnumFacing facing, float hitX, float hitY, float hitZ)
{
	return false;
}

void Block::onEntityWalk(World* worldIn, const BlockPos& pos, Entity* entityIn)
{
	
}

IBlockState Block::getStateForPlacement(World* worldIn, const BlockPos& pos, EnumFacing facing, float hitX, float hitY, float hitZ, int meta, EntityLivingBase* placer)
{
	return getStateFromMeta(meta);
}

void Block::onBlockClicked(World* worldIn, const BlockPos& pos, EntityPlayer* playerIn)
{
	
}

Vec3d Block::modifyAcceleration(World* worldIn, const BlockPos& pos, Entity* entityIn, Vec3d motion)
{
	return motion;
}

int32_t Block::getWeakPower(IBlockState* blockState, IBlockAccess* blockAccess, const BlockPos& pos, EnumFacing side)
{
	return 0;
}

bool Block::canProvidePower(IBlockState* state)
{
	return false;
}

void Block::onEntityCollision(World* worldIn, const BlockPos& pos, IBlockState* state, Entity* entityIn)
{}

int32_t Block::getStrongPower(IBlockState* blockState, IBlockAccess* blockAccess, const BlockPos& pos, EnumFacing side)
{
	return 0;
}

void Block::harvestBlock(World* worldIn, EntityPlayer* player, const BlockPos& pos, IBlockState* state, TileEntity* te, ItemStack stack)
{
	player->addStat(StatList.getBlockStats(this));
	player->addExhaustion(0.005F);
	if (canSilkHarvest() && EnchantmentHelper::getEnchantmentLevel(Enchantments::SILK_TOUCH, stack) > 0) {
		ItemStack itemstack = getSilkTouchDrop(state);
		spawnAsEntity(worldIn, pos, itemstack);
	}
	else {
		auto i = EnchantmentHelper::getEnchantmentLevel(Enchantments::FORTUNE, stack);
		dropBlockAsItem(worldIn, pos, state, i);
	}
}

ItemStack Block::getSilkTouchDrop(IBlockState* state)
{
	auto item = Item::getItemFromBlock(this);
	auto i = 0;
	if (item->getHasSubtypes()) {
		i = getMetaFromState(state);
	}

	return ItemStack(item, 1, i);
}

Block& Block::disableStats()
{
	enableStats = false;
	return *this;
}

bool operator==(const Block& lhs, const Block& rhs)
{
	return true;
}

void Block::onBlockPlacedBy(World* worldIn, const BlockPos& pos, IBlockState* state, EntityLivingBase* placer, ItemStack stack)
{
	
}

bool Block::canSpawnInBlock()
{
	return !material.isSolid() && !material.isLiquid();
}

Block* Block::setTranslationKey(std::string_view key)
{
	translationKey = key;
	return this;
}

std::string Block::getLocalizedName() const
{
	return I18n::translateToLocal(getTranslationKey() + ".name");
}

std::string Block::getTranslationKey() const
{
	return "tile." + translationKey;
}

bool Block::eventReceived(IBlockState* state, World* worldIn, const BlockPos& pos, int32_t id, int32_t param)
{
	return false;
}

bool Block::getEnableStats() const
{
	return enableStats;
}

EnumPushReaction Block::getPushReaction(IBlockState* state)
{
	return material.getPushReaction();
}

float Block::getAmbientOcclusionLightValue(IBlockState* state)
{
	return state->isBlockNormalCube() ? 0.2F : 1.0F;
}

void Block::onFallenUpon(World* worldIn, const BlockPos& pos, Entity* entityIn, float fallDistance)
{
	entityIn->fall(fallDistance, 1.0F);
}

void Block::onLanded(World* worldIn, Entity* entityIn)
{
	entityIn->motionY = 0.0;
}

ItemStack Block::getItem(World* worldIn, const BlockPos& pos, IBlockState* state)
{
	return ItemStack(Item.getItemFromBlock(this), 1, damageDropped(state));
}

void Block::getSubBlocks(CreativeTabs itemIn, std::vector<ItemStack>& items)
{
	items.emplace_back(*this);
}

CreativeTabs Block::getCreativeTab()
{
	return displayOnCreativeTab;
}

Block* Block::setCreativeTab(CreativeTabs tab)
{
	displayOnCreativeTab = tab;
	return this;
}

void Block::onBlockHarvested(World* worldIn, const BlockPos& pos, IBlockState* state, EntityPlayer* player)
{
	
}

void Block::fillWithRain(World* worldIn, const BlockPos& pos)
{
	
}

bool Block::requiresUpdates() const
{
	return true;
}

bool Block::canDropFromExplosion(Explosion explosionIn)
{
	return true;
}

bool Block::isAssociatedBlock(const Block* other) const
{
	return this == other;
}

bool Block::hasComparatorInputOverride(IBlockState* state)
{
	return false;
}

int32_t Block::getComparatorInputOverride(IBlockState* blockState, World* worldIn, const BlockPos& pos)
{
	return 0;
}

BlockStateContainer& Block::createBlockState()
{
	return BlockStateContainer(*this,IProperty[0]);
}

BlockStateContainer& Block::getBlockState()
{
	return blockState;
}

void Block::setDefaultState(IBlockState* state)
{
	defaultBlockState = state;
}

void Block::registerBlock(int32_t id, ResourceLocation textualID, Block* block_)
{
	REGISTRY.registe(id, textualID, block_);
}

void Block::registerBlock(int32_t id, std::string_view textualID, Block* block_)
{
	registerBlock(id,ResourceLocation(textualID), block_);
}

IBlockState* Block::getDefaultState()
{
	return defaultBlockState;
}

EnumOffsetType Block::getOffsetType() const
{
	return EnumOffsetType::NONE;
}

Vec3d Block::getOffset(IBlockState* state, IBlockAccess* worldIn, const BlockPos& pos)
{
	EnumOffsetType block$enumoffsettype = getOffsetType();
	if (block$enumoffsettype == EnumOffsetType::NONE) {
		return Vec3d::ZERO;
	}
	else {
		auto i = MathHelper::getCoordinateRandom(pos.getx(), 0, pos.getz());
		return Vec3d(((double)((float)(i >> 16 & 15L) / 15.0F) - 0.5) * 0.5, block$enumoffsettype == EnumOffsetType::XYZ ? ((double)((float)(i >> 20 & 15L) / 15.0F) - 1.0) * 0.2 : 0.0, ((double)((float)(i >> 24 & 15L) / 15.0F) - 0.5) * 0.5);
	}
}

SoundType Block::getSoundType()
{
	return blockSoundType;
}

std::string Block::toString() const
{
	return "Block{" + REGISTRY.getNameForObject(*this).value().to_string() + "}";
}

void addInformation(ItemStack stack, World* worldIn, std::vector<std::string> tooltip, ITooltipFlag* flagIn)
{
}