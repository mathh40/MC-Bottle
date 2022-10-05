#include "EntityBoat.h"

#include "DamageSource.h"
#include "EntityDamageSourceIndirect.h"

EntityBoat::Type EntityBoat::Type::OAK(BlockPlanks.EnumType.OAK.getMetadata(), "oak");
EntityBoat::Type EntityBoat::Type::SPRUCE(BlockPlanks.EnumType.SPRUCE.getMetadata(), "spruce");
EntityBoat::Type EntityBoat::Type::BIRCH(BlockPlanks.EnumType.BIRCH.getMetadata(), "birch");
EntityBoat::Type EntityBoat::Type::JUNGLE(BlockPlanks.EnumType.JUNGLE.getMetadata(), "jungle");
EntityBoat::Type EntityBoat::Type::ACACIA(BlockPlanks.EnumType.ACACIA.getMetadata(), "acacia");
EntityBoat::Type EntityBoat::Type::DARK_OAK(BlockPlanks.EnumType.DARK_OAK.getMetadata(), "dark_oak");

EntityBoat::EntityBoat(World *worldIn) : Entity(worldIn)
{
    preventEntitySpawning = true;
    setSize(1.375F, 0.5625F);
}

EntityBoat::EntityBoat(World *worldIn, double x, double y, double z) : EntityBoat(worldIn)
{
    setPosition(x, y, z);
    motionX  = 0.0;
    motionY  = 0.0;
    motionZ  = 0.0;
    prevPosX = x;
    prevPosY = y;
    prevPosZ = z;
}

std::optional<AxisAlignedBB> EntityBoat::getCollisionBox(Entity *entityIn)
{
    return entityIn->canBePushed() ? entityIn->getEntityBoundingBox() : std::nullopt;
}

std::optional<AxisAlignedBB> EntityBoat::getCollisionBoundingBox()
{
    return getEntityBoundingBox();
}

bool EntityBoat::canBePushed()
{
    return true;
}

double EntityBoat::getMountedYOffset() const
{
    return -0.1;
}

bool EntityBoat::attackEntityFrom(DamageSource::DamageSource source, float amount)
{
    if (isEntityInvulnerable(source))
    {
        return false;
    }
    else if (!world->isRemote && !isDead)
    {
        if (Util:: instanceof <DamageSource::EntityDamageSourceIndirect>(&source) &&
                                  source.getTrueSource() != nullptr && isPassenger(source.getTrueSource()))
        {
            return false;
        }
        else
        {
            setForwardDirection(-getForwardDirection());
            setTimeSinceHit(10);
            setDamageTaken(getDamageTaken() + amount * 10.0F);
            markVelocityChanged();
            bool flag = Util:: instanceof
                <EntityPlayer>(source.getTrueSource()) &&
                    ((EntityPlayer *)source.getTrueSource())->capabilities.isCreativeMode;
            if (flag || getDamageTaken() > 40.0F)
            {
                if (!flag && world->getGameRules().getBoolean("doEntityDrops"))
                {
                    dropItemWithOffset(getItemBoat(), 1, 0.0F);
                }

                setDead();
            }

            return true;
        }
    }
    else
    {
        return true;
    }
}

void EntityBoat::applyEntityCollision(Entity *entityIn)
{
    if (Util:: instanceof <EntityBoat>(entityIn))
    {
        if (entityIn->getEntityBoundingBox().getminY() < getEntityBoundingBox().getmaxY())
        {
            Entity::applyEntityCollision(entityIn);
        }
    }
    else if (entityIn->getEntityBoundingBox().getminY() <= getEntityBoundingBox().getminY())
    {
        Entity::applyEntityCollision(entityIn);
    }
}

Item *EntityBoat::getItemBoat()
{
    switch (getBoatType())
    {
    case OAK:
    default:
        return Items::BOAT;
    case SPRUCE:
        return Items::SPRUCE_BOAT;
    case BIRCH:
        return Items::BIRCH_BOAT;
    case JUNGLE:
        return Items::JUNGLE_BOAT;
    case ACACIA:
        return Items::ACACIA_BOAT;
    case DARK_OAK:
        return Items::DARK_OAK_BOAT;
    }
}

void EntityBoat::performHurtAnimation()
{
    setForwardDirection(-getForwardDirection());
    setTimeSinceHit(10);
    setDamageTaken(getDamageTaken() * 11.0F);
}

bool EntityBoat::canBeCollidedWith()
{
    return !isDead;
}

void EntityBoat::setPositionAndRotationDirect(double x, double y, double z, float yaw, float pitch,
                                              int32_t posRotationIncrements, bool teleport)
{
    lerpX     = x;
    lerpY     = y;
    lerpZ     = z;
    lerpYaw   = (double)yaw;
    lerpPitch = (double)pitch;
    lerpSteps = 10;
}

EnumFacing EntityBoat::getAdjustedHorizontalFacing() const
{
    return getHorizontalFacing().rotateY();
}

void EntityBoat::onUpdate()
{
    previousStatus = status;
    status         = getBoatStatus();
    if (status != EntityBoat::Status::UNDER_WATER && status != EntityBoat::Status::UNDER_FLOWING_WATER)
    {
        outOfControlTicks = 0.0F;
    }
    else
    {
        ++outOfControlTicks;
    }

    if (!world->isRemote && outOfControlTicks >= 60.0F)
    {
        removePassengers();
    }

    if (getTimeSinceHit() > 0)
    {
        setTimeSinceHit(getTimeSinceHit() - 1);
    }

    if (getDamageTaken() > 0.0F)
    {
        setDamageTaken(getDamageTaken() - 1.0F);
    }

    prevPosX = posX;
    prevPosY = posY;
    prevPosZ = posZ;
    Entity::onUpdate();
    tickLerp();
    if (canPassengerSteer())
    {
        auto passanger = getPassengers();
        if (passanger.empty() || !(Util:: instanceof <EntityPlayer>(passanger[0])))
        {
            setPaddleState(false, false);
        }

        updateMotion();
        if (world->isRemote)
        {
            controlBoat();
            world->sendPacketToServer(new CPacketSteerBoat(getPaddleState(0), getPaddleState(1)));
        }

        move(MoverType::SELF, motionX, motionY, motionZ);
    }
    else
    {
        motionX = 0.0;
        motionY = 0.0;
        motionZ = 0.0;
    }

    for (auto i = 0; i <= 1; ++i)
    {
        if (getPaddleState(i))
        {
            if (!isSilent() && (double)(MathHelper::fmod(paddlePositions[i], 6.2831855F)) <= 0.7853981633974483 &&
                ((double)paddlePositions[i] + 0.39269909262657166) % 6.283185307179586 >= 0.7853981633974483)
            {
                SoundEvent soundevent = getPaddleSound();
                if (soundevent != nullptr)
                {
                    Vec3d vec3d = getLook(1.0F);
                    double d0   = i == 1 ? -vec3d.getz() : vec3d.getz();
                    double d1   = i == 1 ? vec3d.getx() : -vec3d.getx();
                    world->playSound(nullptr, posX + d0, posY, posZ + d1, soundevent, getSoundCategory(), 1.0F,
                                     0.8F + 0.4F * MathHelper::nextFloat(rand));
                }
            }

            paddlePositions[i] = (float)((double)paddlePositions[i] + 0.39269909262657166);
        }
        else
        {
            paddlePositions[i] = 0.0F;
        }
    }

    doBlockCollisions();
    auto list = world->getEntitiesInAABBexcluding(
        this, getEntityBoundingBox().grow(0.20000000298023224, -0.009999999776482582, 0.20000000298023224),
        EntitySelectors::getTeamCollisionPredicate(this));
    if (!list.isEmpty())
    {
        bool flag = !world->isRemote && !(Util:: instanceof <EntityPlayer>(getControllingPassenger()));

        for (auto j = 0; j < list.size(); ++j)
        {
            Entity *entity = list.get(j);
            if (!entity.isPassenger(this))
            {
                if (flag && getPassengers().size() < 2 && !entity.isRiding() && entity.width < width && Util::
                        instanceof <EntityLivingBase>(entity) && !(Util:: instanceof <EntityWaterMob>(entity)) &&
                                       !(Util:: instanceof <EntityPlayer>(entity)))
                {
                    entity.startRiding(this);
                }
                else
                {
                    applyEntityCollision(entity);
                }
            }
        }
    }
}

void EntityBoat::setPaddleState(bool left, bool right)
{
    dataManager.set(DATA_ID_PADDLE[0], left);
    dataManager.set(DATA_ID_PADDLE[1], right);
}

float EntityBoat::getRowingTime(int side, float limbSwing)
{
    return getPaddleState(side) ? (float)MathHelper::clampedLerp((double)paddlePositions[side] - 0.39269909262657166,
                                                                 (double)paddlePositions[side], (double)limbSwing)
                                : 0.0F;
}

float EntityBoat::getWaterLevelAbove()
{
    AxisAlignedBB axisalignedbb = getEntityBoundingBox();
    auto i                      = MathHelper::floor(axisalignedbb.getminX());
    auto j                      = MathHelper::ceil(axisalignedbb.getmaxX());
    auto k                      = MathHelper::floor(axisalignedbb.getmaxY());
    auto l                      = MathHelper::ceil(axisalignedbb.getmaxY() - lastYd);
    auto i1                     = MathHelper::floor(axisalignedbb.getminZ());
    auto j1                     = MathHelper::ceil(axisalignedbb.getmaxZ());
    BlockPos blockpos$pooledmutableblockpos;

    float f;

    for (auto k1 = k; k1 < l; ++k1)
    {
        f = 0.0F;

        for (auto l1 = i; l1 < j; ++l1)
        {
            for (auto i2 = i1; i2 < j1; ++i2)
            {
                blockpos$pooledmutableblockpos.setPos(l1, k1, i2);
                IBlockState *iblockstate = world->getBlockState(blockpos$pooledmutableblockpos);
                if (iblockstate->getMaterial() == Material::WATER)
                {
                    f = MathHelper::max(
                        f, BlockLiquid::getBlockLiquidHeight(iblockstate, world, blockpos$pooledmutableblockpos));
                }

                if (f >= 1.0F)
                {
                    goto label108;
                }
            }
        }

        if (f < 1.0F)
        {
            float f2    = (float)blockpos$pooledmutableblockpos.gety() + f;
            float var19 = f2;
            return var19;
        }
    }
label108:
    float f1 = (float)(l + 1);
    f        = f1;

    return f;
}

float EntityBoat::getBoatGlide()
{
    AxisAlignedBB axisalignedbb = getEntityBoundingBox();
    AxisAlignedBB axisalignedbb1(axisalignedbb.getminX(), axisalignedbb.getminY() - 0.001, axisalignedbb.getminZ(),
                                 axisalignedbb.getmaxX(), axisalignedbb.getminY(), axisalignedbb.getmaxZ());
    auto i  = MathHelper::floor(axisalignedbb1.getminX()) - 1;
    auto j  = MathHelper::ceil(axisalignedbb1.getmaxX()) + 1;
    auto k  = MathHelper::floor(axisalignedbb1.getminY()) - 1;
    auto l  = MathHelper::ceil(axisalignedbb1.getmaxY()) + 1;
    auto i1 = MathHelper::floor(axisalignedbb1.getminZ()) - 1;
    auto j1 = MathHelper::ceil(axisalignedbb1.getmaxZ()) + 1;
    std::optional<std::vector<AxisAlignedBB>> list;
    float f    = 0.0F;
    int32_t k1 = 0;
    BlockPos blockpos$pooledmutableblockpos;
    for (auto l1 = i; l1 < j; ++l1)
    {
        for (auto i2 = i1; i2 < j1; ++i2)
        {
            auto j2 = (l1 != i && l1 != j - 1 ? 0 : 1) + (i2 != i1 && i2 != j1 - 1 ? 0 : 1);
            if (j2 != 2)
            {
                for (auto k2 = k; k2 < l; ++k2)
                {
                    if (j2 <= 0 || k2 != k && k2 != l - 1)
                    {
                        blockpos$pooledmutableblockpos.setPos(l1, k2, i2);
                        IBlockState *iblockstate = world->getBlockState(blockpos$pooledmutableblockpos);
                        iblockstate->addCollisionBoxToList(world, blockpos$pooledmutableblockpos, axisalignedbb1, list,
                                                           this, false);
                        if (!list->empty())
                        {
                            f += iblockstate->getBlock()->slipperiness;
                            ++k1;
                        }

                        list->clear();
                    }
                }
            }
        }
    }

    return f / (float)k1;
}

void EntityBoat::updatePassenger(Entity *passenger)
{
    if (isPassenger(passenger))
    {
        float f        = 0.0F;
        float f1       = (float)((isDead ? 0.009999999776482582 : getMountedYOffset()) + passenger->getYOffset());
        auto passagers = getPassengers();
        if (passagers.size() > 1)
        {
            auto ite = std::find(passagers.begin(), passagers.end(), passenger);
            auto i   = std::distance(passagers.begin(), ite);
            if (i == 0)
            {
                f = 0.2F;
            }
            else
            {
                f = -0.6F;
            }

            if (Util:: instanceof <EntityAnimal>(passenger))
            {
                f = (float)((double)f + 0.2);
            }
        }

        Vec3d vec3d = (Vec3d((double)f, 0.0, 0.0)).rotateYaw(-rotationYaw * 0.017453292F - 1.5707964F);
        passenger->setPosition(posX + vec3d.getx(), posY + (double)f1, posZ + vec3d.getz());
        passenger->rotationYaw += deltaRotation;
        passenger->setRotationYawHead(passenger->getRotationYawHead() + deltaRotation);
        applyYawToEntity(passenger);
        if (Util:: instanceof <EntityAnimal>(passenger) && getPassengers().size() > 1)
        {
            auto j = passenger->getEntityId() % 2 == 0 ? 90 : 270;
            passenger->setRenderYawOffset(((EntityAnimal *)passenger)->renderYawOffset + (float)j);
            passenger->setRotationYawHead(passenger->getRotationYawHead() + (float)j);
        }
    }
}

void EntityBoat::applyOrientationToEntity(Entity *entityToUpdate)
{
    applyYawToEntity(entityToUpdate);
}

bool EntityBoat::processInitialInteract(EntityPlayer *player, EnumHand hand)
{
    if (player->isSneaking())
    {
        return false;
    }
    else
    {
        if (!world->isRemote && outOfControlTicks < 60.0F)
        {
            player->startRiding(this);
        }

        return true;
    }
}

bool EntityBoat::getPaddleState(int32_t side)
{
    return dataManager.get(DATA_ID_PADDLE[side]) && getControllingPassenger() != nullptr;
}

void EntityBoat::setDamageTaken(float damageTaken)
{
    dataManager.set(DAMAGE_TAKEN, damageTaken);
}

float EntityBoat::getDamageTaken()
{
    return dataManager.get(DAMAGE_TAKEN);
}

void EntityBoat::setTimeSinceHit(int32_t timeSinceHit)
{
    dataManager.set(TIME_SINCE_HIT, timeSinceHit);
}

int32_t EntityBoat::getTimeSinceHit()
{
    return dataManager.get(TIME_SINCE_HIT);
}

void EntityBoat::setForwardDirection(int32_t forwardDirection)
{
    dataManager.set(FORWARD_DIRECTION, forwardDirection);
}

int32_t EntityBoat::getForwardDirection()
{
    return dataManager.get(FORWARD_DIRECTION);
}

void EntityBoat::setBoatType(EntityBoat::Type boatType)
{
    dataManager.set(BOAT_TYPE, boatType.ordinal());
}

EntityBoat::Type EntityBoat::getBoatType()
{
    return EntityBoat::Type::byId(dataManager.get(BOAT_TYPE));
}

Entity *EntityBoat::getControllingPassenger()
{
    auto list = getPassengers();
    return list.empty() ? nullptr : list[0];
}

void EntityBoat::updateInputs(bool p_184442_1_, bool p_184442_2_, bool p_184442_3_, bool p_184442_4_)
{
    leftInputDown    = p_184442_1_;
    rightInputDown   = p_184442_2_;
    forwardInputDown = p_184442_3_;
    backInputDown    = p_184442_4_;
}

EntityBoat::Type::Type(int32_t metadataIn, std::string_view nameIn) : name(nameIn), metadata(metadataIn)
{
}

std::string EntityBoat::Type::getName() const
{
    return name;
}

int32_t EntityBoat::Type::getMetadata() const
{
    return metadata;
}

std::string EntityBoat::Type::toString() const
{
    return name;
}

EntityBoat::Type &EntityBoat::Type::byId(int32_t id)
{
    if (id < 0 || id >= values().size())
    {
        id = 0;
    }

    return *values()[id];
}

std::vector<EntityBoat::Type *> EntityBoat::Type::values()
{
    return value;
}

EntityBoat::Type &EntityBoat::Type::getTypeFromString(std::string_view nameIn)
{
    for (auto i = 0; i < values().size(); ++i)
    {
        if (values()[i]->getName() == nameIn)
        {
            return *values()[i];
        }
    }

    return *values()[0];
}

bool EntityBoat::checkInWater()
{
    AxisAlignedBB axisalignedbb = getEntityBoundingBox();
    auto i                      = MathHelper::floor(axisalignedbb.getminX());
    auto j                      = MathHelper::ceil(axisalignedbb.getmaxX());
    auto k                      = MathHelper::floor(axisalignedbb.getminY());
    auto l                      = MathHelper::ceil(axisalignedbb.getminY() + 0.001);
    auto i1                     = MathHelper::floor(axisalignedbb.getminZ());
    auto j1                     = MathHelper::ceil(axisalignedbb.getmaxZ());
    bool flag                   = false;
    waterLevel                  = std::numeric_limits<double>::min();
    BlockPos blockpos$pooledmutableblockpos;

    for (auto k1 = i; k1 < j; ++k1)
    {
        for (auto l1 = k; l1 < l; ++l1)
        {
            for (auto i2 = i1; i2 < j1; ++i2)
            {
                blockpos$pooledmutableblockpos.setPos(k1, l1, i2);
                IBlockState *iblockstate = world->getBlockState(blockpos$pooledmutableblockpos);
                if (iblockstate->getMaterial() == Material::WATER)
                {
                    float f    = BlockLiquid::getLiquidHeight(iblockstate, world, blockpos$pooledmutableblockpos);
                    waterLevel = MathHelper::max((double)f, waterLevel);
                    flag |= axisalignedbb.getminY() < (double)f;
                }
            }
        }
    }

    return flag;
}

std::optional<EntityBoat::Status> EntityBoat::getUnderwaterStatus()
{
    AxisAlignedBB axisalignedbb = getEntityBoundingBox();
    double d0                   = axisalignedbb.getmaxY() + 0.001;
    auto i                      = MathHelper::floor(axisalignedbb.getminX());
    auto j                      = MathHelper::ceil(axisalignedbb.getmaxX());
    auto k                      = MathHelper::floor(axisalignedbb.getmaxY());
    auto l                      = MathHelper::ceil(d0);
    auto i1                     = MathHelper::floor(axisalignedbb.getminZ());
    auto j1                     = MathHelper::ceil(axisalignedbb.getmaxZ());
    bool flag                   = false;
    BlockPos blockpos$pooledmutableblockpos;

    for (auto k1 = i; k1 < j; ++k1)
    {
        for (auto l1 = k; l1 < l; ++l1)
        {
            for (auto i2 = i1; i2 < j1; ++i2)
            {
                blockpos$pooledmutableblockpos.setPos(k1, l1, i2);
                IBlockState *iblockstate = world->getBlockState(blockpos$pooledmutableblockpos);
                if (iblockstate->getMaterial() == Material::WATER &&
                    d0 < (double)BlockLiquid::getLiquidHeight(iblockstate, world, blockpos$pooledmutableblockpos))
                {
                    if (iblockstate->getValue(BlockLiquid::LEVEL) != 0)
                    {
                        EntityBoat::Status entityboat$status = EntityBoat::Status::UNDER_FLOWING_WATER;
                        return entityboat$status;
                    }

                    flag = true;
                }
            }
        }
    }

    return flag ? EntityBoat::Status::UNDER_WATER : std::nullopt;
}

void EntityBoat::updateMotion()
{
    double d0 = -0.03999999910593033;
    double d1 = hasNoGravity() ? 0.0 : -0.03999999910593033;
    double d2 = 0.0;
    momentum  = 0.05F;
    if (previousStatus == EntityBoat::Status::IN_AIR && status != EntityBoat::Status::IN_AIR &&
        status != EntityBoat::Status::ON_LAND)
    {
        waterLevel = getEntityBoundingBox().getminY() + (double)height;
        setPosition(posX, (double)(getWaterLevelAbove() - height) + 0.101, posZ);
        motionY = 0.0;
        lastYd  = 0.0;
        status  = EntityBoat::Status::IN_WATER;
    }
    else
    {
        if (status == EntityBoat::Status::IN_WATER)
        {
            d2       = (waterLevel - getEntityBoundingBox().getminY()) / (double)height;
            momentum = 0.9F;
        }
        else if (status == EntityBoat::Status::UNDER_FLOWING_WATER)
        {
            d1       = -7.0E-4;
            momentum = 0.9F;
        }
        else if (status == EntityBoat::Status::UNDER_WATER)
        {
            d2       = 0.009999999776482582;
            momentum = 0.45F;
        }
        else if (status == EntityBoat::Status::IN_AIR)
        {
            momentum = 0.9F;
        }
        else if (status == EntityBoat::Status::ON_LAND)
        {
            momentum = boatGlide;
            if (Util:: instanceof <EntityPlayer>(getControllingPassenger()))
            {
                boatGlide /= 2.0F;
            }
        }

        motionX *= (double)momentum;
        motionZ *= (double)momentum;
        deltaRotation *= momentum;
        motionY += d1;
        if (d2 > 0.0)
        {
            double d3 = 0.65;
            motionY += d2 * 0.06153846016296973;
            double d4 = 0.75;
            motionY *= 0.75;
        }
    }
}

void EntityBoat::controlBoat()
{
    if (isBeingRidden())
    {
        float f = 0.0F;
        if (leftInputDown)
        {
            deltaRotation += -1.0F;
        }

        if (rightInputDown)
        {
            ++deltaRotation;
        }

        if (rightInputDown != leftInputDown && !forwardInputDown && !backInputDown)
        {
            f += 0.005F;
        }

        rotationYaw += deltaRotation;
        if (forwardInputDown)
        {
            f += 0.04F;
        }

        if (backInputDown)
        {
            f -= 0.005F;
        }

        motionX += (double)(MathHelper::sin(-rotationYaw * 0.017453292F) * f);
        motionZ += (double)(MathHelper::cos(rotationYaw * 0.017453292F) * f);
        setPaddleState(rightInputDown && !leftInputDown || forwardInputDown,
                       leftInputDown && !rightInputDown || forwardInputDown);
    }
}

bool EntityBoat::canTriggerWalking()
{
    return false;
}

void EntityBoat::entityInit()
{
    dataManager.registe(TIME_SINCE_HIT, 0);
    dataManager.registe(FORWARD_DIRECTION, 1);
    dataManager.registe(DAMAGE_TAKEN, 0.0F);
    dataManager.registe(BOAT_TYPE, EntityBoat::Type::OAK.ordinal());

    for (auto dataparameter : DATA_ID_PADDLE)
    {
        dataManager.registe(dataparameter, false);
    }
}

std::optional<SoundEvent> EntityBoat::getPaddleSound()
{
    switch (getBoatStatus())
    {
    case Status::IN_WATER:
    case Status::UNDER_WATER:
    case Status::UNDER_FLOWING_WATER:
        return SoundEvents::ENTITY_BOAT_PADDLE_WATER;
    case Status::ON_LAND:
        return SoundEvents::ENTITY_BOAT_PADDLE_LAND;
    case Status::IN_AIR:
    default:
        return std::nullopt;
    }
}

void EntityBoat::applyYawToEntity(Entity *entityToUpdate) const
{
    entityToUpdate->setRenderYawOffset(rotationYaw);
    float f  = MathHelper::wrapDegrees(entityToUpdate->rotationYaw - rotationYaw);
    float f1 = MathHelper::clamp(f, -105.0F, 105.0F);
    entityToUpdate->prevRotationYaw += f1 - f;
    entityToUpdate->rotationYaw += f1 - f;
    entityToUpdate->setRotationYawHead(entityToUpdate->rotationYaw);
}

void EntityBoat::writeEntityToNBT(NBTTagCompound *compound)
{
    compound->setString("Type", getBoatType().getName());
}

void EntityBoat::readEntityFromNBT(NBTTagCompound *compound)
{
    if (compound->hasKey("Type", 8))
    {
        setBoatType(EntityBoat::Type::getTypeFromString(compound->getString("Type")));
    }
}

void EntityBoat::updateFallState(double y, bool onGroundIn, IBlockState *state, const BlockPos &pos)
{
    lastYd = motionY;
    if (!isRiding())
    {
        if (onGroundIn)
        {
            if (fallDistance > 3.0F)
            {
                if (status != EntityBoat::Status::ON_LAND)
                {
                    fallDistance = 0.0F;
                    return;
                }

                fall(fallDistance, 1.0F);
                if (!world->isRemote && !isDead)
                {
                    setDead();
                    if (world->getGameRules().getBoolean("doEntityDrops"))
                    {
                        int32_t j;
                        for (j = 0; j < 3; ++j)
                        {
                            entityDropItem(
                                ItemStack(Item::getItemFromBlock(Blocks::PLANKS), 1, getBoatType().getMetadata()),
                                0.0F);
                        }

                        for (j = 0; j < 2; ++j)
                        {
                            dropItemWithOffset(Items::STICK, 1, 0.0F);
                        }
                    }
                }
            }

            fallDistance = 0.0F;
        }
        else if (world->getBlockState((new BlockPos(this)).down()).getMaterial() != Material.WATER && y < 0.0D)
        {
            fallDistance = (float)((double)fallDistance - y);
        }
    }
}

bool EntityBoat::canFitPassenger(Entity *passenger)
{
    return getPassengers().size() < 2;
}

void EntityBoat::tickLerp()
{
    if (lerpSteps > 0 && !canPassengerSteer())
    {
        double d0     = posX + (lerpX - posX) / (double)lerpSteps;
        double d1     = posY + (lerpY - posY) / (double)lerpSteps;
        double d2     = posZ + (lerpZ - posZ) / (double)lerpSteps;
        double d3     = MathHelper::wrapDegrees(lerpYaw - (double)rotationYaw);
        rotationYaw   = (float)((double)rotationYaw + d3 / (double)lerpSteps);
        rotationPitch = (float)((double)rotationPitch + (lerpPitch - (double)rotationPitch) / (double)lerpSteps);
        --lerpSteps;
        setPosition(d0, d1, d2);
        setRotation(rotationYaw, rotationPitch);
    }
}

EntityBoat::Status EntityBoat::getBoatStatus()
{
    EntityBoat::Status entityboat$status = getUnderwaterStatus();
    if (entityboat$status != nullptr)
    {
        waterLevel = getEntityBoundingBox().getmaxY();
        return entityboat$status;
    }
    else if (checkInWater())
    {
        return EntityBoat::Status::IN_WATER;
    }
    else
    {
        float f = getBoatGlide();
        if (f > 0.0F)
        {
            boatGlide = f;
            return EntityBoat::Status::ON_LAND;
        }
        else
        {
            return EntityBoat::Status::IN_AIR;
        }
    }
}
