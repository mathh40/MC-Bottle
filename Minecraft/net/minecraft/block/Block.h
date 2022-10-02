#pragma once

#include "../creativetab/CreativeTabs.h"
#include "../item/ItemStack.h"
#include "../util/BlockRenderLayer.h"
#include "../util/registry/RegistryNamespacedDefaultedByKey.h"
#include "../world/Explosion.h"
#include "../world/IBlockAccess.h"
#include "../world/chunk/BlockStateContainer.h"
#include "SoundType.h"
#include <EnumHand.h>

enum class EnumOffsetType {
    NONE,
    XZ,
    XYZ
};
class Item;
class IBlockAccess;
class ITooltipFlag;
class Mirror;
class TileEntity;

class Block {
public:
    static RegistryNamespacedDefaultedByKey<ResourceLocation, Block> REGISTRY;
    static std::unordered_map<IBlockState *, uint32_t> BLOCK_STATE_IDS;
    static AxisAlignedBB FULL_BLOCK_AABB;
    static AxisAlignedBB NULL_AABB;
    float blockParticleGravity;
    float slipperiness;

    static uint32_t getIdFromBlock(Block *blockIn);
    static uint32_t getStateId(IBlockState *state);
    static Block *getBlockById(uint32_t id);
    static IBlockState *getStateById(int32_t id);
    static Block *getBlockFromItem(Item *itemIn);
    static Block *getBlockFromName(std::string_view name);
    bool isTopSolid(IBlockState *state);
    bool isFullBlock(IBlockState *state) const;
    static bool canEntitySpawn(IBlockState *state, Entity *entityIn);
    uint16_t getLightOpacity(IBlockState *state) const;
    bool isTranslucent(IBlockState *state) const;
    uint16_t getLightValue(IBlockState *state) const;
    bool getUseNeighborBrightness(IBlockState *state) const;
    Material &getMaterial(IBlockState *state);
    MapColor &getMapColor(IBlockState *state, IBlockAccess *worldIn, const BlockPos &pos);
    virtual IBlockState *getStateFromMeta(uint16_t meta);
    virtual uint16_t getMetaFromState(IBlockState *state);
    IBlockState *getActualState(IBlockState *state, IBlockAccess *worldIn, const BlockPos &pos);
    virtual IBlockState *withRotation(IBlockState *state, Rotation &rot);
    virtual IBlockState *withMirror(IBlockState *state, Mirror &mirrorIn);
    bool isBlockNormalCube(IBlockState *state);
    bool isNormalCube(IBlockState *state);
    bool causesSuffocation(IBlockState *state);
    virtual bool isFullCube(IBlockState *state);
    bool hasCustomBreakingProgress(IBlockState *state);
    virtual bool isPassable(IBlockAccess *worldIn, const BlockPos &pos);
    virtual EnumBlockRenderType getRenderType(IBlockState *state);
    virtual bool isReplaceable(IBlockAccess *worldIn, const BlockPos &pos);
    float getBlockHardness(IBlockState *blockState, World *worldIn, const BlockPos &pos);
    bool getTickRandomly() const;
    bool hasTileEntity() const;
    virtual AxisAlignedBB &getBoundingBox(IBlockState *state, IBlockAccess *source, const BlockPos &pos);
    int32_t getPackedLightmapCoords(IBlockState *state, IBlockAccess *source, const BlockPos &pos);
    virtual bool shouldSideBeRendered(IBlockState *blockState, IBlockAccess *blockAccess, const BlockPos &pos,
                                      EnumFacing side);
    virtual BlockFaceShape getBlockFaceShape(IBlockAccess *worldIn, IBlockState *state, const BlockPos &pos,
                                             EnumFacing face);
    AxisAlignedBB &getSelectedBoundingBox(IBlockState *state, World *worldIn, const BlockPos &pos);
    void addCollisionBoxToList(IBlockState *state, World *worldIn, const BlockPos &pos, const AxisAlignedBB &entityBox,
                               const std::vector<AxisAlignedBB> &collidingBoxes, Entity *entityIn, bool isActualState);
    virtual std::optional<AxisAlignedBB> getCollisionBoundingBox(IBlockState *blockState, IBlockAccess *worldIn,
                                                                 const BlockPos &pos);
    virtual bool isOpaqueCube(IBlockState *state);
    virtual bool canCollideCheck(IBlockState *state, bool hitIfLiquid);
    bool isCollidable() const;
    void randomTick(World *worldIn, const BlockPos &pos, IBlockState *state, Random &random);
    virtual void randomDisplayTick(IBlockState *stateIn, World *worldIn, const BlockPos &pos, Random &rand);
    void onPlayerDestroy(World *worldIn, const BlockPos &pos, IBlockState *state);
    virtual void neighborChanged(IBlockState *state, World *worldIn, const BlockPos &pos, Block *blockIn,
                                 const BlockPos &fromPos);
    virtual int32_t tickRate(World *worldIn);
    virtual void onBlockAdded(World *worldIn, const BlockPos &pos, IBlockState *state);
    virtual void breakBlock(World *worldIn, const BlockPos &pos, IBlockState *state);
    uint32_t quantityDropped(Random &random) const;
    virtual Item *getItemDropped(IBlockState *state, Random &rand, uint32_t fortune);
    float getPlayerRelativeBlockHardness(IBlockState *state, EntityPlayer *player, World *worldIn, const BlockPos &pos);
    void dropBlockAsItem(World *worldIn, const BlockPos &pos, IBlockState *state, uint32_t fortune);
    virtual void dropBlockAsItemWithChance(World *worldIn, const BlockPos &pos, IBlockState *state, float chance,
                                           uint32_t fortune);
    static void spawnAsEntity(World *worldIn, const BlockPos &pos, ItemStack& stack);
    virtual uint32_t damageDropped(IBlockState *state);
    float getExplosionResistance(Entity *exploder);
    std::optional<RayTraceResult> collisionRayTrace(IBlockState *blockState, World *worldIn, const BlockPos &pos,
                                                    const Vec3d &start, const Vec3d &end);
    void onExplosionDestroy(World *worldIn, const BlockPos &pos, Explosion explosionIn);
    BlockRenderLayer getRenderLayer() const;
    bool canPlaceBlockOnSide(World *worldIn, const BlockPos &pos, EnumFacing side);
    virtual bool canPlaceBlockAt(World *worldIn, const BlockPos &pos);
    virtual bool onBlockActivated(World *worldIn, const BlockPos &pos, IBlockState *state, EntityPlayer *playerIn,
                                  EnumHand hand, EnumFacing facing, float hitX, float hitY, float hitZ);
    void onEntityWalk(World *worldIn, const BlockPos &pos, Entity *entityIn);
    virtual IBlockState* getStateForPlacement(World *worldIn, const BlockPos &pos, EnumFacing facing, float hitX,
                                              float hitY, float hitZ, uint32_t meta, EntityLivingBase *placer);
    void onBlockClicked(World *worldIn, const BlockPos &pos, EntityPlayer *playerIn);
    Vec3d modifyAcceleration(World *worldIn, const BlockPos &pos, Entity *entityIn, Vec3d motion);
    uint32_t getWeakPower(IBlockState *blockState, IBlockAccess *blockAccess, const BlockPos &pos, EnumFacing side);
    bool canProvidePower(IBlockState *state);
    void onEntityCollision(World *worldIn, const BlockPos &pos, IBlockState *state, Entity *entityIn);
    uint32_t getStrongPower(IBlockState *blockState, IBlockAccess *blockAccess, const BlockPos &pos, EnumFacing side);
    virtual void harvestBlock(World *worldIn, EntityPlayer *player, const BlockPos &pos, IBlockState *state,
                              TileEntity *te, ItemStack stack);
    uint32_t quantityDroppedWithBonus(uint32_t fortune, Random &random);
    void onBlockPlacedBy(World *worldIn, const BlockPos &pos, IBlockState *state, EntityLivingBase *placer,
                         ItemStack stack);
    virtual bool canSpawnInBlock();
    Block *setTranslationKey(std::string_view key);
    virtual std::string getLocalizedName() const;
    std::string getTranslationKey() const;
    virtual bool eventReceived(IBlockState *state, World *worldIn, const BlockPos &pos, uint32_t id, uint32_t param);
    bool getEnableStats() const;
    EnumPushReaction getPushReaction(IBlockState *state);
    virtual float getAmbientOcclusionLightValue(IBlockState *state);
    void onFallenUpon(World *worldIn, const BlockPos &pos, Entity *entityIn, float fallDistance);
    void onLanded(World *worldIn, Entity *entityIn);
    virtual ItemStack getItem(World *worldIn, const BlockPos &pos, IBlockState *state);
    virtual void getSubBlocks(CreativeTabs itemIn, std::vector<ItemStack> &items);
    CreativeTabs getCreativeTab();
    Block *setCreativeTab(CreativeTabs tab);
    void onBlockHarvested(World *worldIn, const BlockPos &pos, IBlockState *state, EntityPlayer *player);
    void fillWithRain(World *worldIn, const BlockPos &pos);
    bool requiresUpdates() const;
    bool canDropFromExplosion(Explosion explosionIn);
    bool isAssociatedBlock(const Block *other) const;
    bool hasComparatorInputOverride(IBlockState *state);
    uint32_t getComparatorInputOverride(IBlockState *blockState, World *worldIn, const BlockPos &pos);
    BlockStateContainer &getBlockState();
    IBlockState *getDefaultState();
    EnumOffsetType getOffsetType() const;
    Vec3d getOffset(IBlockState *state, IBlockAccess *worldIn, const BlockPos &pos);
    SoundType getSoundType();
    std::string toString() const;
    void addInformation(ItemStack& stack, World *worldIn, std::vector<std::string> tooltip, ITooltipFlag *flagIn);
    static void registerBlocks();

    Block(Material blockMaterialIn, MapColor blockMapColorIn);
    friend bool operator==(const Block &lhs, const Block &rhs);

protected:
    virtual ~Block() = default;
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

    Block(Material &materialIn);
    Block *setSoundType(SoundType sound);
    Block *setLightOpacity(uint16_t opacity);
    Block *setLightLevel(float value);
    Block *setResistance(float resistance);
    static bool isExceptionBlockForAttaching(const Block *attachBlock);
    static bool isExceptBlockForAttachWithPiston(Block *attachBlock);
    Block *setHardness(float hardness);
    Block *setBlockUnbreakable();
    Block *setTickRandomly(bool shouldTick);
    static void addCollisionBoxToList(const BlockPos& pos, AxisAlignedBB entityBox, std::vector<AxisAlignedBB> &collidingBoxes,
                                      std::optional<AxisAlignedBB> blockBox);
    virtual void updateTick(World *worldIn, const BlockPos &pos, IBlockState *state, Random &rand);
    void dropXpOnBlockBreak(World *worldIn, const BlockPos &pos, uint32_t amount);
    std::optional<RayTraceResult> rayTrace(const BlockPos& pos, Vec3d start, Vec3d end, AxisAlignedBB boundingBox);
    bool canSilkHarvest();
    ItemStack getSilkTouchDrop(IBlockState *state);
    Block *disableStats();
    virtual BlockStateContainer &createBlockState();
    void setDefaultState(IBlockState *state);

private:
    static ResourceLocation AIR_ID;
    CreativeTabs *displayOnCreativeTab;
    IBlockState *defaultBlockState;
    std::string translationKey;

    static void registerBlock(uint32_t id, ResourceLocation textualID, Block *block_);
    static void registerBlock(uint32_t id, std::string_view textualID, Block *block_);
};
