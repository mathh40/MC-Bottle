#pragma once
#include "../../util/math/RayTraceResult.h"
#include "../material/Material.h"
#include "BlockFaceShape.h"

#include <EnumBlockRenderType.h>
#include <Mirror.h>
#include <optional>
#include <vector>

class AxisAlignedBB;
class World;
class BlockPos;
class IBlockAccess;
class Entity;
class IBlockState;
class EntityPlayer;

class IBlockProperties
{
  public:
    virtual ~IBlockProperties()                                                                            = default;
    virtual Material &getMaterial()                                                                        = 0;
    virtual bool isFullBlock()                                                                             = 0;
    virtual bool canEntitySpawn(Entity *var1)                                                              = 0;
    virtual uint16_t getLightOpacity()                                                                     = 0;
    virtual uint16_t getLightValue()                                                                       = 0;
    virtual bool isTranslucent()                                                                           = 0;
    virtual bool useNeighborBrightness()                                                                   = 0;
    virtual MapColor &getMapColor(IBlockAccess *var1, const BlockPos &var2)                                = 0;
    virtual IBlockState *withRotation(Rotation &var1)                                                      = 0;
    virtual IBlockState *withMirror(Mirror &var1)                                                          = 0;
    virtual bool isFullCube()                                                                              = 0;
    virtual bool hasCustomBreakingProgress()                                                               = 0;
    virtual EnumBlockRenderType getRenderType()                                                            = 0;
    virtual int32_t getPackedLightmapCoords(IBlockAccess *var1, const BlockPos &var2)                      = 0;
    virtual float getAmbientOcclusionLightValue()                                                          = 0;
    virtual bool isBlockNormalCube()                                                                       = 0;
    virtual bool isNormalCube()                                                                            = 0;
    virtual bool canProvidePower()                                                                         = 0;
    virtual uint32_t getWeakPower(IBlockAccess *var1, const BlockPos &var2, EnumFacing &var3)              = 0;
    virtual bool hasComparatorInputOverride()                                                              = 0;
    virtual uint32_t getComparatorInputOverride(World *var1, const BlockPos &var2)                         = 0;
    virtual float getBlockHardness(World *var1, const BlockPos &var2)                                      = 0;
    virtual float getPlayerRelativeBlockHardness(EntityPlayer *var1, World *var2, const BlockPos &var3)    = 0;
    virtual uint32_t getStrongPower(IBlockAccess *var1, const BlockPos &var2, EnumFacing &var3)            = 0;
    virtual EnumPushReaction getPushReaction()                                                             = 0;
    virtual IBlockState *getActualState(IBlockAccess *var1, const BlockPos &var2)                          = 0;
    virtual AxisAlignedBB &getSelectedBoundingBox(World *var1, const BlockPos &var2)                       = 0;
    virtual bool shouldSideBeRendered(IBlockAccess *var1, const BlockPos &var2, EnumFacing &var3)          = 0;
    virtual bool isOpaqueCube()                                                                            = 0;
    virtual std::optional<AxisAlignedBB> getCollisionBoundingBox(IBlockAccess *var1, const BlockPos &var2) = 0;
    virtual void addCollisionBoxToList(World *var1, const BlockPos &var2, AxisAlignedBB &var3,
                                       std::optional<std::vector<AxisAlignedBB>> &var4, Entity *var5, bool var6) = 0;
    virtual AxisAlignedBB &getBoundingBox(IBlockAccess *var1, const BlockPos &var2)                              = 0;
    virtual RayTraceResult &collisionRayTrace(World *var1, const BlockPos &var2, Vec3d &var3, Vec3d &var4)       = 0;
    virtual bool isTopSolid()                                                                                    = 0;
    virtual Vec3d &getOffset(IBlockAccess *var1, const BlockPos &var2)                                           = 0;
    virtual bool causesSuffocation()                                                                             = 0;
    virtual BlockFaceShape &getBlockFaceShape(IBlockAccess *var1, const BlockPos &var2, EnumFacing &var3)        = 0;
};
