#pragma once
#include "../util/registry/RegistryNamespacedDefaultedByKey.h"
#include "../util/ResourceLocation.h"
#include "../util/math/AxisAlignedBB.h"
#include "../util/EnumBlockRenderType.h"
#include "../../../../pcg-cpp/pcg_random.hpp"
#include "../util/BlockRenderLayer.h"
#include "material/EnumPushReaction.h"
#include "SoundType.h"
#include "../world/chunk/BlockStateContainer.h"
#include "state/BlockFaceShape.h"

enum class EnumHand;
class EnumFacing;
class Mirror;
class IBlockState;
class Item;
class Entity;
class Material;
class MapColor;
class IBlockAccess;
class World;
class EntityPlayer;
class ItemStack;
class Explosion;
class TileEntity;
class EntityLivingBase;

enum class EnumOffsetType {
	NONE,
	XZ,
	XYZ
};

class Block
{
public:
	static RegistryNamespacedDefaultedByKey< ResourceLocation, Block> REGISTRY;
	static std::unordered_map<IBlockState*, uint32_t> BLOCK_STATE_IDS;
	static AxisAlignedBB FULL_BLOCK_AABB;
	static AxisAlignedBB NULL_AABB;
	float blockParticleGravity;
	float slipperiness;

	static int32_t getIdFromBlock(Block blockIn);
	static int32_t getStateId(IBlockState* state);
	static Block& getBlockById(int32_t id);
	static IBlockState* getStateById(int32_t id);
	static Block& getBlockFromItem(std::optional<Item> itemIn);
	static std::optional<Block> getBlockFromName(std::string_view name);
	bool isTopSolid(IBlockState* state);
	bool isFullBlock(IBlockState* state);
	bool canEntitySpawn(IBlockState* state, Entity& entityIn);
	uint16_t getLightOpacity(IBlockState* state);
	bool isTranslucent(IBlockState* state);
	uint16_t getLightValue(IBlockState* state);
	bool getUseNeighborBrightness(IBlockState* state);
	Material& getMaterial(IBlockState* state);
	MapColor& getMapColor(IBlockState* state, IBlockAccess& worldIn, BlockPos& pos);
	virtual IBlockState* getStateFromMeta(uint16_t meta);
	virtual uint16_t getMetaFromState(IBlockState* state);
	IBlockState* getActualState(IBlockState* state, IBlockAccess& worldIn, BlockPos& pos);
	virtual IBlockState* withRotation(IBlockState* state, Rotation& rot);
	virtual IBlockState* withMirror(IBlockState* state, Mirror& mirrorIn);
	bool isBlockNormalCube(IBlockState* state);
	bool isNormalCube(IBlockState* state);
	bool causesSuffocation(IBlockState* state);
	virtual bool isFullCube(IBlockState* state);
	bool hasCustomBreakingProgress(IBlockState* state);
	virtual bool isPassable(IBlockAccess& worldIn, BlockPos& pos);
	virtual EnumBlockRenderType getRenderType(IBlockState* state);
	virtual bool isReplaceable(IBlockAccess& worldIn, BlockPos& pos);
	float getBlockHardness(IBlockState* blockState, World& worldIn, BlockPos& pos);
	bool getTickRandomly() const;
	bool hasTileEntity() const;
	virtual AxisAlignedBB& getBoundingBox(IBlockState* state, IBlockAccess& source, BlockPos& pos);
	int32_t getPackedLightmapCoords(IBlockState* state, IBlockAccess& source, BlockPos& pos);
	virtual bool shouldSideBeRendered(IBlockState* blockState, IBlockAccess blockAccess, BlockPos pos, EnumFacing side);
	virtual BlockFaceShape& getBlockFaceShape(IBlockAccess worldIn, IBlockState* state, BlockPos pos, EnumFacing face);
	AxisAlignedBB& getSelectedBoundingBox(IBlockState* state, World worldIn, BlockPos pos);
	void addCollisionBoxToList(IBlockState* state, World worldIn, BlockPos pos, AxisAlignedBB entityBox, std::vector<AxisAlignedBB>& collidingBoxes, std::optional<Entity> entityIn, bool isActualState);
	virtual std::optional<AxisAlignedBB> getCollisionBoundingBox(IBlockState* blockState, IBlockAccess worldIn, BlockPos pos);
	virtual bool isOpaqueCube(IBlockState* state);
	virtual bool canCollideCheck(IBlockState* state, bool hitIfLiquid);
	bool isCollidable() const;
	void randomTick(World worldIn, BlockPos pos, IBlockState* state, pcg32& random);
	virtual void randomDisplayTick(IBlockState* stateIn, World worldIn, BlockPos pos, pcg32& rand);
	void onPlayerDestroy(World worldIn, BlockPos pos, IBlockState* state);
	virtual void neighborChanged(IBlockState* state, World worldIn, BlockPos pos, Block blockIn, BlockPos fromPos);
	virtual int32_t tickRate(World* worldIn);
	virtual void onBlockAdded(World worldIn, BlockPos pos, IBlockState* state);
	virtual void breakBlock(World worldIn, BlockPos pos, IBlockState* state);
	int32_t quantityDropped(pcg32& random) const;
	virtual Item getItemDropped(IBlockState* state, pcg32& rand, int32_t fortune);
	float getPlayerRelativeBlockHardness(IBlockState* state, EntityPlayer player, World worldIn, BlockPos pos);
	void dropBlockAsItem(World worldIn, BlockPos pos, IBlockState* state, int32_t fortune);
	virtual void dropBlockAsItemWithChance(World worldIn, BlockPos pos, IBlockState* state, float chance, int32_t fortune);
	static void spawnAsEntity(World worldIn, BlockPos pos, ItemStack stack);
	virtual int32_t damageDropped(IBlockState* state);
	float getExplosionResistance(Entity exploder);
	std::optional<RayTraceResult> collisionRayTrace(IBlockState* blockState, World worldIn, BlockPos pos, Vec3d start, Vec3d end);
	void onExplosionDestroy(World worldIn, BlockPos pos, Explosion explosionIn);
	BlockRenderLayer getRenderLayer() const;
	bool canPlaceBlockOnSide(World worldIn, BlockPos pos, EnumFacing side);
	virtual bool canPlaceBlockAt(World worldIn, BlockPos pos);
	virtual bool onBlockActivated(World worldIn, BlockPos pos, IBlockState* state, EntityPlayer playerIn, EnumHand hand, EnumFacing facing, float hitX, float hitY, float hitZ);
	void onEntityWalk(World worldIn, BlockPos pos, Entity entityIn);
	virtual IBlockState* getStateForPlacement(World worldIn, BlockPos pos, EnumFacing facing, float hitX, float hitY, float hitZ, int meta, EntityLivingBase placer);
	void onBlockClicked(World worldIn, BlockPos pos, EntityPlayer playerIn);
	Vec3d modifyAcceleration(World worldIn, BlockPos pos, Entity entityIn, Vec3d motion);
	int32_t getWeakPower(IBlockState* blockState, IBlockAccess blockAccess, BlockPos pos, EnumFacing side);
	bool canProvidePower(IBlockState* state);
	void onEntityCollision(World worldIn, BlockPos pos, IBlockState* state, Entity entityIn);
	int32_t getStrongPower(IBlockState* blockState, IBlockAccess blockAccess, BlockPos pos, EnumFacing side);
	virtual void harvestBlock(World worldIn, EntityPlayer player, BlockPos pos, IBlockState* state, std::optional<TileEntity> te, ItemStack stack);
	int32_t quantityDroppedWithBonus(int fortune, pcg32& random);
	void onBlockPlacedBy(World worldIn, BlockPos pos, IBlockState* state, EntityLivingBase placer, ItemStack stack);
	virtual bool canSpawnInBlock();
	Block& setTranslationKey(std::string key);
	virtual std::string getLocalizedName() const;
	std::string getTranslationKey() const;
	virtual bool eventReceived(IBlockState* state, World worldIn, BlockPos pos, int32_t id, int32_t param);
	bool getEnableStats() const;
	EnumPushReaction getPushReaction(IBlockState* state);
	virtual float getAmbientOcclusionLightValue(IBlockState* state);
	void onFallenUpon(World worldIn, BlockPos pos, Entity entityIn, float fallDistance);
	void onLanded(World worldIn, Entity entityIn);
	virtual ItemStack getItem(World worldIn, BlockPos pos, IBlockState* state);
	virtual void getSubBlocks(CreativeTabs itemIn, std::vector<ItemStack>& items);
	CreativeTabs getCreativeTab();
	Block& setCreativeTab(CreativeTabs tab);
	void onBlockHarvested(World worldIn, BlockPos pos, IBlockState* state, EntityPlayer player);
	void fillWithRain(World worldIn, BlockPos pos);
	bool requiresUpdates() const;
	bool canDropFromExplosion(Explosion explosionIn);
	bool isAssociatedBlock(const Block& other) const;
	bool hasComparatorInputOverride(IBlockState* state);
	int32_t getComparatorInputOverride(IBlockState* blockState, World worldIn, BlockPos pos);
	BlockStateContainer& getBlockState();
	IBlockState* getDefaultState();
	EnumOffsetType getOffsetType() const;
	Vec3d getOffset(IBlockState* state, IBlockAccess worldIn, BlockPos pos);
	SoundType getSoundType();
	std::string toString() const;
	void addInformation(ItemStack stack, std::optional<World> worldIn, std::vector<> tooltip, ITooltipFlag flagIn);
	static void registerBlocks();

	Block(Material blockMaterialIn, MapColor blockMapColorIn);
	friend bool operator==(const Block& lhs, const Block& rhs);

protected:
	~Block() = default;
	bool fullBlock;
	uint16_t lightOpacity;
	bool translucent;
	uint16_t lightValue;
	bool useNeighborBrightness;
	float blockHardness;
	float blockResistance;
	bool enableStats;
	bool needsRandomTick;
	bool bTileEntity;
	SoundType blockSoundType;
	Material material;
	MapColor blockMapColor;
	BlockStateContainer blockState;

	Block(Material& materialIn);
	Block& setSoundType(SoundType sound);
	Block& setLightOpacity(uint16_t opacity);
	Block& setLightLevel(float value);
	Block& setResistance(float resistance);
	static bool isExceptionBlockForAttaching(const Block& attachBlock);
	static bool isExceptBlockForAttachWithPiston(Block attachBlock);
	Block& setHardness(float hardness);
	Block& setBlockUnbreakable();
	Block& setTickRandomly(bool shouldTick);
	static void addCollisionBoxToList(BlockPos pos, AxisAlignedBB entityBox, std::vector<AxisAlignedBB>& collidingBoxes, std::optional< AxisAlignedBB> blockBox);
	virtual void updateTick(World worldIn, BlockPos pos, IBlockState* state, pcg32& rand);
	void dropXpOnBlockBreak(World worldIn, BlockPos pos, int32_t amount);
	std::optional<RayTraceResult> rayTrace(BlockPos pos, Vec3d start, Vec3d end, AxisAlignedBB boundingBox);
	bool canSilkHarvest();
	ItemStack getSilkTouchDrop(IBlockState* state); 
	Block& disableStats();
	virtual BlockStateContainer& createBlockState();
	void setDefaultState(IBlockState* state);

private:
	static ResourceLocation AIR_ID;
	CreativeTabs displayOnCreativeTab;
	IBlockState* defaultBlockState;
	std::string translationKey;

	static void registerBlock(int32_t id, ResourceLocation textualID, Block block_);
	static void registerBlock(int32_t id, std::string textualID, Block block_);
};


