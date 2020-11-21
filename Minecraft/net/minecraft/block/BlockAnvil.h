#pragma once

#include "../world/IInteractionObject.h"
#include "BlockFalling.h"
#include "properties/PropertyDirection.h"
#include "properties/PropertyInteger.h"
class BlockStateContainer;

class BlockAnvil : public BlockFalling {
public:
    static PropertyDirection FACING;
    static PropertyInteger DAMAGE;

    bool isFullCube(IBlockState *state) override;
    BlockFaceShape getBlockFaceShape(IBlockAccess *worldIn, IBlockState *state, const BlockPos& pos, EnumFacing face) override;
    bool isOpaqueCube(IBlockState *state) override;
    IBlockState *getStateForPlacement(World *worldIn, const BlockPos& pos , EnumFacing facing, float hitX, float hitY,
                                      float hitZ, int32_t meta, EntityLivingBase* placer) override;
    bool onBlockActivated(World *worldIn, const BlockPos& pos, IBlockState *state, EntityPlayer* playerIn, EnumHand hand,
                          EnumFacing facing, float hitX, float hitY, float hitZ) override;
    int32_t damageDropped(IBlockState *state) override;
    AxisAlignedBB& getBoundingBox(IBlockState *state, IBlockAccess *source, const BlockPos& pos) override;
    void getSubBlocks(CreativeTabs itemIn, std::vector<ItemStack> &items) override;
    void onEndFalling(World *worldIn, const BlockPos& pos, IBlockState *fallingState, IBlockState *hitState) override;
    void onBroken(World *worldIn, const BlockPos& pos) override;
    bool shouldSideBeRendered(IBlockState *blockState, IBlockAccess *blockAccess, const BlockPos& pos,
                              EnumFacing side) override;
    IBlockState *getStateFromMeta(uint16_t meta) override;
    uint16_t getMetaFromState(IBlockState *state) override;
    IBlockState *withRotation(IBlockState *state, Rotation& rot) override;

    class Anvil : public IInteractionObject {
    public:
        Anvil(World *worldIn, const BlockPos& pos);
        std::string getName() const override;
        bool hasCustomName() const override;
        ITextComponent* getDisplayName() const override;
        Container* createContainer(InventoryPlayer* playerInventory, EntityPlayer* playerIn) const override;
        std::string getGuiID() const override;
    private:
        World *world;
        BlockPos position;
    };

protected:
    static AxisAlignedBB X_AXIS_AABB;
    static AxisAlignedBB Z_AXIS_AABB;
    static std::shared_ptr<spdlog::logger> LOGGER;

    BlockAnvil();

    void onStartFalling(EntityFallingBlock *fallingEntity) override;
    BlockStateContainer &createBlockState() override;
private:
};
