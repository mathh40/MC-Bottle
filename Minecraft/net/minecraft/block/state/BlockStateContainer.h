#pragma once
#include "BlockStateBase.h"

#include <span>
#include <unordered_map>
#include <vector>

class Block;
class IBlockState;

class BlockStateContainer
{
  public:
    class StateImplementation : public BlockStateBase
    {
      public:
        StateImplementation(Block *blockIn, const std::unordered_map<> &propertiesIn);

        std::vector<const std::string> getPropertyKeys() override;
        std::function<bool()> getValue(IProperty *property) override;
        IBlockState *withProperty(IProperty *property, std::function<bool()> value) override;
        std::unordered_map<uint64_t, IProperty *> getProperties() override;
        Block *getBlock() override;
        friend bool operator==(const StateImplementation &lhs, const StateImplementation &rhs);
        void buildPropertyValueTable(
            std::unordered_map<std::unordered_map<IProperty *, std::set<std::any>>, IBlockState *> &map);
        Material &getMaterial() override;
        bool isFullBlock() override;
        bool canEntitySpawn(Entity *entityIn) override;
        uint16_t getLightOpacity() override;
        uint16_t getLightValue() override;
        bool isTranslucent() override;
        bool useNeighborBrightness() override;
        MapColor &getMapColor(IBlockAccess *p_185909_1_, const BlockPos &p_185909_2_) override;
        IBlockState *withRotation(Rotation &rot) override;
        IBlockState *withMirror(Mirror &mirrorIn) override;
        bool isFullCube() override;
        bool hasCustomBreakingProgress() override;
        EnumBlockRenderType getRenderType() override;
        int32_t getPackedLightmapCoords(IBlockAccess *source, const BlockPos &pos) override;
        float getAmbientOcclusionLightValue() override;
        bool isBlockNormalCube() override;
        bool isNormalCube() override;
        bool canProvidePower() override;
        uint32_t getWeakPower(IBlockAccess *blockAccess, const BlockPos &pos, EnumFacing &side) override;
        bool hasComparatorInputOverride() override;
        uint32_t getComparatorInputOverride(World *worldIn, const BlockPos &pos) override;
        float getBlockHardness(World *worldIn, const BlockPos &pos) override;
        float getPlayerRelativeBlockHardness(EntityPlayer *player, World *worldIn, const BlockPos &pos) override;
        uint32_t getStrongPower(IBlockAccess *blockAccess, const BlockPos &pos, EnumFacing &side) override;
        EnumPushReaction getPushReaction() override;
        IBlockState *getActualState(IBlockAccess *blockAccess, const BlockPos &pos) override;
        AxisAlignedBB &getSelectedBoundingBox(World *worldIn, const BlockPos &pos) override;
        bool shouldSideBeRendered(IBlockAccess *blockAccess, const BlockPos &pos, EnumFacing &facing) override;
        bool isOpaqueCube() override;
        std::optional<AxisAlignedBB> getCollisionBoundingBox(IBlockAccess *worldIn, const BlockPos &pos) override;
        void addCollisionBoxToList(World *worldIn, const BlockPos &pos, AxisAlignedBB &entityBox,
                                   std::optional<std::vector<AxisAlignedBB>> &collidingBoxes, Entity *entityIn,
                                   bool p_185908_6_) override;
        AxisAlignedBB &getBoundingBox(IBlockAccess *blockAccess, const BlockPos &pos) override;
        RayTraceResult &collisionRayTrace(World *worldIn, const BlockPos &pos, Vec3d &start, Vec3d &end) override;
        bool isTopSolid() override;
        Vec3d &getOffset(IBlockAccess *access, const BlockPos &pos) override;
        bool onBlockEventReceived(World *worldIn, const BlockPos &pos, uint32_t id, uint32_t param);
        void neighborChanged(World *worldIn, const BlockPos &pos, Block *blockIn, const BlockPos &fromPos) override;
        bool causesSuffocation() override;
        BlockFaceShape &getBlockFaceShape(IBlockAccess *worldIn, const BlockPos &pos, EnumFacing &facing) override;

      private:
        std::unordered_map<uint64_t, IProperty *> getPropertiesWithValue(IProperty *property,
                                                                         std::function<bool()> value);
        Block *block;
        std::unordered_map<uint64_t, IProperty *> properties;
        std::unordered_map<> propertyValueTable;
    };

    BlockStateContainer(Block *blockIn, std::span<IProperty *> properties);
    static std::string validateProperty(Block *block, IProperty *property);
    std::vector<std::shared_ptr<BlockStateContainer::StateImplementation>> getValidStates();
    IBlockState *getBaseState();
    Block *getBlock();
    std::vector<> getProperties();
    std::string toString();
    IProperty *getProperty(std::string_view propertyName);

  private:
    std::vector<std::set<std::any>> getAllowedValues();

    Block *block;
    std::unordered_map<std::string, IProperty *> properties;
    std::vector<std::shared_ptr<BlockStateContainer::StateImplementation>> validStates;
};
