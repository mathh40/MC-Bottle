#include "BlockAnvil.h"
#include "../../../../spdlog/include/spdlog/spdlog.h"
#include "../inventory/ContainerRepair.h"
#include "../util/EnumFacing.h"
#include "../util/Util.h"
#include "../util/text/TextComponentTranslation.h"


std::shared_ptr<spdlog::logger> BlockAnvil::LOGGER = spdlog::get("Minecraft")->clone("Registry");
PropertyDirection BlockAnvil::FACING = BlockHorizontal::FACING;
PropertyInteger BlockAnvil::DAMAGE = PropertyInteger::create("damage", 0, 2);
AxisAlignedBB BlockAnvil::X_AXIS_AABB(0.0, 0.0, 0.125, 1.0, 1.0, 0.875);
AxisAlignedBB BlockAnvil::Z_AXIS_AABB(0.125, 0.0, 0.0, 0.875, 1.0, 1.0);


BlockAnvil::BlockAnvil() :
    BlockFalling(Material::ANVIL) {
    setDefaultState(blockState.getBaseState().withProperty(FACING, EnumFacing::NORTH).withProperty(DAMAGE, 0));
    setLightOpacity(0);
    setCreativeTab(CreativeTabs::DECORATIONS);
}

bool BlockAnvil::isFullCube(IBlockState *state) { return false; }

BlockFaceShape BlockAnvil::getBlockFaceShape(IBlockAccess *worldIn, IBlockState *state, const BlockPos& pos, EnumFacing face) {
    return BlockFaceShape::UNDEFINED;
}

bool BlockAnvil::isOpaqueCube(IBlockState *state) { return false; }

IBlockState BlockAnvil::getStateForPlacement(World *worldIn, const BlockPos& pos, EnumFacing facing, float hitX, float hitY,
                                             float hitZ, int32_t meta, EntityLivingBase *placer) {
    EnumFacing enumfacing = placer->getHorizontalFacing().rotateY();

    try {
        return BlockFalling::getStateForPlacement(worldIn, pos, facing, hitX, hitY, hitZ, meta, placer)
               ->withProperty(FACING, enumfacing)
               ->withProperty(DAMAGE, meta >> 2);
    } catch (IllegalArgumentException var11) {
        if (!worldIn->isRemote) {
            LOGGER->warn("Invalid damage property for anvil at %s. Found %d, must be in [0, 1, 2]", pos, meta >> 2);
            if (Util::instanceof<EntityPlayer>(placer)) {
                placer->sendMessage(
                    TextComponentTranslation("Invalid damage property. Please pick in [0, 1, 2]", new Object[0]));
            }
        }

        return BlockFalling::getStateForPlacement(worldIn, pos, facing, hitX, hitY, hitZ, 0, placer)
               ->withProperty(FACING, enumfacing)
               ->withProperty(DAMAGE, 0);
    }
}

bool BlockAnvil::onBlockActivated(World *worldIn, const BlockPos& pos, IBlockState *state, EntityPlayer *playerIn,
                                  EnumHand hand,
                                  EnumFacing facing, float hitX, float hitY, float hitZ) {
    if (!worldIn->isRemote) {
        playerIn->displayGui(BlockAnvil.Anvil(worldIn, pos));
    }

    return true;
}

int32_t BlockAnvil::damageDropped(IBlockState *state) { return state->getValue(DAMAGE); }

AxisAlignedBB& BlockAnvil::getBoundingBox(IBlockState *state, IBlockAccess *source, const BlockPos& pos) {
    EnumFacing enumfacing = (EnumFacing)state->getValue(BlockAnvil::FACING);
    return enumfacing.getAxis() == Axis::X ? X_AXIS_AABB : Z_AXIS_AABB;
}

void BlockAnvil::getSubBlocks(CreativeTabs itemIn, std::vector<ItemStack> &items) {
    items.emplace_back(this);
    items.emplace_back(this, 1, 1);
    items.emplace_back(this, 1, 2);
}

void BlockAnvil::onStartFalling(EntityFallingBlock* fallingEntity) { fallingEntity->setHurtEntities(true); }

void BlockAnvil::onEndFalling(World *worldIn, const BlockPos& pos, IBlockState *fallingState, IBlockState *hitState) {
    worldIn->playEvent(1031, pos, 0);
}

void BlockAnvil::onBroken(World* worldIn, const BlockPos& pos) { worldIn->playEvent(1029, pos, 0); }

bool BlockAnvil::shouldSideBeRendered(IBlockState* blockState, IBlockAccess* blockAccess, const BlockPos& pos, EnumFacing side) {
    return true;
}

IBlockState* BlockAnvil::getStateFromMeta(uint16_t meta) {
    return getDefaultState()
           ->withProperty(FACING, EnumFacing::byHorizontalIndex(meta & 3))
           .withProperty(DAMAGE, (meta & 15) >> 2);
}

uint16_t BlockAnvil::getMetaFromState(IBlockState* state) {
    auto i = 0;
    i = i | ((EnumFacing)state.getValue(FACING)).getHorizontalIndex();
    i |= state.getValue(DAMAGE) << 2;
    return i;
}

IBlockState* BlockAnvil::withRotation(IBlockState* state, Rotation& rot) {
    return state->getBlock() != this
               ? state
               : state->withProperty(FACING, rot.rotate((EnumFacing)state->getValue(FACING)));
}

std::string BlockAnvil::Anvil::getName() const { return "anvil"; }

bool BlockAnvil::Anvil::hasCustomName() const { return false; }

ITextComponent* BlockAnvil::Anvil::getDisplayName() const {
    return std::make_unique<TextComponentTranslation>(Blocks::ANVIL.getTranslationKey() + ".name");
}

std::string BlockAnvil::Anvil::getGuiID() const { return "minecraft:anvil"; }

BlockAnvil::Anvil::Anvil(World* worldIn, const BlockPos& pos) :
    world(worldIn), position(pos) {
}

Container* BlockAnvil::Anvil::createContainer(InventoryPlayer* playerInventory, EntityPlayer* playerIn) const{
    return ContainerRepair(playerInventory, world, position, playerIn);
}

BlockStateContainer& BlockAnvil::createBlockState() {
    return BlockStateContainer(this,
    {
        FACING, DAMAGE
    }
    )
    ;
}
