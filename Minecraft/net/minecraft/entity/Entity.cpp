#include "Entity.h"

#include <typeindex>



#include "DamageSource.h"
#include "EntityLivingBase.h"
#include "Explosion.h"
#include "Mirror.h"
#include "ReportedException.h"
#include "Rotation.h"
#include "SoundCategory.h"
#include "World.h"
#include "WorldProvider.h"
#include "WorldServer.h"
#include "../../../../spdlog/include/spdlog/spdlog-inl.h"
#include "../../../../spdlog/include/spdlog/fmt/bundled/format.h"
#include "../item/ItemStack.h"
#include "../nbt/NBTTagDouble.h"
#include "../nbt/NBTTagFloat.h"
#include "../nbt/NBTTagString.h"
#include "../potion/PotionUtils.h"
#include "../scoreboard/ScorePlayerTeam.h"
#include "../scoreboard/Team.h"
#include "../util/text/TextComponentString.h"
#include "datafix/DataFixer.h"
#include "datafix/FixTypes.h"
#include "datafix/IDataWalker.h"
#include "math/MathHelper.h"
#include "math/Vec2f.h"
#include "text/TextComponentString.h"
#include "text/translation/I18n.h"

std::shared_ptr<spdlog::logger> Entity::LOGGER = spdlog::get("Minecraft")->clone("EntityAIFindEntityNearestPlayer");

DataParameter Entity::FLAGS = EntityDataManager.createKey(Entity.class, DataSerializers.BYTE);
DataParameter Entity::AIR = EntityDataManager.createKey(Entity.class, DataSerializers.VARINT);
DataParameter Entity::CUSTOM_NAME = EntityDataManager.createKey(Entity.class, DataSerializers.STRING);
DataParameter Entity::CUSTOM_NAME_VISIBLE = EntityDataManager.createKey(Entity.class, DataSerializers.BOOLEAN);
DataParameter Entity::SILENT = EntityDataManager.createKey(Entity.class, DataSerializers.BOOLEAN);
DataParameter Entity::NO_GRAVITY = EntityDataManager.createKey(Entity.class, DataSerializers.BOOLEAN);


class EntityDataWalker :public IDataWalker
{
public:
    NBTTagCompound* process(IDataFixer* fixer, NBTTagCompound* compound, int32_t versionIn)
    {
        if (compound->hasKey("Passengers", 9)) {
            auto nbttaglist = compound->getTagList("Passengers", 10);

            for(int32_t i = 0; i < nbttaglist->tagCount(); ++i) 
            {
                nbttaglist->set(i, fixer.process(FixTypes::ENTITY, nbttaglist->getCompoundTagAt(i), versionIn));
            }
        }

        return compound;
    }
};


Entity::Entity(World *worldIn)
    :entityId(nextEntityID++),boundingBox(ZERO_AABB),width(0.6F),height(1.8F),nextStepDistance(1),nextFlap(1.0F)
    ,firstUpdate(true),entityUniqueID(MathHelper::getRandomUUID(rand)),cachedUniqueIdString(entityUniqueID.str())
    ,pistonDeltas({0.0, 0.0, 0.0}),world(worldIn)
{
    fire = -getFireImmuneTicks();
    setPosition(0.0, 0.0, 0.0);

    if (worldIn != nullptr) 
    {
        dimension = worldIn->provider->getDimensionType().getId();
    }

    dataManager = EntityDataManager(this);
    dataManager.registers(FLAGS, 0);
    dataManager.registers(AIR, 300);
    dataManager.registers(CUSTOM_NAME_VISIBLE, false);
    dataManager.registers(CUSTOM_NAME, "");
    dataManager.registers(SILENT, false);
    dataManager.registers(NO_GRAVITY, false);
    entityInit();
}

int32_t Entity::getEntityId() const
{
    return entityId;
}

void Entity::setEntityId(int32_t id)
{
    entityId = id;
}

std::unordered_set<std::string> Entity::getTags() const
{
    return tags;
}

bool Entity::addTag(std::string_view tag)
{
    if (tags.size() >= 1024) 
    {
        return false;
    }
    else 
    {
        tags.emplace(tag);
        return true;
    }
}

bool Entity::removeTag(std::string_view tag)
{
    return tags.erase(tag);
}

void Entity::onKillCommand()
{
    setDead();
}

EntityDataManager Entity::getDataManager()
{
    return dataManager;
}

void Entity::setDead()
{
    isDead = true;
}

void Entity::setDropItemsWhenDead(bool dropWhenDead)
{
}

void Entity::setPosition(double x, double y, double z)
{
    posX = x;
    posY = y;
    posZ = z;
    float f = width / 2.0F;
    float f1 = height;
    setEntityBoundingBox(AxisAlignedBB(x - f, y, z - f, x + f, y + f1, z + f));
}

void Entity::turn(float yaw, float pitch)
{
    float f = rotationPitch;
    float f1 = rotationYaw;
    rotationYaw = rotationYaw + yaw * 0.15;
    rotationPitch = rotationPitch - pitch * 0.15;
    rotationPitch = MathHelper::clamp(rotationPitch, -90.0F, 90.0F);
    prevRotationPitch += rotationPitch - f;
    prevRotationYaw += rotationYaw - f1;
    if (ridingEntity != nullptr) 
    {
        ridingEntity->applyOrientationToEntity(this);
    }
}

void Entity::onUpdate()
{
    if (!world->isRemote) 
    {
        setFlag(6, isGlowing());
    }

    onEntityUpdate();
}

 void Entity::notifyDataManagerChange(DataParameter key)
{
    
}

EnumFacing Entity::getHorizontalFacing() const
{
    return EnumFacing::byHorizontalIndex(MathHelper::floor((rotationYaw * 4.0F / 360.0F) + 0.5) & 3);
}

EnumFacing Entity::getAdjustedHorizontalFacing() const
{
    return getHorizontalFacing();
}

bool Entity::isSpectatedByPlayer(EntityPlayerMP *player)
{
    return true;
}

AxisAlignedBB Entity::getEntityBoundingBox() const
{
    return boundingBox;
}

AxisAlignedBB Entity::getRenderBoundingBox() const
{
    return getEntityBoundingBox();
}

void Entity::setEntityBoundingBox(AxisAlignedBB bb)
{
    boundingBox = bb;
}

float Entity::getEyeHeight() const
{
    return height * 0.85F;
}

bool Entity::isOutsideBorder() const
{
    return OutsideBorder;
}

void Entity::setOutsideBorder(bool outsideBorder)
{
    OutsideBorder = outsideBorder;
}

bool Entity::replaceItemInInventory(int32_t inventorySlot, ItemStack itemStackIn)
{
    return false;
}

void Entity::sendMessage(ITextComponent *component)
{

}

bool Entity::canUseCommand(int32_t permLevel, std::string_view commandName)
{
    return true;
}

BlockPos Entity::getPosition()
{
    return BlockPos(posX, posY + 0.5, posZ);
}

Vec3d Entity::getPositionVector()
{
    return Vec3d(posX, posY, posZ);
}

World * Entity::getEntityWorld()
{
    return world;
}

Entity * Entity::getCommandSenderEntity()
{
    return this;
}

bool Entity::sendCommandFeedback()
{
    return false;
}

void Entity::setCommandStat(const CommandResultStatsType &type, int32_t amount)
{
    if (world != nullptr && !world->isRemote) 
    {
        cmdResultStats.setCommandStatForSender(world->getMinecraftServer(), this, type, amount);
    }
}

MinecraftServer * Entity::getServer()
{
    return world->getMinecraftServer();
}

CommandResultStats Entity::getCommandStats() const
{
    return cmdResultStats;
}

void Entity::setCommandStats(Entity *entityIn)
{
    cmdResultStats.addAllStats(entityIn->getCommandStats());
}

EnumActionResult Entity::applyPlayerInteraction(EntityPlayer *player, Vec3d vec, EnumHand hand)
{
    return EnumActionResult::PASS;
}

bool Entity::isImmuneToExplosions()
{
    return false;
}

void Entity::addTrackingPlayer(EntityPlayerMP *player)
{

}

void Entity::removeTrackingPlayer(EntityPlayerMP *player)
{

}

float Entity::getRotatedYaw(Rotation transformRotation) const
{
    float f = MathHelper::wrapDegrees(rotationYaw);
    switch(transformRotation)
    {
    case Rotation::CLOCKWISE_180:
        return f + 180.0F;
    case Rotation::COUNTERCLOCKWISE_90:
        return f + 270.0F;
    case Rotation::CLOCKWISE_90:
        return f + 90.0F;
    default:
        return f;
    }
}

float Entity::getMirroredYaw(Mirror transformMirror) const
{
    float f = MathHelper::wrapDegrees(rotationYaw);
    switch(transformMirror)
    {
    case Mirror::LEFT_RIGHT:
        return -f;
    case Mirror::FRONT_BACK:
        return 180.0F - f;
    default:
        return f;
    }
}

bool Entity::ignoreItemEntityData()
{
    return false;
}

bool Entity::setPositionNonDirty()
{
    bool flag = isPositionDirty;
    isPositionDirty = false;
    return flag;
}

Entity * Entity::getControllingPassenger()
{
    return nullptr;
}

std::vector<Entity *> Entity::getPassengers() const
{
    return (riddenByEntities.empty() ? std::vector<Entity *>{} : riddenByEntities);
}

bool Entity::isPassenger(Entity *entityIn) const
{
    if(getPassengers().empty())
    {
        return false;
    }

    for(auto entity : getPassengers())
    {
        if (entity == entityIn) 
        {
            return true;
        }
    }

    return false;
}

std::unordered_set<Entity *> Entity::getRecursivePassengers() const
{
    std::unordered_set<Entity *> set;
    getRecursivePassengersByType(typeid(Entity), set);
    return set;
}

std::unordered_set<Entity *> Entity::getRecursivePassengersByType(std::type_index entityClass) const
{
    std::unordered_set<Entity *> set;
    getRecursivePassengersByType(entityClass, set);
    return set;
}

Entity * Entity::getLowestRidingEntity()
{
    Entity* entity;
    for(entity = this; entity->isRiding(); entity = entity->getRidingEntity()) 
    {
    }

    return entity;
}

bool Entity::isRidingSameEntity(Entity *entityIn)
{
    return getLowestRidingEntity() == entityIn->getLowestRidingEntity();
}

bool Entity::isRidingOrBeingRiddenBy(Entity *entityIn) const
{
    const auto var2 = getPassengers().begin();

    Entity* entity;
    do 
    {
        if (var2 == getPassengers().end()) 
        {
            return false;
        }

        entity = *var2;
        if (entity == entityIn) 
        {
            return true;
        }
    } while(!entity->isRidingOrBeingRiddenBy(entityIn));

    return true;
}

bool Entity::canPassengerSteer()
{
    Entity* entity = getControllingPassenger();
    if (Util::instanceof<EntityPlayer>(entity)) 
    {
        return ((EntityPlayer*)entity)->isUser();
    }
    else 
    {
        return !world->isRemote;
    }
}

Entity * Entity::getRidingEntity() const
{
    return ridingEntity;
}

EnumPushReaction Entity::getPushReaction()
{
    return EnumPushReaction::NORMAL;
}

SoundCategory Entity::getSoundCategory()
{
    return SoundCategory::NEUTRAL;
}

void Entity::getRecursivePassengersByType(std::type_index entityClass, std::unordered_set<Entity *> theSet) const
{
    Entity* entity;
    for(auto ite = getPassengers().begin(); ite != getPassengers().end(); entity->getRecursivePassengersByType(entityClass, theSet)) 
    {
        entity = *ite;
        if (entityClass == typeid(entity)) 
        {
            theSet.emplace(entity);
        }
    }
}

void Entity::onEntityUpdate()
{
    world->profiler.startSection("entityBaseTick");
    if (isRiding() && getRidingEntity().isDead) 
    {
        dismountRidingEntity();
    }

    if (rideCooldown > 0) 
    {
        --rideCooldown;
    }

    prevDistanceWalkedModified = distanceWalkedModified;
    prevPosX = posX;
    prevPosY = posY;
    prevPosZ = posZ;
    prevRotationPitch = rotationPitch;
    prevRotationYaw = rotationYaw;
    if (!world->isRemote && Util::instanceof<WorldServer>(world)) 
    {
        world->profiler.startSection("portal");
        if (inPortal) 
        {
            MinecraftServer* minecraftserver = world->getMinecraftServer();
            if (minecraftserver->getAllowNether()) 
            {
                if (!isRiding()) 
                {
                    int32_t i = getMaxInPortalTime();
                    if (portalCounter++ >= i) 
                    {
                        portalCounter = i;
                        timeUntilPortal = getPortalCooldown();
                        uint8_t j;
                        if (world->provider->getDimensionType().getId() == -1) 
                        {
                            j = 0;
                        }
                        else 
                        {
                            j = -1;
                        }

                        changeDimension(j);
                    }
                }

                inPortal = false;
            }
        }
        else 
        {
            if (portalCounter > 0) 
            {
                portalCounter -= 4;
            }

            if (portalCounter < 0) 
            {
                ortalCounter = 0;
            }
        }

        decrementTimeUntilPortal();
        world->profiler.endSection();
    }

    spawnRunningParticles();
    handle_water_movement();
    if (world->isRemote) 
    {
        extinguish();
    }
    else if (fire > 0) 
    {
        if (bisImmuneToFire) 
        {
            fire -= 4;
            if (fire < 0) 
            {
                extinguish();
            }
        }
        else 
        {
            if (fire % 20 == 0) 
            {
                attackEntityFrom(DamageSource::DamageSource::ON_FIRE, 1.0F);
            }

            --fire;
        }
    }

    if (isInLava()) 
    {
        setOnFireFromLava();
        fallDistance *= 0.5F;
    }

    if (posY < -64.0) 
    {
        outOfWorld();
    }

    if (!world->isRemote) 
    {
        setFlag(0, fire > 0);
    }

    firstUpdate = false;
    world->profiler.endSection();
}

int32_t Entity::getMaxInPortalTime() const
{
    return 1;
}

void Entity::setFire(int32_t seconds)
{
    int32_t i = seconds * 20;
    if (Util::instanceof<EntityLivingBase>(this)) 
    {
        i = EnchantmentProtection::getFireTimeForEntity((EntityLivingBase*)this, i);
    }

    if (fire < i) 
    {
        fire = i;
    }
}

void Entity::extinguish()
{
    fire = 0;
}

bool Entity::isOffsetPositionInLiquid(double x, double y, double z)
{
    AxisAlignedBB axisalignedbb = getEntityBoundingBox().offset(x, y, z);
    return isLiquidPresentInAABB(axisalignedbb);
}

void Entity::move(const MoverType &type, double x, double y, double z)
{
    if (noClip) 
    {
        setEntityBoundingBox(getEntityBoundingBox().offset(x, y, z));
        resetPositionToBB();
    }
    else 
    {
        if (type == MoverType::PISTON) 
        {
            int64_t i = world->getTotalWorldTime();
            if (i != pistonDeltasGameTime) 
            {
                std::fill(pistonDeltas.begin(),pistonDeltas.end(),0.0);
                pistonDeltasGameTime = i;
            }

            int32_t i5 = 0;
            double d13 = 0.0;
            if (x != 0.0) 
            {
                i5 = Axis::X.ordinal();
                d13 = MathHelper::clamp(x + pistonDeltas[i5], -0.51, 0.51);
                x = d13 - pistonDeltas[i5];
                pistonDeltas[i5] = d13;
                if (MathHelper::abs(x) <= 9.999999747378752E-6) 
                {
                    return;
                }
            }
            else if (y != 0.0) 
            {
                i5 = Axis::Y.ordinal();
                d13 = MathHelper::clamp(y + pistonDeltas[i5], -0.51, 0.51);
                y = d13 - pistonDeltas[i5];
                pistonDeltas[i5] = d13;
                if (MathHelper::abs(y) <= 9.999999747378752E-6) 
                {
                    return;
                }
            }
            else 
            {
                if (z == 0.0) 
                {
                    return;
                }

                i5 = Axis::Z.ordinal();
                d13 = MathHelper::clamp(z + pistonDeltas[i5], -0.51, 0.51);
                z = d13 - pistonDeltas[i5];
                pistonDeltas[i5] = d13;
                if (MathHelper::abs(z) <= 9.999999747378752E-6) 
                {
                    return;
                }
            }
        }

        world->profiler.startSection("move");
        double d10 = posX;
        double d11 = posY;
        double d1 = posZ;
        if (isInWeb) 
        {
            isInWeb = false;
            x *= 0.25;
            y *= 0.05000000074505806;
            z *= 0.25;
            motionX = 0.0;
            motionY = 0.0;
            motionZ = 0.0;
        }
        
        double d2 = x;
        double d3 = y;
        double d4 = z;
        if ((type == MoverType::SELF || type == MoverType::PLAYER) && onGround && isSneaking() && Util::instanceof<EntityPlayer>(this)) 
        {
            for(double var20 = 0.05; x != 0.0 && world->getCollisionBoxes(this, getEntityBoundingBox().offset(x, -stepHeight, 0.0))->empty(); d2 = x) 
            {
                if (x < 0.05 && x >= -0.05) 
                {
                    x = 0.0;
                }
                else if (x > 0.0) 
                {
                    x -= 0.05;
                }
                else 
                {
                    x += 0.05;
                }
            }

            for(; z != 0.0 && world->getCollisionBoxes(this, getEntityBoundingBox().offset(0.0, -stepHeight, z))->empty(); d4 = z) 
            {
                if (z < 0.05 && z >= -0.05) 
                {
                    z = 0.0;
                }
                else if (z > 0.0) 
                {
                    z -= 0.05;
                }
                else 
                {
                    z += 0.05;
                }
            }

            for(; x != 0.0 && z != 0.0 && world->getCollisionBoxes(this, getEntityBoundingBox().offset(x, -stepHeight, z))->empty(); d4 = z) 
            {
                if (x < 0.05 && x >= -0.05) 
                {
                    x = 0.0;
                }
                else if (x > 0.0) 
                {
                    x -= 0.05;
                }
                else 
                {
                    x += 0.05;
                }

                d2 = x;
                if (z < 0.05 && z >= -0.05) 
                {
                    z = 0.0;
                } 
                else if (z > 0.0) 
                {
                    z -= 0.05;
                }
                else 
                {
                    z += 0.05;
                }
            }
        }

        auto list1 = world->getCollisionBoxes(this, getEntityBoundingBox().expand(x, y, z));
        AxisAlignedBB axisalignedbb = getEntityBoundingBox();
        int32_t k5 = 0;
        int32_t j6 = 0;

        if (y != 0.0) 
        {
            for(auto boxes : list1.value())
            {
                y = boxes.calculateYOffset(getEntityBoundingBox(), y);
            }

            setEntityBoundingBox(getEntityBoundingBox().offset(0.0, y, 0.0));
        }

        if (x != 0.0) 
        {
            for(auto boxes : list1.value())
            {
                x = boxes.calculateXOffset(getEntityBoundingBox(), x);
            }

            if (x != 0.0) 
            {
                setEntityBoundingBox(getEntityBoundingBox().offset(x, 0.0, 0.0));
            }
        }

        if (z != 0.0) 
        {
            for(auto boxes : list1.value())
            {
                z = boxes.calculateZOffset(getEntityBoundingBox(), z);
            }

            if (z != 0.0) 
            {
                setEntityBoundingBox(getEntityBoundingBox().offset(0.0, 0.0, z));
            }
        }

        bool flag = onGround || y != y && y < 0.0;
        double d8 = 0;
        if (stepHeight > 0.0F && flag && (d2 != x || d4 != z)) 
        {
            double d14 = x;
            double d6 = y;
            double d7 = z;
            AxisAlignedBB axisalignedbb1 = getEntityBoundingBox();
            setEntityBoundingBox(axisalignedbb);
            y = stepHeight;
            auto list = world->getCollisionBoxes(this, getEntityBoundingBox().expand(d2, y, d4)).value();
            AxisAlignedBB axisalignedbb2 = getEntityBoundingBox();
            AxisAlignedBB axisalignedbb3 = axisalignedbb2.expand(d2, 0.0, d4);
            d8 = y;

            for(auto boxes : list)
            {
                d8 = boxes.calculateYOffset(axisalignedbb3, d8);
            }

            axisalignedbb2 = axisalignedbb2.offset(0.0, d8, 0.0);
            double d18 = d2;

            for(auto boxes : list)
            {
                d18 = boxes.calculateXOffset(axisalignedbb2, d18);
            }

            axisalignedbb2 = axisalignedbb2.offset(d18, 0.0, 0.0);
            double d19 = d4;

            for(auto boxes : list)
            {
                d19 = boxes.calculateZOffset(axisalignedbb2, d19);
            }

            axisalignedbb2 = axisalignedbb2.offset(0.0, 0.0, d19);
            AxisAlignedBB axisalignedbb4 = getEntityBoundingBox();
            double d20 = y;

            for(auto boxes : list)
            {
                d20 = boxes.calculateYOffset(axisalignedbb4, d20);
            }

            axisalignedbb4 = axisalignedbb4.offset(0.0, d20, 0.0);
            double d21 = d2;

            for(auto boxes : list)
            {
                d21 = boxes.calculateXOffset(axisalignedbb4, d21);
            }

            axisalignedbb4 = axisalignedbb4.offset(d21, 0.0, 0.0);
            double d22 = d4;

            for(auto boxes : list)
            {
                d22 = boxes.calculateZOffset(axisalignedbb4, d22);
            }

            axisalignedbb4 = axisalignedbb4.offset(0.0, 0.0, d22);
            double d23 = d18 * d18 + d19 * d19;
            double d9 = d21 * d21 + d22 * d22;
            if (d23 > d9) 
            {
                x = d18;
                z = d19;
                y = -d8;
                setEntityBoundingBox(axisalignedbb2);
            }
            else 
            {
                x = d21;
                z = d22;
                y = -d20;
                setEntityBoundingBox(axisalignedbb4);
            }

            for(auto boxes : list)
            {
                y = boxes.calculateYOffset(getEntityBoundingBox(), y);
            }

            setEntityBoundingBox(getEntityBoundingBox().offset(0.0, y, 0.0));
            if (d14 * d14 + d7 * d7 >= x * x + z * z) 
            {
                x = d14;
                y = d6;
                z = d7;
                setEntityBoundingBox(axisalignedbb1);
            }
        }

        world->profiler.endSection();
        world->profiler.startSection("rest");
        resetPositionToBB();
        collidedHorizontally = d2 != x || d4 != z;
        collidedVertically = y != y;
        onGround = collidedVertically && d3 < 0.0;
        collided = collidedHorizontally || collidedVertically;
        j6 = MathHelper::floor(posX);
        int32_t i1 = MathHelper::floor(posY - 0.20000000298023224);
        int32_t k6 = MathHelper::floor(posZ);
        BlockPos blockpos = BlockPos(j6, i1, k6);
        IBlockState* iblockstate = world->getBlockState(blockpos);
        if (iblockstate->getMaterial() == Material::AIR) 
        {
            BlockPos blockpos1 = blockpos.down();
            IBlockState* iblockstate1 = world->getBlockState(blockpos1);
            Block* block1 = iblockstate1.getBlock();
            if (Util::instanceof<BlockFence>(block1) || Util::instanceof<BlockWall>(block1) || Util::instanceof<BlockFenceGate>(block1)) 
            {
                iblockstate = iblockstate1;
                blockpos = blockpos1;
            }
        }

        updateFallState(y, onGround, iblockstate, blockpos);
        if (d2 != x) 
        {
            motionX = 0.0;
        }

        if (d4 != z) 
        {
            motionZ = 0.0;
        }

        Block* block = iblockstate->getBlock();
        if (d3 != y) 
        {
            block->onLanded(world, this);
        }

        if (canTriggerWalking() && (!onGround || !isSneaking() || !
            Util::instanceof<EntityPlayer>(this)) && !isRiding()) 
        {
            double d15 = posX - d10;
            double d16 = posY - d11;
            d8 = posZ - d1;
            if (block != Blocks::LADDER) 
            {
                d16 = 0.0;
            }

            if (block != nullptr && onGround) 
            {
                block->onEntityWalk(world, blockpos, this);
            }

            distanceWalkedModified = distanceWalkedModified + MathHelper::sqrt(d15 * d15 + d8 * d8) * 0.6;
            distanceWalkedOnStepModified = distanceWalkedOnStepModified + MathHelper::sqrt(d15 * d15 + d16 * d16 + d8 * d8) * 0.6;
            if (distanceWalkedOnStepModified > nextStepDistance && iblockstate->getMaterial() != Material::AIR) 
            {
                nextStepDistance = distanceWalkedOnStepModified + 1;
                if (!isInWater()) 
                {
                    playStepSound(blockpos, block);
                }
                else 
                {
                    Entity* entity = isBeingRidden() && getControllingPassenger() != nullptr ? getControllingPassenger() : this;
                    float f = entity == this ? 0.35F : 0.4F;
                    float f1 = MathHelper::sqrt(entity->motionX * entity->motionX * 0.20000000298023224 + entity->motionY * entity->motionY + entity->motionZ * entity->motionZ * 0.20000000298023224) * f;
                    if (f1 > 1.0F) 
                    {
                        f1 = 1.0F;
                    }

                    playSound(getSwimSound(), f1, 1.0F + (MathHelper::nextFloat(rand) - MathHelper::nextFloat(rand)) * 0.4F);
                }
            }
            else if (distanceWalkedOnStepModified > nextFlap && makeFlySound() && iblockstate->getMaterial() == Material::AIR)
            {
                nextFlap = playFlySound(distanceWalkedOnStepModified);
            }
        }

        try 
        {
            doBlockCollisions();
        }
        catch (std::exception& var58) 
        {
            CrashReport crashreport = CrashReport::makeCrashReport(var58, "Checking entity block collision");
            CrashReportCategory crashreportcategory = crashreport.makeCategory("Entity being checked for collision");
            addEntityCrashInfo(crashreportcategory);
            throw ReportedException(crashreport);
        }

        bool flag1 = isWet();
        if (world->isFlammableWithin(getEntityBoundingBox().shrink(0.001))) 
        {
            dealFireDamage(1);
            if (!flag1) 
            {
                ++fire;
                if (fire == 0) 
                {
                    setFire(8);
                }
            }
        }
        else if (fire <= 0) 
        {
            fire = -getFireImmuneTicks();
        }

        if (flag1 && isBurning()) 
        {
            playSound(SoundEvents::ENTITY_GENERIC_EXTINGUISH_FIRE, 0.7F, 1.6F + (MathHelper::nextFloat(rand) - MathHelper::nextFloat(rand)) * 0.4F);
            fire = -getFireImmuneTicks();
        }

        world->profiler.endSection();
    }
}

void Entity::resetPositionToBB()
{
    AxisAlignedBB axisalignedbb = getEntityBoundingBox();
    posX = (axisalignedbb.getminX() + axisalignedbb.getmaxX()) / 2.0;
    posY = axisalignedbb.getminY();
    posZ = (axisalignedbb.getminZ() + axisalignedbb.getmaxZ()) / 2.0;
}

void Entity::playSound(SoundEvent soundIn, float volume, float pitch)
{
    if (!isSilent()) 
    {
        world->playSound(nullptr, posX, posY, posZ, soundIn, getSoundCategory(), volume, pitch);
    }
}

bool Entity::isSilent()
{
    return dataManager.get(SILENT);
}

void Entity::setSilent(bool isSilent)
{
    dataManager.set(SILENT, isSilent);
}

bool Entity::hasNoGravity()
{
    return dataManager.get(NO_GRAVITY);
}

void Entity::setNoGravity(bool noGravity)
{
    dataManager.set(NO_GRAVITY, noGravity);
}

std::optional<AxisAlignedBB> Entity::getCollisionBoundingBox()
{
    return std::nullopt;
}

bool Entity::isImmuneToFire() const
{
    return bisImmuneToFire;
}

void Entity::fall(float distance, float damageMultiplier)
{
    if (isBeingRidden()) 
    {
        for(auto entity : getPassengers())
        {
            entity->fall(distance, damageMultiplier);
        }
    }
}

bool Entity::isWet()
{
    if (inWater) 
    {
        return true;
    }
    else 
    {
        PooledMutableBlockPos blockpos$pooledmutableblockpos = PooledMutableBlockPos::retain(posX, posY, posZ);
        if (!world->isRainingAt(blockpos$pooledmutableblockpos) && !world->isRainingAt(blockpos$pooledmutableblockpos.setPos(posX, posY + height, posZ))) 
        {
            blockpos$pooledmutableblockpos.release();
            return false;
        }
        else 
        {
            blockpos$pooledmutableblockpos.release();
            return true;
        }
    }
}

bool Entity::isInWater()
{
    return inWater;
}

bool Entity::isOverWater()
{
    return world->handleMaterialAcceleration(getEntityBoundingBox().grow(0.0, -20.0, 0.0).shrink(0.001), Material::WATER, this);
}

bool Entity::handle_water_movement()
{
    if (Util::instanceof<EntityBoat>(getRidingEntity())) 
    {
        inWater = false;
    }
    else if (world->handleMaterialAcceleration(getEntityBoundingBox().grow(0.0, -0.4000000059604645, 0.0).shrink(0.001), Material::WATER, this)) 
    {
        if (!inWater && !firstUpdate)
        {
            doWaterSplashEffect();
        }

        fallDistance = 0.0F;
        inWater = true;
        extinguish();
    }
    else 
    {
        inWater = false;
    }

    return inWater;
}

void Entity::spawnRunningParticles()
{
    if (isSprinting() && !isInWater()) 
    {
        createRunningParticles();
    }
}

bool Entity::isInsideOfMaterial(const Material &materialIn)
{
    if (Util::instanceof<EntityBoat>(getRidingEntity())) 
    {
        return false;
    }
    else 
    {
        double d0 = posY + getEyeHeight();
        BlockPos blockpos = BlockPos(posX, d0, posZ);
        IBlockState* iblockstate = world->getBlockState(blockpos);
        if (iblockstate->getMaterial() == materialIn) 
        {
            float f = BlockLiquid::getLiquidHeightPercent(iblockstate->getBlock()->getMetaFromState(iblockstate)) - 0.11111111F;
            float f1 = blockpos.gety() + 1 - f;
            bool flag = d0 < f1;
            return !flag && Util::instanceof<EntityPlayer>(this) ? false : flag;
        }
        else 
        {
            return false;
        }
    }
}

bool Entity::isInLava()
{
    return world->isMaterialInBB(getEntityBoundingBox().grow(-0.10000000149011612, -0.4000000059604645, -0.10000000149011612), Material::LAVA);
}

void Entity::moveRelative(float strafe, float up, float forward, float friction)
{
    float f = strafe * strafe + up * up + forward * forward;
    if (f >= 1.0E-4F) 
    {
        f = MathHelper::sqrt(f);
        if (f < 1.0F) 
        {
            f = 1.0F;
        }

        f = friction / f;
        strafe *= f;
        up *= f;
        forward *= f;
        float f1 = MathHelper::sin(rotationYaw * 0.017453292F);
        float f2 = MathHelper::cos(rotationYaw * 0.017453292F);
        motionX += strafe * f2 - forward * f1;
        motionY += up;
        motionZ += forward * f2 + strafe * f1;
    }
}

int32_t Entity::getBrightnessForRender()
{
    MutableBlockPos blockpos$mutableblockpos = MutableBlockPos(MathHelper::floor(posX), 0, MathHelper::floor(posZ));
    if (world->isBlockLoaded(blockpos$mutableblockpos)) 
    {
        blockpos$mutableblockpos.setY(MathHelper::floor(posY + getEyeHeight()));
        return world->getCombinedLight(blockpos$mutableblockpos, 0);
    }
    else 
    {
        return 0;
    }
}

float Entity::getBrightness()
{
    MutableBlockPos blockpos$mutableblockpos = MutableBlockPos(MathHelper::floor(posX), 0, MathHelper::floor(posZ));
    if (world->isBlockLoaded(blockpos$mutableblockpos)) 
    {
        blockpos$mutableblockpos.setY(MathHelper::floor(posY + getEyeHeight()));
        return world->getLightBrightness(blockpos$mutableblockpos);
    }
    else 
    {
        return 0.0F;
    }
}

void Entity::setWorld(World *worldIn)
{
    world = worldIn;
}

void Entity::setPositionAndRotation(double x, double y, double z, float yaw, float pitch)
{
    posX = MathHelper::clamp(x, -3.0E7, 3.0E7);
    posY = y;
    posZ = MathHelper::clamp(z, -3.0E7, 3.0E7);
    prevPosX = posX;
    prevPosY = posY;
    prevPosZ = posZ;
    pitch = MathHelper::clamp(pitch, -90.0F, 90.0F);
    rotationYaw = yaw;
    rotationPitch = pitch;
    prevRotationYaw = rotationYaw;
    prevRotationPitch = rotationPitch;
    double d0 = prevRotationYaw - yaw;
    if (d0 < -180.0) 
    {
        prevRotationYaw += 360.0F;
    }

    if (d0 >= 180.0) 
    {
        prevRotationYaw -= 360.0F;
    }

    setPosition(posX, posY, posZ);
    setRotation(yaw, pitch);
}

void Entity::moveToBlockPosAndAngles(BlockPos pos, float rotationYawIn, float rotationPitchIn)
{
    setLocationAndAngles(pos.getx() + 0.5, pos.gety(), pos.getz() + 0.5, rotationYawIn, rotationPitchIn);
}

void Entity::setLocationAndAngles(double x, double y, double z, float yaw, float pitch)
{
    posX = x;
    posY = y;
    posZ = z;
    prevPosX = posX;
    prevPosY = posY;
    prevPosZ = posZ;
    lastTickPosX = posX;
    lastTickPosY = posY;
    lastTickPosZ = posZ;
    rotationYaw = yaw;
    rotationPitch = pitch;
    setPosition(posX, posY, posZ);
}

float Entity::getDistance(Entity *entityIn) const
{
    float f = posX - entityIn->posX;
    float f1 = posY - entityIn->posY;
    float f2 = posZ - entityIn->posZ;
    return MathHelper::sqrt(f * f + f1 * f1 + f2 * f2);
}

double Entity::getDistanceSq(double x, double y, double z) const
{
    double d0 = posX - x;
    double d1 = posY - y;
    double d2 = posZ - z;
    return d0 * d0 + d1 * d1 + d2 * d2;
}

double Entity::getDistanceSq(const BlockPos &pos) const
{
    return pos.distanceSq(posX, posY, posZ);
}

double Entity::getDistanceSqToCenter(const BlockPos &pos) const
{
    return pos.distanceSqToCenter(posX, posY, posZ);
}

double Entity::getDistance(double x, double y, double z) const
{
    double d0 = posX - x;
    double d1 = posY - y;
    double d2 = posZ - z;
    return MathHelper::sqrt(d0 * d0 + d1 * d1 + d2 * d2);
}

double Entity::getDistanceSq(Entity *entityIn) const
{
    double d0 = posX - entityIn->posX;
    double d1 = posY - entityIn->posY;
    double d2 = posZ - entityIn->posZ;
    return d0 * d0 + d1 * d1 + d2 * d2;
}

void Entity::onCollideWithPlayer(EntityPlayer *entityIn)
{
}

void Entity::applyEntityCollision(Entity *entityIn)
{
    if (!isRidingSameEntity(entityIn) && !entityIn->noClip && !noClip) 
    {
        double d0 = entityIn->posX - posX;
        double d1 = entityIn->posZ - posZ;
        double d2 = MathHelper::absMax(d0, d1);
        if (d2 >= 0.009999999776482582) 
        {
            d2 = MathHelper::sqrt(d2);
            d0 /= d2;
            d1 /= d2;
            double d3 = 1.0 / d2;
            if (d3 > 1.0) 
            {
                d3 = 1.0;
            }

            d0 *= d3;
            d1 *= d3;
            d0 *= 0.05000000074505806;
            d1 *= 0.05000000074505806;
            d0 *= 1.0F - entityCollisionReduction;
            d1 *= 1.0F - entityCollisionReduction;
            if (!isBeingRidden()) 
            {
                addVelocity(-d0, 0.0, -d1);
            }

            if (!entityIn->isBeingRidden()) 
            {
                entityIn->addVelocity(d0, 0.0, d1);
            }
        }
    }
}

void Entity::addVelocity(double x, double y, double z)
{
    motionX += x;
    motionY += y;
    motionZ += z;
    isAirBorne = true;
}

bool Entity::attackEntityFrom(DamageSource::DamageSource source, float amount)
{
    if (isEntityInvulnerable(source)) 
    {
        return false;
    }
    else 
    {
        markVelocityChanged();
        return false;
    }
}

Vec3d Entity::getLook(float partialTicks)
{
    if (partialTicks == 1.0F) 
    {
        return getVectorForRotation(rotationPitch, rotationYaw);
    }
    else 
    {
        float f = prevRotationPitch + (rotationPitch - prevRotationPitch) * partialTicks;
        float f1 = prevRotationYaw + (rotationYaw - prevRotationYaw) * partialTicks;
        return getVectorForRotation(f, f1);
    }
}

Vec3d Entity::getPositionEyes(float partialTicks)
{
    if (partialTicks == 1.0F) 
    {
        return Vec3d(posX, posY + getEyeHeight(), posZ);
    }
    else 
    {
        double d0 = prevPosX + (posX - prevPosX) * partialTicks;
        double d1 = prevPosY + (posY - prevPosY) * partialTicks + getEyeHeight();
        double d2 = prevPosZ + (posZ - prevPosZ) * partialTicks;
        return Vec3d(d0, d1, d2);
    }
}

std::optional<RayTraceResult> Entity::rayTrace(double blockReachDistance, float partialTicks)
{
    Vec3d vec3d = getPositionEyes(partialTicks);
    Vec3d vec3d1 = getLook(partialTicks);
    Vec3d vec3d2 = vec3d.add(vec3d1.getx() * blockReachDistance, vec3d1.gety() * blockReachDistance, vec3d1.getz() * blockReachDistance);
    return world->rayTraceBlocks(vec3d, vec3d2, false, false, true);
}

bool Entity::canBeCollidedWith()
{
    return false;
}

bool Entity::canBePushed()
{
    return false;
}

void Entity::awardKillScore(Entity *p_191956_1_, int32_t p_191956_2_, DamageSource::DamageSource p_191956_3_)
{
    if (Util::instanceof< EntityPlayerMP>(p_191956_1_)) 
    {
        CriteriaTriggers::ENTITY_KILLED_PLAYER::trigger((EntityPlayerMP*)p_191956_1_, this, p_191956_3_);
    }
}

bool Entity::isInRangeToRender3d(double x, double y, double z)
{
    double d0 = posX - x;
    double d1 = posY - y;
    double d2 = posZ - z;
    double d3 = d0 * d0 + d1 * d1 + d2 * d2;
    return isInRangeToRenderDist(d3);
}

bool Entity::isInRangeToRenderDist(double distance)
{
    double d0 = getEntityBoundingBox().getAverageEdgeLength();
    if (std::isnan(d0)) 
    {
        d0 = 1.0;
    }

    d0 = d0 * 64.0 * renderDistanceWeight;
    return distance < d0 * d0;
}

bool Entity::writeToNBTAtomically(NBTTagCompound *compound)
{
    auto s = getEntityString();
    if (!isDead && s != nullptr) 
    {
        compound->setString("id", s);
        writeToNBT(compound);
        return true;
    }
    else 
    {
        return false;
    }
}

bool Entity::writeToNBTOptional(NBTTagCompound *compound)
{
    auto s = getEntityString();
    if (!isDead && s != nullptr && !isRiding()) 
    {
        compound->setString("id", s);
        writeToNBT(compound);
        return true;
    }
    else 
    {
        return false;
    }
}

void Entity::registerFixes(DataFixer fixer)
{
    fixer.registerWalker(FixTypes::ENTITY, new EntityDataWalker());
}

NBTTagCompound * Entity::writeToNBT(NBTTagCompound *compound)
{
    try 
    {
        compound->setTag("Pos", newDoubleNBTList(posX, posY, posZ));
        compound->setTag("Motion", newDoubleNBTList(motionX, motionY, motionZ));
        compound->setTag("Rotation", newFloatNBTList(rotationYaw, rotationPitch));
        compound->setFloat("FallDistance", fallDistance);
        compound->setShort("Fire", fire);
        compound->setShort("Air", getAir());
        compound->setBoolean("OnGround", onGround);
        compound->setInteger("Dimension", dimension);
        compound->setBoolean("Invulnerable", invulnerable);
        compound->setInteger("PortalCooldown", timeUntilPortal);
        compound->setUniqueId("UUID", getUniqueID());
        if (hasCustomName()) 
        {
            compound->setString("CustomName", getCustomNameTag());
        }

        if (getAlwaysRenderNameTag()) 
        {
            compound->setBoolean("CustomNameVisible", getAlwaysRenderNameTag());
        }

        cmdResultStats.writeStatsToNBT(compound);
        if (isSilent()) 
        {
            compound->setBoolean("Silent", isSilent());
        }

        if (hasNoGravity()) 
        {
            compound->setBoolean("NoGravity", hasNoGravity());
        }

        if (glowing) 
        {
            compound->setBoolean("Glowing", glowing);
        }

        NBTTagList nbttaglist1;
        Iterator var7;
        if (!tags.isEmpty()) {
            nbttaglist1 = NBTTagList();
            var7 = tags.iterator();

            while(var7.hasNext()) {
                auto s = (String)var7.next();
                nbttaglist1.appendTag(new NBTTagString(s));
            }

            compound->setTag("Tags", nbttaglist1);
        }

        writeEntityToNBT(compound);
        if (isBeingRidden()) {
            nbttaglist1 = new NBTTagList();
            var7 = getPassengers().iterator();

            while(var7.hasNext()) 
            {
                Entity* entity = (Entity)var7.next();
                NBTTagCompound* nbttagcompound = new NBTTagCompound();
                if (entity.writeToNBTAtomically(nbttagcompound)) 
                {
                    nbttaglist1.appendTag(nbttagcompound);
                }
            }

            if (!nbttaglist1.isEmpty()) {
                compound->setTag("Passengers", nbttaglist1);
            }
        }

        return compound;
    }
    catch (Throwable var6) 
    {
        CrashReport crashreport = CrashReport.makeCrashReport(var6, "Saving entity NBT");
        CrashReportCategory crashreportcategory = crashreport.makeCategory("Entity being saved");
        addEntityCrashInfo(crashreportcategory);
        throw new ReportedException(crashreport);
    }
}

void Entity::readFromNBT(NBTTagCompound *compound)
{
    try 
    {
        NBTTagList nbttaglist = compound->getTagList("Pos", 6);
        NBTTagList nbttaglist2 = compound->getTagList("Motion", 6);
        NBTTagList nbttaglist3 = compound->getTagList("Rotation", 5);
        motionX = nbttaglist2.getDoubleAt(0);
        motionY = nbttaglist2.getDoubleAt(1);
        motionZ = nbttaglist2.getDoubleAt(2);
        if (MathHelper::abs(motionX) > 10.0)
        {
            motionX = 0.0;
        }

        if (MathHelper::abs(motionY) > 10.0)
        {
            motionY = 0.0;
        }

        if (MathHelper::abs(motionZ) > 10.0) 
        {
            motionZ = 0.0;
        }

        posX = nbttaglist.getDoubleAt(0);
        posY = nbttaglist.getDoubleAt(1);
        posZ = nbttaglist.getDoubleAt(2);
        lastTickPosX = posX;
        lastTickPosY = posY;
        lastTickPosZ = posZ;
        prevPosX = posX;
        prevPosY = posY;
        prevPosZ = posZ;
        rotationYaw = nbttaglist3.getFloatAt(0);
        rotationPitch = nbttaglist3.getFloatAt(1);
        prevRotationYaw = rotationYaw;
        prevRotationPitch = rotationPitch;
        setRotationYawHead(rotationYaw);
        setRenderYawOffset(rotationYaw);
        fallDistance = compound->getFloat("FallDistance");
        fire = compound->getShort("Fire");
        setAir(compound->getShort("Air"));
        onGround = compound->getBoolean("OnGround");
        if (compound->hasKey("Dimension")) 
        {
            dimension = compound->getInteger("Dimension");
        }

        invulnerable = compound->getBoolean("Invulnerable");
        timeUntilPortal = compound->getInteger("PortalCooldown");
        if (compound->hasUniqueId("UUID")) 
        {
            entityUniqueID = compound->getUniqueId("UUID");
            cachedUniqueIdString = entityUniqueID.str();
        }

        setPosition(posX, posY, posZ);
        setRotation(rotationYaw, rotationPitch);
        if (compound->hasKey("CustomName", 8)) 
        {
            setCustomNameTag(compound->getString("CustomName"));
        }

        setAlwaysRenderNameTag(compound->getBoolean("CustomNameVisible"));
        cmdResultStats.readStatsFromNBT(compound);
        setSilent(compound->getBoolean("Silent"));
        setNoGravity(compound->getBoolean("NoGravity"));
        setGlowing(compound->getBoolean("Glowing"));
        if (compound->hasKey("Tags", 9)) 
        {
            tags.clear();
            NBTTagList nbttaglist1 = compound->getTagList("Tags", 8);
            auto i = MathHelper::min(nbttaglist1.tagCount(), 1024);

            for(int j = 0; j < i; ++j) 
            {
                tags.add(nbttaglist1.getStringTagAt(j));
            }
        }

        readEntityFromNBT(compound);
        if (shouldSetPosAfterLoading()) 
        {
            setPosition(posX, posY, posZ);
        }
    }
    catch (Throwable var8) 
    {
        CrashReport crashreport = CrashReport.makeCrashReport(var8, "Loading entity NBT");
        CrashReportCategory crashreportcategory = crashreport.makeCategory("Entity being loaded");
        addEntityCrashInfo(crashreportcategory);
        throw ReportedException(crashreport);
    }
}

EntityItem* Entity::dropItem(const Item* itemIn, int32_t size)
{
    return dropItemWithOffset(itemIn, size, 0.0F);
}

EntityItem * Entity::dropItemWithOffset(const Item* itemIn, int32_t size, float offsetY)
{
    return entityDropItem(ItemStack(itemIn, size, 0), offsetY);
}

EntityItem * Entity::entityDropItem(ItemStack stack, float offsetY)
{
    if (stack.isEmpty()) 
    {
        return nullptr;
    }
    else 
    {
        EntityItem* entityitem = new EntityItem(world, posX, posY + offsetY, posZ, stack);
        entityitem->setDefaultPickupDelay();
        world->spawnEntity(entityitem);
        return entityitem;
    }
}

bool Entity::isEntityAlive() const
{
    return !isDead;
}

bool Entity::isEntityInsideOpaqueBlock()
{
    if (noClip) 
    {
        return false;
    }
    else 
    {
        PooledMutableBlockPos blockpos$pooledmutableblockpos = PooledMutableBlockPos::retain();

        for(auto i = 0; i < 8; ++i) 
        {
            auto j = MathHelper::floor(posY + ((i >> 0) % 2) - 0.5F) * 0.1F) + getEyeHeight();
            auto k = MathHelper::floor(posX + ((i >> 1) % 2) - 0.5F) * width * 0.8F;
            auto l = MathHelper::floor(posZ + ((i >> 2) % 2) - 0.5F) * width * 0.8F;
            if (blockpos$pooledmutableblockpos.getx() != k || blockpos$pooledmutableblockpos.gety() != j || blockpos$pooledmutableblockpos.getz() != l) 
            {
                blockpos$pooledmutableblockpos.setPos(k, j, l);
                if (world->getBlockState(blockpos$pooledmutableblockpos)->causesSuffocation()) 
                {
                    blockpos$pooledmutableblockpos.release();
                    return true;
                }
            }
        }

        blockpos$pooledmutableblockpos.release();
        return false;
    }
}

bool Entity::processInitialInteract(EntityPlayer *player, EnumHand hand)
{
    return false;
}

std::optional<AxisAlignedBB> Entity::getCollisionBox(Entity *entityIn)
{
    return std::nullopt;
}

void Entity::updateRidden()
{
    auto entity = getRidingEntity();
    if (isRiding() && entity.isDead) 
    {
        dismountRidingEntity();
    }
    else 
    {
        motionX = 0.0;
        motionY = 0.0;
        motionZ = 0.0;
        onUpdate();
        if (isRiding()) 
        {
            entity.updatePassenger(this);
        }
    }
}

void Entity::updatePassenger(Entity *passenger)
{
    if (isPassenger(passenger)) 
    {
        passenger->setPosition(posX, posY + getMountedYOffset() + passenger->getYOffset(), posZ);
    }
}

void Entity::applyOrientationToEntity(Entity *entityToUpdate)
{

}

double Entity::getYOffset()
{
    return 0.0;
}

double Entity::getMountedYOffset() const
{
    return height * 0.75;
}

bool Entity::startRiding(Entity *entityIn)
{
    return startRiding(entityIn, false);
}

bool Entity::startRiding(Entity *entityIn, bool force)
{
    for(auto entity = entityIn; entity->ridingEntity != nullptr; entity = entity->ridingEntity) 
    {
        if (entity->ridingEntity == this) 
        {
            return false;
        }
    }

    if (!force && (!canBeRidden(entityIn) || !entityIn->canFitPassenger(this))) 
    {
        return false;
    }
    else 
    {
        if (isRiding()) 
        {
            dismountRidingEntity();
        }

        ridingEntity = entityIn;
        ridingEntity->addPassenger(this);
        return true;
    }
}

void Entity::removePassengers()
{
    for(auto i = riddenByEntities.size() - 1; i >= 0; --i) 
    {
        riddenByEntities[i]->dismountRidingEntity();
    }
}

void Entity::addPassenger(Entity *passenger)
{
    if (passenger->getRidingEntity() != this) 
    {
        throw std::logic_error("Use x.startRiding(y), not y.addPassenger(x)");
    }
    else 
    {
        if (!world->isRemote && Util::instanceof<EntityPlayer>(passenger) && !(Util::instanceof<EntityPlayer>(getControllingPassenger()))) 
        {
            riddenByEntities.emplace_back(0, passenger);
        }
        else 
        {
            riddenByEntities.emplace_back(passenger);
        }
    }
}

void Entity::removePassenger(Entity *passenger)
{
    if (passenger->getRidingEntity() == this) 
    {
        throw std::logic_error("Use x.stopRiding(y), not y.removePassenger(x)");
    }
    else 
    {
        Util::erase(riddenByEntities,passenger);
        passenger->rideCooldown = 60;
    }
}

bool Entity::canFitPassenger(Entity *passenger)
{
    return getPassengers().size() < 1;
}

bool Entity::getFlag(int32_t flag)
{
    return ((std::byte)dataManager.get(FLAGS) & 1 << flag) != 0;
}

void Entity::setFlag(int32_t flag, bool set)
{
    std::byte b0 = dataManager.get(FLAGS);
    if (set) 
    {
        dataManager.set(FLAGS, (b0 | 1 << flag));
    }
    else 
    {
        dataManager.set(FLAGS, (b0 & ~(1 << flag)));
    }
}

bool Entity::pushOutOfBlocks(double x, double y, double z)
{
    BlockPos blockpos(x, y, z);
    double d0 = x - blockpos.getx();
    double d1 = y - blockpos.gety();
    double d2 = z - blockpos.getz();
    if (!world->collidesWithAnyBlock(getEntityBoundingBox())) 
    {
        return false;
    }
    else 
    {
        EnumFacing enumfacing = EnumFacing::UP;
        double d3 = std::numeric_limits<double>::max();
        if (!world->isBlockFullCube(blockpos.west()) && d0 < d3) 
        {
            d3 = d0;
            enumfacing = EnumFacing::WEST;
        }

        if (!world->isBlockFullCube(blockpos.east()) && 1.0 - d0 < d3) 
        {
            d3 = 1.0 - d0;
            enumfacing = EnumFacing::EAST;
        }

        if (!world->isBlockFullCube(blockpos.north()) && d2 < d3) 
        {
            d3 = d2;
            enumfacing = EnumFacing::NORTH;
        }

        if (!world->isBlockFullCube(blockpos.south()) && 1.0 - d2 < d3) 
        {
            d3 = 1.0 - d2;
            enumfacing = EnumFacing::SOUTH;
        }

        if (!world->isBlockFullCube(blockpos.up()) && 1.0 - d1 < d3) 
        {
            d3 = 1.0 - d1;
            enumfacing = EnumFacing::UP;
        }

        float f = MathHelper::nextFloat(rand) * 0.2F + 0.1F;
        float f1 = enumfacing.getAxisDirection().getOffset();
        if (enumfacing.getAxis() == Axis::X) 
        {
            motionX = (f1 * f);
            motionY *= 0.75;
            motionZ *= 0.75;
        }
        else if (enumfacing.getAxis() == Axis::Y) 
        {
            motionX *= 0.75;
            motionY = (f1 * f);
            motionZ *= 0.75;
        }
        else if (enumfacing.getAxis() == Axis::Z) 
        {
            motionX *= 0.75;
            motionY *= 0.75;
            motionZ = (f1 * f);
        }

        return true;
    }
}

HoverEvent Entity::getHoverEvent()
{
    NBTTagCompound* nbttagcompound = new NBTTagCompound();
    ResourceLocation resourcelocation = EntityList.getKey(this);
    nbttagcompound->setString("id", getCachedUniqueIdString());
    if (resourcelocation != nullptr) 
    {
        nbttagcompound->setString("type", resourcelocation.to_string());
    }

    nbttagcompound->setString("name", getName());
    return HoverEvent(Action::SHOW_ENTITY, new TextComponentString(nbttagcompound->to_string()));
}

void Entity::applyEnchantments(EntityLivingBase *entityLivingBaseIn, Entity *entityIn)
{
    if (Util::instanceof<EntityLivingBase>(entityIn)) 
    {
        EnchantmentHelper::applyThornEnchantments((EntityLivingBase*)entityIn, entityLivingBaseIn);
    }

    EnchantmentHelper::applyArthropodEnchantments(entityLivingBaseIn, entityIn);
}

int32_t Entity::getFireImmuneTicks()
{
     return 1;
}

void Entity::dismountRidingEntity()
{
    if (ridingEntity != nullptr) 
    {
        Entity* entity = ridingEntity;
        ridingEntity = nullptr;
        entity->removePassenger(this);
    }
}

void Entity::setPositionAndRotationDirect(double x, double y, double z, float yaw, float pitch,
    int32_t posRotationIncrements, bool teleport)
{
    setPosition(x, y, z);
    setRotation(yaw, pitch);
}

float Entity::getCollisionBorderSize()
{
    return 0.0F;
}

Vec3d Entity::getLookVec()
{
    return getVectorForRotation(rotationPitch, rotationYaw);
}

Vec2f Entity::getPitchYaw() const
{
    return Vec2f(rotationPitch, rotationYaw);
}

Vec3d Entity::getForward() const
{
    return Vec3dfromPitchYaw(getPitchYaw());
}

void Entity::setPortal(const BlockPos &pos)
{
    if (timeUntilPortal > 0) 
    {
        timeUntilPortal = getPortalCooldown();
    }
    else 
    {
        if (!world->isRemote && !(pos == lastPortalPos)) 
        {
            lastPortalPos = pos;
            BlockPattern::PatternHelper blockpattern$patternhelper = Blocks::PORTAL.createPatternHelper(world, lastPortalPos);
            double d0 = blockpattern$patternhelper.getForwards().getAxis() == Axis::X ? blockpattern$patternhelper.getFrontTopLeft().getZ() : blockpattern$patternhelper.getFrontTopLeft().getX();
            double d1 = blockpattern$patternhelper.getForwards().getAxis() == Axis::X ? posZ : posX;
            d1 = MathHelper::abs(MathHelper::pct(d1 - (blockpattern$patternhelper.getForwards().rotateY().getAxisDirection() == AxisDirection::NEGATIVE ? 1 : 0), d0, d0 - blockpattern$patternhelper.getWidth()));
            double d2 = MathHelper::pct(posY - 1.0, blockpattern$patternhelper.getFrontTopLeft().getY(), (blockpattern$patternhelper.getFrontTopLeft().getY() - blockpattern$patternhelper.getHeight()));
            lastPortalVec = Vec3d(d1, d2, 0.0);
            teleportDirection = blockpattern$patternhelper.getForwards();
        }

        inPortal = true;
    }
}

int32_t Entity::getPortalCooldown()
{
    return 300;
}

void Entity::setVelocity(double x, double y, double z)
{
    motionX = x;
    motionY = y;
    motionZ = z;
}

void Entity::handleStatusUpdate(std::byte id)
{
}

void Entity::performHurtAnimation()
{
}

std::vector<> Entity::getHeldEquipment()
{
    return EMPTY_EQUIPMENT;
}

std::vector<> Entity::getArmorInventoryList()
{
    return EMPTY_EQUIPMENT;
}

Vec3d Entity::getVectorForRotation(float pitch, float yaw)
{
    float f = MathHelper::cos(-yaw * 0.017453292F - 3.1415927F);
    float f1 = MathHelper::sin(-yaw * 0.017453292F - 3.1415927F);
    float f2 = -MathHelper::cos(-pitch * 0.017453292F);
    float f3 = MathHelper::sin(-pitch * 0.017453292F);
    return Vec3d((f1 * f2), f3, (f * f2));
}

bool Entity::shouldSetPosAfterLoading()
{
    return true;
}

std::string Entity::getEntityString()
{
    auto resourcelocation = EntityList.getKey(this);
    return resourcelocation == nullptr ? nullptr : resourcelocation.toString();
}

NBTTagList * Entity::newDoubleNBTList(std::initializer_list<double> numbers)
{
    NBTTagList* nbttaglist = new NBTTagList();

    for(auto d0 : numbers)
    {
        nbttaglist->appendTag(new NBTTagDouble(d0));
    }

    return nbttaglist;
}

NBTTagList * Entity::newFloatNBTList(std::initializer_list<float> numbers)
{
    NBTTagList* nbttaglist = new NBTTagList();
    for(auto f : numbers)
    {
        nbttaglist->appendTag(new NBTTagFloat(f));
    }

    return nbttaglist;
}

bool Entity::canBeRidden(Entity *entityIn)
{
    return rideCooldown <= 0;
}

std::vector<> Entity::getEquipmentAndArmor()
{
    return getHeldEquipment() + getArmorInventoryList();
}

void Entity::setItemStackToSlot(EntityEquipmentSlot slotIn, ItemStack stack)
{
}

bool Entity::isBurning()
{
    bool flag = world != nullptr && world->isRemote;
    return !isImmuneToFire && (fire > 0 || flag && getFlag(0));
}

bool Entity::isRiding()
{
    return getRidingEntity() != nullptr;
}

bool Entity::isBeingRidden()
{
    return !getPassengers().isEmpty();
}

bool Entity::isSneaking()
{
    return getFlag(1);
}

void Entity::setSneaking(bool sneaking)
{
    setFlag(1, sneaking);
}

bool Entity::isSprinting()
{
    return getFlag(3);
}

void Entity::setSprinting(bool sprinting)
{
    setFlag(3, sprinting);
}

bool Entity::isGlowing()
{
    return glowing || world->isRemote && getFlag(6);
}

void Entity::setGlowing(bool glowingIn)
{
    glowing = glowingIn;
    if (!world->isRemote) 
    {
        setFlag(6, glowing);
    }
}

bool Entity::isInvisible()
{
    return getFlag(5);
}

bool Entity::isInvisibleToPlayer(EntityPlayer *player)
{
    if (player->isSpectator()) 
    {
        return false;
    }
    else 
    {
        Team* team = getTeam();
        return team != nullptr && player != nullptr && player->getTeam() == team && team->getSeeFriendlyInvisiblesEnabled() ? false : isInvisible();
    }
}

Team * Entity::getTeam()
{
    return world->getScoreboard().getPlayersTeam(getCachedUniqueIdString());
}

bool Entity::isOnSameTeam(Entity *entityIn)
{
    return isOnScoreboardTeam(entityIn->getTeam());
}

bool Entity::isOnScoreboardTeam(Team *teamIn)
{
    return getTeam() != nullptr ? getTeam()->isSameTeam(teamIn) : false;
}

void Entity::setInvisible(bool invisible)
{
    setFlag(5, invisible);
}

int32_t Entity::getAir()
{
    return dataManager.get(AIR);
}

void Entity::setAir(int32_t air)
{
    dataManager.set(AIR, air);
}

void Entity::onStruckByLightning(EntityLightningBolt *lightningBolt)
{
    attackEntityFrom(DamageSource::LIGHTNING_BOLT, 5.0F);
    ++fire;
    if (fire == 0) 
    {
        setFire(8);
    }
}

void Entity::onKillEntity(EntityLivingBase *entityLivingIn)
{

}

void Entity::setInWeb()
{
    isInWeb = true;
    fallDistance = 0.0F;
}

std::string Entity::getName()
{
    if (hasCustomName()) 
    {
        return getCustomNameTag();
    }
    else 
    {
        std::string s = EntityList.getEntityString(this);
        if (s.empty()) 
        {
            s = "generic";
        }

        return I18n::translateToLocal("entity." + s + ".name");
    }
}

std::vector<Entity*> Entity::getParts()
{
    return {};
}

bool Entity::isEntityEqual(Entity *entityIn)
{
    return this == entityIn;
}

float Entity::getRotationYawHead()
{
    return 0.0F;
}

void Entity::setRotationYawHead(float rotation)
{

}

void Entity::setRenderYawOffset(float offset)
{

}

bool Entity::canBeAttackedWithItem()
{
    return true;
}

bool Entity::hitByEntity(Entity *entityIn)
{
    return false;
}

std::string Entity::toString()
{
    return fmt::format("%s['%s'/%d, l='%s', x=%.2f, y=%.2f, z=%.2f]", getClass().getSimpleName(), getName(), entityId, world == nullptr ? "~NULL~" : world->getWorldInfo().getWorldName(), posX, posY, posZ);
}

bool Entity::isEntityInvulnerable(DamageSource source)
{
    return invulnerable && source != DamageSource::OUT_OF_WORLD && !source.isCreativePlayer();
}

bool Entity::getIsInvulnerable() const
{
    return invulnerable;
}

void Entity::setEntityInvulnerable(bool isInvulnerable)
{
    invulnerable = isInvulnerable;
}

void Entity::copyLocationAndAnglesFrom(Entity *entityIn)
{
    setLocationAndAngles(entityIn->posX, entityIn->posY, entityIn->posZ, entityIn->rotationYaw, entityIn->rotationPitch);
}

Entity * Entity::changeDimension(int32_t dimensionIn)
{
    if (!world->isRemote && !isDead) 
    {
        world->profiler.startSection("changeDimension");
        auto minecraftserver = getServer();
        int32_t i = dimension;
        WorldServer* worldserver = minecraftserver->getWorld(i);
        WorldServer* worldserver1 = minecraftserver->getWorld(dimensionIn);
        dimension = dimensionIn;
        if (i == 1 && dimensionIn == 1) 
        {
            worldserver1 = minecraftserver->getWorld(0);
            dimension = 0;
        }

        world->removeEntity(this);
        isDead = false;
        world->profiler.startSection("reposition");
        BlockPos blockpos;
        if (dimensionIn == 1) 
        {
            blockpos = worldserver1->getSpawnCoordinate();
        }
        else 
        {
            double d0 = posX;
            double d1 = posZ;
            double d2 = 8.0;
            if (dimensionIn == -1) 
            {
                d0 = MathHelper::clamp(d0 / 8.0, worldserver1->getWorldBorder().minX() + 16.0, worldserver1->getWorldBorder().maxX() - 16.0);
                d1 = MathHelper::clamp(d1 / 8.0, worldserver1->getWorldBorder().minZ() + 16.0, worldserver1->getWorldBorder().maxZ() - 16.0);
            }
            else if (dimensionIn == 0) 
            {
                d0 = MathHelper::clamp(d0 * 8.0, worldserver1->getWorldBorder().minX() + 16.0, worldserver1->getWorldBorder().maxX() - 16.0);
                d1 = MathHelper::clamp(d1 * 8.0, worldserver1->getWorldBorder().minZ() + 16.0, worldserver1->getWorldBorder().maxZ() - 16.0);
            }

            d0 = MathHelper::clamp((int)d0, -29999872, 29999872);
            d1 = MathHelper::clamp((int)d1, -29999872, 29999872);
            float f = rotationYaw;
            setLocationAndAngles(d0, posY, d1, 90.0F, 0.0F);
            Teleporter teleporter = worldserver1->getDefaultTeleporter();
            teleporter.placeInExistingPortal(this, f);
            blockpos = BlockPos(this);
        }

        worldserver->updateEntityWithOptionalForce(this, false);
        world->profiler.endStartSection("reloading");
        Entity* entity = EntityList.newEntity(getClass(), worldserver1);
        if (entity != nullptr) 
        {
            entity.copyDataFromOld(this);
            if (i == 1 && dimensionIn == 1) 
            {
                BlockPos blockpos1 = worldserver1->getTopSolidOrLiquidBlock(worldserver1->getSpawnPoint());
                entity->moveToBlockPosAndAngles(blockpos1, entity.rotationYaw, entity.rotationPitch);
            }
            else 
            {
                entity->moveToBlockPosAndAngles(blockpos, entity.rotationYaw, entity.rotationPitch);
            }

            bool flag = entity.forceSpawn;
            entity->forceSpawn = true;
            worldserver1->spawnEntity(entity);
            entity->forceSpawn = flag;
            worldserver1->updateEntityWithOptionalForce(entity, false);
        }

        isDead = true;
        world->profiler.endSection();
        worldserver->resetUpdateEntityTick();
        worldserver1->resetUpdateEntityTick();
        world->profiler.endSection();
        return entity;
    }
    else 
    {
        return nullptr;
    }
}

bool Entity::isNonBoss()
{
    return true;
}

float Entity::getExplosionResistance(Explosion explosionIn, World *worldIn, BlockPos pos, IBlockState *blockStateIn)
{
    return blockStateIn->getBlock()->getExplosionResistance(this);
}

bool Entity::canExplosionDestroyBlock(Explosion explosionIn, World *worldIn, BlockPos pos, IBlockState *blockStateIn,
    float p_174816_5_)
{
    return true;
}

int32_t Entity::getMaxFallHeight()
{
    return 3;
}

Vec3d Entity::getLastPortalVec() const
{
    return lastPortalVec;
}

EnumFacing Entity::getTeleportDirection() const
{
    return teleportDirection;
}

bool Entity::doesEntityNotTriggerPressurePlate()
{
    return false;
}

void Entity::addEntityCrashInfo(CrashReportCategory &category)
{
    category.addDetail("Entity Type", new ICrashReportDetail() {
        public std::string call() throws Exception {
            return EntityList.getKey(Entity.this) + " (" + Entity.this.getClass().getCanonicalName() + ")";
        }
    });
    category.addCrashSection("Entity ID", this.entityId);
    category.addDetail("Entity Name", new ICrashReportDetail() {
        public std::string call() throws Exception {
            return Entity.this.getName();
        }
    });
    category.addCrashSection("Entity's Exact location", fmt::format("%.2f, %.2f, %.2f", this.posX, this.posY, this.posZ));
    category.addCrashSection("Entity's Block location", CrashReportCategory.getCoordinateInfo(MathHelper.floor(this.posX), MathHelper.floor(this.posY), MathHelper.floor(this.posZ)));
    category.addCrashSection("Entity's Momentum", fmt::format("%.2f, %.2f, %.2f", this.motionX, this.motionY, this.motionZ));
    category.addDetail("Entity's Passengers", new ICrashReportDetail() {
        public std::string call() throws Exception {
            return Entity.this.getPassengers().toString();
        }
    });
    category.addDetail("Entity's Vehicle", new ICrashReportDetail() {
        public std::string call() throws Exception {
            return Entity.this.getRidingEntity().toString();
        }
    });
}

bool Entity::canRenderOnFire()
{
    return isBurning();
}

void Entity::setUniqueId(xg::Guid uniqueIdIn)
{
    entityUniqueID = uniqueIdIn;
    cachedUniqueIdString = entityUniqueID.str();
}

xg::Guid Entity::getUniqueID() const
{
    return entityUniqueID;
}

std::string_view Entity::getCachedUniqueIdString() const
{
    return cachedUniqueIdString;
}

bool Entity::isPushedByWater()
{
    return true;
}

double Entity::getRenderDistanceWeight()
{
    return renderDistanceWeight;
}

void Entity::setRenderDistanceWeight(double renderDistWeight)
{
    renderDistanceWeight = renderDistWeight;
}

ITextComponent * Entity::getDisplayName()
{
    TextComponentString* textcomponentstring = new TextComponentString(ScorePlayerTeam::formatPlayerName(getTeam(), getName()));
    textcomponentstring.getStyle().setHoverEvent(getHoverEvent());
    textcomponentstring.getStyle().setInsertion(getCachedUniqueIdString());
    return textcomponentstring;
}

void Entity::setCustomNameTag(std::string name)
{
    dataManager.set(CUSTOM_NAME, name);
}

std::string Entity::getCustomNameTag()
{
    return dataManager.get(CUSTOM_NAME);
}

bool Entity::hasCustomName()
{
    return !(dataManager.get(CUSTOM_NAME)).isEmpty();
}

void Entity::setAlwaysRenderNameTag(bool alwaysRenderNameTag)
{
    dataManager.set(CUSTOM_NAME_VISIBLE, alwaysRenderNameTag);
}

bool Entity::getAlwaysRenderNameTag()
{
    return dataManager.get(CUSTOM_NAME_VISIBLE);
}

void Entity::setPositionAndUpdate(double x, double y, double z)
{
    isPositionDirty = true;
    setLocationAndAngles(x, y, z, rotationYaw, rotationPitch);
    world->updateEntityWithOptionalForce(this, false);
}

bool Entity::getAlwaysRenderNameTagForRender()
{
    return getAlwaysRenderNameTag();
}

void Entity::preparePlayerToSpawn()
{
    if (world != nullptr) 
    {
        while(true) 
        {
            if (posY > 0.0 && posY < 256.0) 
            {
                setPosition(posX, posY, posZ);
                if (!world->getCollisionBoxes(this, getEntityBoundingBox()).isEmpty()) 
                {
                    ++posY;
                    continue;
                }
            }

            motionX = 0.0;
            motionY = 0.0;
            motionZ = 0.0;
            rotationPitch = 0.0F;
            break;
        }
    }
}

void Entity::setSize(float width, float height)
{
    if (width != width || height != height) 
    {
        float f = width;
        width = width;
        height = height;
        if (width < f) 
        {
            double d0 = width / 2.0;
            setEntityBoundingBox(AxisAlignedBB(posX - d0, posY, posZ - d0, posX + d0, posY + height, posZ + d0));
            return;
        }

        AxisAlignedBB axisalignedbb = getEntityBoundingBox();
        setEntityBoundingBox(AxisAlignedBB(axisalignedbb.getminX(), axisalignedbb.getminY(), axisalignedbb.getminZ(), axisalignedbb.getminX() + width, axisalignedbb.getminY() + height, axisalignedbb.getminZ() + width));
        if (width > f && !firstUpdate && !world->isRemote) 
        {
            move(MoverType::SELF, (f - width), 0.0, f - width);
        }
    }
}

void Entity::setRotation(float yaw, float pitch)
{
    rotationYaw = MathHelper::fmod(yaw, 360.0F);
    rotationPitch = MathHelper::fmod(pitch, 360.0F);
}

void Entity::decrementTimeUntilPortal()
{
    if (timeUntilPortal > 0) 
    {
        --timeUntilPortal;
    }
}

void Entity::setOnFireFromLava()
{
    if (!isImmuneToFire) 
    {
        attackEntityFrom(DamageSource::LAVA, 4.0F);
        setFire(15);
    }
}

void Entity::outOfWorld()
{
    setDead();
}

SoundEvent Entity::getSwimSound()
{
    return SoundEvents::ENTITY_GENERIC_SWIM;
}

SoundEvent Entity::getSplashSound()
{
    return SoundEvents::ENTITY_GENERIC_SPLASH;
}

void Entity::doBlockCollisions()
{
    AxisAlignedBB axisalignedbb = getEntityBoundingBox();
    PooledMutableBlockPos blockpos$pooledmutableblockpos = PooledMutableBlockPos::retain(axisalignedbb.getminX() + 0.001, axisalignedbb.getminY() + 0.001, axisalignedbb.getminZ() + 0.001);
    PooledMutableBlockPos blockpos$pooledmutableblockpos1 = PooledMutableBlockPos::retain(axisalignedbb.getmaxX() - 0.001, axisalignedbb.getmaxY() - 0.001, axisalignedbb.getmaxZ() - 0.001);
    PooledMutableBlockPos blockpos$pooledmutableblockpos2 = PooledMutableBlockPos::retain();
    if (world->isAreaLoaded(blockpos$pooledmutableblockpos, blockpos$pooledmutableblockpos1)) 
    {
        for(int32_t i = blockpos$pooledmutableblockpos.getx(); i <= blockpos$pooledmutableblockpos1.getx(); ++i) 
        {
            for(int32_t j = blockpos$pooledmutableblockpos.gety(); j <= blockpos$pooledmutableblockpos1.gety(); ++j) 
            {
                for(int32_t k = blockpos$pooledmutableblockpos.getz(); k <= blockpos$pooledmutableblockpos1.getz(); ++k) 
                {
                    blockpos$pooledmutableblockpos2.setPos(i, j, k);
                    IBlockState* iblockstate = world->getBlockState(blockpos$pooledmutableblockpos2);

                    try 
                    {
                        iblockstate->getBlock()->onEntityCollision(world, blockpos$pooledmutableblockpos2, iblockstate, this);
                        onInsideBlock(iblockstate);
                    }
                    catch (std::exception& var12) 
                    {
                        CrashReport crashreport = CrashReport.makeCrashReport(var12, "Colliding entity with block");
                        CrashReportCategory crashreportcategory = crashreport.makeCategory("Block being collided with");
                        CrashReportCategory.addBlockInfo(crashreportcategory, blockpos$pooledmutableblockpos2, iblockstate);
                        throw ReportedException(crashreport);
                    }
                }
            }
        }
    }

    blockpos$pooledmutableblockpos.release();
    blockpos$pooledmutableblockpos1.release();
    blockpos$pooledmutableblockpos2.release();
}

void Entity::onInsideBlock(IBlockState *p_191955_1_)
{
}

void Entity::playStepSound(BlockPos pos, Block *blockIn)
{
    SoundType soundtype = blockIn->getSoundType();^
    if (world->getBlockState(pos.up())->getBlock() == Blocks::SNOW_LAYER) 
    {
        soundtype = Blocks::SNOW_LAYER.getSoundType();
        playSound(soundtype.getStepSound(), soundtype.getVolume() * 0.15F, soundtype.getPitch());
    }
    else if (!blockIn->getDefaultState()->getMaterial().isLiquid()) 
    {
        playSound(soundtype.getStepSound(), soundtype.getVolume() * 0.15F, soundtype.getPitch());
    }
}

float Entity::playFlySound(float p_191954_1_)
{
    return 0.0F;
}

bool Entity::makeFlySound()
{
    return false;
}

bool Entity::canTriggerWalking()
{
    return true;
}

void Entity::updateFallState(double y, bool onGroundIn, IBlockState *state, BlockPos pos)
{
    if (onGroundIn) 
    {
        if (fallDistance > 0.0F) 
        {
            state->getBlock()->onFallenUpon(world, pos, this, fallDistance);
        }

        fallDistance = 0.0F;
    }
    else if (y < 0.0) 
    {
        fallDistance = fallDistance - y;
    }
}

void Entity::dealFireDamage(int32_t amount)
{
    if (!bisImmuneToFire) 
    {
        attackEntityFrom(DamageSource::IN_FIRE, amount);
    }
}

void Entity::doWaterSplashEffect()
{
    Entity* entity = isBeingRidden() && getControllingPassenger() != nullptr ? getControllingPassenger() : this;
    float f = entity == this ? 0.2F : 0.9F;
    float f1 = MathHelper::sqrt(entity->motionX * entity->motionX * 0.20000000298023224 + entity->motionY * entity->motionY + entity->motionZ * entity->motionZ * 0.20000000298023224) * f;
    if (f1 > 1.0F) 
    {
        f1 = 1.0F;
    }

    playSound(getSplashSound(), f1, 1.0F + (MathHelper::nextFloat(rand) - MathHelper::nextFloat(rand)) * 0.4F);
    float f2 = MathHelper::floor(getEntityBoundingBox().getminY());

    int j;
    float f5;
    float f6;
    for(j = 0; j < 1.0F + width * 20.0F; ++j) 
    {
        f5 = (MathHelper::nextFloat(rand) * 2.0F - 1.0F) * width;
        f6 = (MathHelper::nextFloat(rand) * 2.0F - 1.0F) * width;
        world->spawnParticle(EnumParticleTypes::WATER_BUBBLE, posX + f5, (f2 + 1.0F), posZ + f6, motionX, motionY - (MathHelper::nextFloat(rand) * 0.2F), motionZ);
    }

    for(j = 0; j < 1.0F + width * 20.0F; ++j) 
    {
        f5 = (MathHelper::nextFloat(rand) * 2.0F - 1.0F) * width;
        f6 = (MathHelper::nextFloat(rand) * 2.0F - 1.0F) * width;
        world->spawnParticle(EnumParticleTypes::WATER_SPLASH, posX + f5, (f2 + 1.0F), posZ + f6, motionX, motionY, motionZ);
    }
}

void Entity::createRunningParticles()
{
    auto i = MathHelper::floor(posX);
    auto j = MathHelper::floor(posY - 0.20000000298023224);
    auto k = MathHelper::floor(posZ);
    BlockPos blockpos = BlockPos(i, j, k);
    IBlockState* iblockstate = world->getBlockState(blockpos);
    if (iblockstate->getRenderType() != EnumBlockRenderType::INVISIBLE) 
    {
        world->spawnParticle(EnumParticleTypes::BLOCK_CRACK, posX + (MathHelper::nextFloat(rand) - 0.5) * width, getEntityBoundingBox().minY + 0.1, posZ + (MathHelper::nextFloat(rand) - 0.5) * width, -motionX * 4.0, 1.5, -motionZ * 4.0, Block::getStateId(iblockstate));
    }
}

void Entity::markVelocityChanged()
{
    velocityChanged = true;
}

bool Entity::isLiquidPresentInAABB(const AxisAlignedBB &bb)
{
    return world->getCollisionBoxes(this, bb).isEmpty() && !world->containsAnyLiquid(bb);
}

void Entity::copyDataFromOld(Entity *entityIn)
{
    NBTTagCompound* nbttagcompound = entityIn->writeToNBT(new NBTTagCompound());
    nbttagcompound->removeTag("Dimension");
    readFromNBT(nbttagcompound);
    timeUntilPortal = entityIn->timeUntilPortal;
    lastPortalPos = entityIn->lastPortalPos;
    lastPortalVec = entityIn->lastPortalVec;
    teleportDirection = entityIn->teleportDirection;
}

bool operator==(const Entity &lhs, const Entity &rhs)
{
    return lhs.entityId == rhs.entityId;
}
