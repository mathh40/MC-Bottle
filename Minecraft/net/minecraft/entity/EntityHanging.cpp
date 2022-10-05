#include "EntityHanging.h"

#include "DamageSource.h"

auto IS_HANGING_ENTITY = [](Entity *p_apply_1_) -> bool { return Util:: instanceof <EntityHanging>(p_apply_1_); };

EntityHanging::EntityHanging(World *worldIn) : Entity(worldIn)
{
    setSize(0.5F, 0.5F);
}

EntityHanging::EntityHanging(World *worldIn, BlockPos hangingPositionIn) : Entity(worldIn)
{
    hangingPosition = hangingPositionIn;
}

void EntityHanging::onUpdate()
{
    prevPosX = posX;
    prevPosY = posY;
    prevPosZ = posZ;
    if (tickCounter1++ == 100 && !world->isRemote)
    {
        tickCounter1 = 0;
        if (!isDead && !onValidSurface())
        {
            setDead();
            onBroken(nullptr);
        }
    }
}

bool EntityHanging::onValidSurface()
{
    if (!world->getCollisionBoxes(this, getEntityBoundingBox()).isEmpty())
    {
        return false;
    }
    else
    {
        auto i                      = MathHelper::max(1, getWidthPixels() / 16);
        auto j                      = MathHelper::max(1, getHeightPixels() / 16);
        const BlockPos blockpos     = hangingPosition.offset(facingDirection->getOpposite());
        const EnumFacing enumfacing = facingDirection->rotateYCCW();
        BlockPos blockpos$mutableblockpos;

        for (auto k = 0; k < i; ++k)
        {
            for (auto l = 0; l < j; ++l)
            {
                auto i1 = (i - 1) / -2;
                auto j1 = (j - 1) / -2;
                blockpos$mutableblockpos.setPos(blockpos).move(enumfacing, k + i1).move(EnumFacing::UP, l + j1);
                auto iblockstate = world->getBlockState(blockpos$mutableblockpos);
                if (!iblockstate->getMaterial().isSolid() && !BlockRedstoneDiode::isDiode(iblockstate))
                {
                    return false;
                }
            }
        }

        return world->getEntitiesInAABBexcluding(this, getEntityBoundingBox(), IS_HANGING_ENTITY).isEmpty();
    }
}

bool EntityHanging::canBeCollidedWith()
{
    return true;
}

bool EntityHanging::hitByEntity(Entity *entityIn)
{
    return Util:: instanceof
        <EntityPlayer>(entityIn) ? attackEntityFrom(DamageSource::causePlayerDamage((EntityPlayer *)entityIn), 0.0F)
                                 : false;
}

EnumFacing EntityHanging::getHorizontalFacing() const
{
    return facingDirection.value_or(EnumFacing::UP);
}

bool EntityHanging::attackEntityFrom(DamageSource::DamageSource source, float amount)
{
    if (isEntityInvulnerable(source))
    {
        return false;
    }
    else
    {
        if (!isDead && !world->isRemote)
        {
            setDead();
            markVelocityChanged();
            onBroken(source.getTrueSource());
        }

        return true;
    }
}

void EntityHanging::move(const MoverType &type, double x, double y, double z)
{
    if (!world->isRemote && !isDead && x * x + y * y + z * z > 0.0)
    {
        setDead();
        onBroken(nullptr);
    }
}

void EntityHanging::addVelocity(double x, double y, double z)
{
    if (!world->isRemote && !isDead && x * x + y * y + z * z > 0.0)
    {
        setDead();
        onBroken(nullptr);
    }
}

void EntityHanging::writeEntityToNBT(NBTTagCompound *compound)
{
    compound->setByte("Facing", facingDirection.getHorizontalIndex());
    BlockPos blockpos = getHangingPosition();
    compound->setInteger("TileX", blockpos.getx());
    compound->setInteger("TileY", blockpos.gety());
    compound->setInteger("TileZ", blockpos.getz());
}

void EntityHanging::readEntityFromNBT(NBTTagCompound *compound)
{
    hangingPosition =
        BlockPos(compound->getInteger("TileX"), compound->getInteger("TileY"), compound->getInteger("TileZ"));
    updateFacingWithBoundingBox(EnumFacing::byHorizontalIndex(compound->getByte("Facing")));
}

EntityItem *EntityHanging::entityDropItem(ItemStack stack, float offsetY)
{
    EntityItem *entityitem =
        new EntityItem(world, posX + (double)((float)facingDirection.getXOffset() * 0.15F), posY + (double)offsetY,
                       posZ + (double)((float)facingDirection.getZOffset() * 0.15F), stack);
    entityitem->setDefaultPickupDelay();
    world->spawnEntity(entityitem);
    return entityitem;
}

void EntityHanging::setPosition(double x, double y, double z)
{
    hangingPosition = BlockPos(x, y, z);
    updateBoundingBox();
    isAirBorne = true;
}

BlockPos EntityHanging::getHangingPosition() const
{
    return hangingPosition;
}

float EntityHanging::getRotatedYaw(Rotation transformRotation)
{
    if (facingDirection != std::nullopt && facingDirection->getAxis() != Axis::Y)
    {
        switch (transformRotation)
        {
        case Rotation::CLOCKWISE_180:
            facingDirection = facingDirection->getOpposite();
            break;
        case Rotation::COUNTERCLOCKWISE_90:
            facingDirection = facingDirection->rotateYCCW();
            break;
        case Rotation::CLOCKWISE_90:
            facingDirection = facingDirection->rotateY();
        }
    }

    float f = MathHelper::wrapDegrees(rotationYaw);
    switch (transformRotation)
    {
    case Rotation::CLOCKWISE_180:
        return f + 180.0F;
    case Rotation::COUNTERCLOCKWISE_90:
        return f + 90.0F;
    case Rotation::CLOCKWISE_90:
        return f + 270.0F;
    default:
        return f;
    }
}

float EntityHanging::getMirroredYaw(Mirror transformMirror)
{
    return getRotatedYaw(transformMirror.toRotation(facingDirection));
}

void EntityHanging::onStruckByLightning(EntityLightningBolt *lightningBolt)
{
}

void EntityHanging::entityInit()
{
}

void EntityHanging::updateFacingWithBoundingBox(EnumFacing facingDirectionIn)
{
    assert(facingDirectionIn.getAxis().isHorizontal());
    facingDirection = facingDirectionIn;
    rotationYaw     = (float)(facingDirection->getHorizontalIndex() * 90);
    prevRotationYaw = rotationYaw;
    updateBoundingBox();
}

void EntityHanging::updateBoundingBox()
{
    if (facingDirection != std::nullopt)
    {
        double d0 = (double)hangingPosition.getx() + 0.5;
        double d1 = (double)hangingPosition.gety() + 0.5;
        double d2 = (double)hangingPosition.getz() + 0.5;
        double d3 = 0.46875;
        double d4 = offs(getWidthPixels());
        double d5 = offs(getHeightPixels());
        d0 -= (double)facingDirection.getXOffset() * 0.46875;
        d2 -= (double)facingDirection.getZOffset() * 0.46875;
        d1 += d5;
        EnumFacing enumfacing = facingDirection.rotateYCCW();
        d0 += d4 * (double)enumfacing.getXOffset();
        d2 += d4 * (double)enumfacing.getZOffset();
        posX      = d0;
        posY      = d1;
        posZ      = d2;
        double d6 = (double)getWidthPixels();
        double d7 = (double)getHeightPixels();
        double d8 = (double)getWidthPixels();
        if (facingDirection.getAxis() == Axis::Z)
        {
            d8 = 1.0;
        }
        else
        {
            d6 = 1.0;
        }

        d6 /= 32.0;
        d7 /= 32.0;
        d8 /= 32.0;
        setEntityBoundingBox(AxisAlignedBB(d0 - d6, d1 - d7, d2 - d8, d0 + d6, d1 + d7, d2 + d8));
    }
}

bool EntityHanging::shouldSetPosAfterLoading()
{
    return false;
}

double EntityHanging::offs(int32_t p_190202_1_)
{
    return p_190202_1_ % 32 == 0 ? 0.5 : 0.0;
}
