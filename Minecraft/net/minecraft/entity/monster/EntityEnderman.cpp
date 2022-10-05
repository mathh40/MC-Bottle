#include "EntityEnderman.h"

#include "../SharedMonsterAttributes.h"
#include "../ai/EntityAIAttackMelee.h"
#include "../ai/EntityAIHurtByTarget.h"
#include "../ai/EntityAILookIdle.h"
#include "../ai/EntityAINearestAttackableTarget.h"
#include "../ai/EntityAISwimming.h"
#include "../ai/EntityAIWanderAvoidWater.h"
#include "../ai/EntityAIWatchClosest.h"
#include "../ai/attributes/IAttributeInstance.h"
#include "EntityDamageSourceIndirect.h"

AttributeModifier ATTACKING_SPEED_BOOST =
    (AttributeModifier(EntityEnderman::ATTACKING_SPEED_BOOST_ID, "Attacking speed boost", 0.15000000596046448, 0))
        .setSaved(false);
DataParameter CARRIED_BLOCK = EntityDataManager.createKey(EntityEnderman.class, DataSerializers::OPTIONAL_BLOCK_STATE);
DataParameter SCREAMING     = EntityDataManager.createKey(EntityEnderman.class, DataSerializers::BOOLEAN);

EntityEnderman::EntityEnderman(World *worldIn) : EntityMob(worldIn)
{
    setSize(0.6F, 2.9F);
    stepHeight = 1.0F;
    setPathPriority(PathNodeType::WATER, -1.0F);
}

void EntityEnderman::setAttackTarget(EntityLivingBase *entitylivingbaseIn)
{
    EntityMob::setAttackTarget(entitylivingbaseIn);
    IAttributeInstance *iattributeinstance = getEntityAttribute(SharedMonsterAttributes::MOVEMENT_SPEED.get());
    if (entitylivingbaseIn == nullptr)
    {
        targetChangeTime = 0;
        dataManager.set(SCREAMING, false);
        iattributeinstance->removeModifier(ATTACKING_SPEED_BOOST);
    }
    else
    {
        targetChangeTime = ticksExisted;
        dataManager.set(SCREAMING, true);
        if (!iattributeinstance->hasModifier(ATTACKING_SPEED_BOOST))
        {
            iattributeinstance->applyModifier(ATTACKING_SPEED_BOOST);
        }
    }
}

void EntityEnderman::playEndermanSound()
{
    if (ticksExisted >= lastCreepySound + 400)
    {
        lastCreepySound = ticksExisted;
        if (!isSilent())
        {
            world->playSound(posX, posY + (double)getEyeHeight(), posZ, SoundEvents::ENTITY_ENDERMEN_STARE,
                             getSoundCategory(), 2.5F, 1.0F, false);
        }
    }
}

void EntityEnderman::notifyDataManagerChange(DataParameter key)
{
    if (SCREAMING == key && isScreaming() && world->isRemote)
    {
        playEndermanSound();
    }

    EntityMob::notifyDataManagerChange(key);
}

void EntityEnderman::registerFixesEnderman(DataFixer fixer)
{
    EntityMob::registerFixesMob(fixer, EntityEnderman.class);
}

void EntityEnderman::writeEntityToNBT(NBTTagCompound *compound)
{
    EntityMob::writeEntityToNBT(compound);
    IBlockState *iblockstate = getHeldBlockState();
    if (iblockstate != nullptr)
    {
        compound->setShort("carried", (short)Block::getIdFromBlock(iblockstate->getBlock()));
        compound->setShort("carriedData", (short)iblockstate->getBlock()->getMetaFromState(iblockstate));
    }
}

void EntityEnderman::readEntityFromNBT(NBTTagCompound *compound)
{
    EntityMob::readEntityFromNBT(compound);
    IBlockState *iblockstate;
    if (compound->hasKey("carried", 8))
    {
        iblockstate = Block::getBlockFromName(compound->getString("carried"))
                          ->getStateFromMeta(compound->getShort("carriedData") & '\uffff');
    }
    else
    {
        iblockstate = Block::getBlockById(compound->getShort("carried"))
                          ->getStateFromMeta(compound->getShort("carriedData") & '\uffff');
    }

    if (iblockstate == nullptr || iblockstate->getBlock() == nullptr || iblockstate->getMaterial() == Material::AIR)
    {
        iblockstate = nullptr;
    }

    setHeldBlockState(iblockstate);
}

float EntityEnderman::getEyeHeight()
{
    return 2.55F;
}

void EntityEnderman::onLivingUpdate()
{
    if (world->isRemote)
    {
        for (int i = 0; i < 2; ++i)
        {
            world->spawnParticle(EnumParticleTypes::PORTAL, posX + (MathHelper::nextDouble(rand) - 0.5) * (double)width,
                                 posY + MathHelper::nextDouble(rand) * (double)height - 0.25,
                                 posZ + (MathHelper::nextDouble(rand) - 0.5) * (double)width,
                                 (MathHelper::nextDouble(rand) - 0.5) * 2.0, -MathHelper::nextDouble(rand),
                                 (MathHelper::nextDouble(rand) - 0.5) * 2.0, {});
        }
    }

    isJumping = false;
    EntityMob::onLivingUpdate();
}

void EntityEnderman::setHeldBlockState(IBlockState *state)
{
    dataManager.set(CARRIED_BLOCK, Optional.fromNullable(state));
}

IBlockState *EntityEnderman::getHeldBlockState()
{
    return dataManager.get(CARRIED_BLOCK);
}

bool EntityEnderman::attackEntityFrom(DamageSource::DamageSource source, float amount)
{
    if (isEntityInvulnerable(source))
    {
        return false;
    }
    else if (Util:: instanceof <DamageSource::EntityDamageSourceIndirect>(source))
    {
        for (int32_t i = 0; i < 64; ++i)
        {
            if (teleportRandomly())
            {
                return true;
            }
        }

        return false;
    }
    else
    {
        bool flag = EntityMob::attackEntityFrom(source, amount);
        if (source.isUnblockable() && rand(10) != 0)
        {
            teleportRandomly();
        }

        return flag;
    }
}

bool EntityEnderman::isScreaming()
{
    return dataManager.get(SCREAMING);
}

EntityEnderman::AITakeBlock::AITakeBlock(EntityEnderman *p_i45841_1_) : enderman(p_i45841_1_)
{
}

bool EntityEnderman::AITakeBlock::shouldExecute()
{
    if (enderman->getHeldBlockState() != nullptr)
    {
        return false;
    }
    else if (!enderman->world->getGameRules().getBoolean("mobGriefing"))
    {
        return false;
    }
    else
    {
        return enderman->getRNG()(20) == 0;
    }
}

void EntityEnderman::AITakeBlock::updateTask()
{
    auto random                   = enderman->getRNG();
    World *world                  = enderman->world;
    int i                         = MathHelper::floor(enderman->posX - 2.0 + MathHelper::nextDouble(random) * 4.0);
    int j                         = MathHelper::floor(enderman->posY + MathHelper::nextDouble(random) * 3.0);
    int k                         = MathHelper::floor(enderman->posZ - 2.0 + MathHelper::nextDouble(random) * 4.0);
    BlockPos blockpos             = BlockPos(i, j, k);
    IBlockState *iblockstate      = world->getBlockState(blockpos);
    Block *block                  = iblockstate->getBlock();
    RayTraceResult raytraceresult = world->rayTraceBlocks(
        Vec3d((double)((float)MathHelper::floor(enderman->posX) + 0.5F), (double)((float)j + 0.5F),
              (double)((float)MathHelper::floor(enderman->posZ) + 0.5F)),
        Vec3d((double)((float)i + 0.5F), (double)((float)j + 0.5F), (double)((float)k + 0.5F)), false, true, false);
    bool flag = raytraceresult != nullptr && raytraceresult.getBlockPos() == (blockpos);
    if (EntityEnderman::CARRIABLE_BLOCKS.contains(block) && flag)
    {
        enderman->setHeldBlockState(iblockstate);
        world->setBlockToAir(blockpos);
    }
}

EntityEnderman::AIPlaceBlock::AIPlaceBlock(EntityEnderman *p_i45843_1_) : enderman(p_i45843_1_)
{
}

bool EntityEnderman::AIPlaceBlock::shouldExecute()
{
    if (enderman->getHeldBlockState() == nullptr)
    {
        return false;
    }
    else if (!enderman->world->getGameRules().getBoolean("mobGriefing"))
    {
        return false;
    }
    else
    {
        return enderman->getRNG()(2000) == 0;
    }
}

void EntityEnderman::AIPlaceBlock::updateTask()
{
    auto random               = enderman->getRNG();
    World *world              = enderman->world;
    int i                     = MathHelper::floor(enderman->posX - 1.0 + MathHelper::nextDouble(random) * 2.0);
    int j                     = MathHelper::floor(enderman->posY + MathHelper::nextDouble(random) * 2.0);
    int k                     = MathHelper::floor(enderman->posZ - 1.0 + MathHelper::nextDouble(random) * 2.0);
    BlockPos blockpos         = BlockPos(i, j, k);
    IBlockState *iblockstate  = world->getBlockState(blockpos);
    IBlockState *iblockstate1 = world->getBlockState(blockpos.down());
    IBlockState *iblockstate2 = enderman->getHeldBlockState();
    if (iblockstate2 != nullptr && canPlaceBlock(world, blockpos, iblockstate2->getBlock(), iblockstate, iblockstate1))
    {
        world->setBlockState(blockpos, iblockstate2, 3);
        enderman->setHeldBlockState(nullptr);
    }
}

bool EntityEnderman::AIPlaceBlock::canPlaceBlock(World *p_188518_1_, BlockPos p_188518_2_, Block *p_188518_3_,
                                                 IBlockState *p_188518_4_, IBlockState *p_188518_5_)
{
    if (!p_188518_3_->canPlaceBlockAt(p_188518_1_, p_188518_2_))
    {
        return false;
    }
    else if (p_188518_4_->getMaterial() != Material::AIR)
    {
        return false;
    }
    else
    {
        return p_188518_5_->getMaterial() == Material::AIR ? false : p_188518_5_->isFullCube();
    }
}

EntityEnderman::AIFindPlayer::AIFindPlayer(EntityEnderman *p_i45842_1_)
    : EntityAINearestAttackableTarget(p_i45842_1_, typeid(EntityPlayer), false), enderman(p_i45842_1_)
{
}

bool EntityEnderman::AIFindPlayer::shouldExecute()
{
    double d0 = getTargetDistance();
    player    = enderman->world->getNearestAttackablePlayer(
           enderman->posX, enderman->posY, enderman->posZ, d0, d0, std::nullopt, [](EntityPlayer *p_apply_1_) {
            return p_apply_1_ != nullptr && AIFindPlayer::enderman->shouldAttackPlayer(p_apply_1_);
        });
    return player != nullptr;
}

void EntityEnderman::AIFindPlayer::startExecuting()
{
    aggroTime    = 5;
    teleportTime = 0;
}

void EntityEnderman::AIFindPlayer::resetTask()
{
    player = nullptr;
    EntityAINearestAttackableTarget::resetTask();
}

bool EntityEnderman::AIFindPlayer::shouldContinueExecuting()
{
    if (player != nullptr)
    {
        if (!enderman->shouldAttackPlayer(player))
        {
            return false;
        }
        else
        {
            enderman->faceEntity(player, 10.0F, 10.0F);
            return true;
        }
    }
    else
    {
        return targetEntity != nullptr && ((EntityPlayer)targetEntity).isEntityAlive()
                   ? true
                   : EntityAINearestAttackableTarget::shouldContinueExecuting();
    }
}

void EntityEnderman::AIFindPlayer::updateTask()
{
    if (player != nullptr)
    {
        if (--aggroTime <= 0)
        {
            targetEntity = player;
            player       = nullptr;
            EntityAINearestAttackableTarget::startExecuting();
        }
    }
    else
    {
        if (targetEntity != nullptr)
        {
            if (enderman->shouldAttackPlayer((EntityPlayer *)targetEntity))
            {
                if (((EntityPlayer *)targetEntity)->getDistanceSq(enderman) < 16.0)
                {
                    enderman->teleportRandomly();
                }

                teleportTime = 0;
            }
            else if (((EntityPlayer *)targetEntity)->getDistanceSq(enderman) > 256.0 && teleportTime++ >= 30 &&
                     enderman->teleportToEntity(targetEntity))
            {
                teleportTime = 0;
            }
        }

        EntityAINearestAttackableTarget::updateTask();
    }
}

void EntityEnderman::initEntityAI()
{
    tasks.addTask(0, new EntityAISwimming(this));
    tasks.addTask(2, new EntityAIAttackMelee(this, 1.0, false));
    tasks.addTask(7, new EntityAIWanderAvoidWater(this, 1.0, 0.0F));
    tasks.addTask(8, new EntityAIWatchClosest(this, EntityPlayer.class, 8.0F));
    tasks.addTask(8, new EntityAILookIdle(this));
    tasks.addTask(10, new EntityEnderman::AIPlaceBlock(this));
    tasks.addTask(11, new EntityEnderman::AITakeBlock(this));
    targetTasks.addTask(1, new EntityEnderman::AIFindPlayer(this));
    targetTasks.addTask(2, new EntityAIHurtByTarget(this, false, new Class[0]));
    targetTasks.addTask(3, new EntityAINearestAttackableTarget(
                               this, EntityEndermite.class, 10, true, false,
                               [](EntityEndermite *p_apply_1_) { return p_apply_1_->isSpawnedByPlayer(); }));
}

void EntityEnderman::applyEntityAttributes()
{
    EntityMob::applyEntityAttributes();
    getEntityAttribute(SharedMonsterAttributes::MAX_HEALTH.get())->setBaseValue(40.0);
    getEntityAttribute(SharedMonsterAttributes::MOVEMENT_SPEED.get())->setBaseValue(0.30000001192092896);
    getEntityAttribute(SharedMonsterAttributes::ATTACK_DAMAGE.get())->setBaseValue(7.0);
    getEntityAttribute(SharedMonsterAttributes::FOLLOW_RANGE.get())->setBaseValue(64.0);
}

void EntityEnderman::entityInit()
{
    EntityMob::entityInit();
    dataManager.registe(CARRIED_BLOCK, Optional.absent());
    dataManager.registe(SCREAMING, false);
}

void EntityEnderman::updateAITasks()
{
    if (isWet())
    {
        attackEntityFrom(DamageSource::DROWN, 1.0F);
    }

    if (world->isDaytime() && ticksExisted >= targetChangeTime + 600)
    {
        float f = getBrightness();
        if (f > 0.5F && world->canSeeSky(BlockPos(this)) && MathHelper::nextFloat(rand) * 30.0F < (f - 0.4F) * 2.0F)
        {
            setAttackTarget(nullptr);
            teleportRandomly();
        }
    }

    EntityMob::updateAITasks();
}

bool EntityEnderman::teleportRandomly()
{
    double d0 = posX + (MathHelper::nextDouble(rand) - 0.5) * 64.0;
    double d1 = posY + (double)(rand(64) - 32);
    double d2 = posZ + (MathHelper::nextDouble(rand) - 0.5) * 64.0;
    return teleportTo(d0, d1, d2);
}

bool EntityEnderman::teleportToEntity(Entity *p_70816_1_)
{
    Vec3d vec3d = Vec3d(posX - p_70816_1_->posX,
                        getEntityBoundingBox().getminY() + (double)(height / 2.0F) - p_70816_1_->posY +
                            (double)p_70816_1_->getEyeHeight(),
                        posZ - p_70816_1_->posZ);
    vec3d       = vec3d.normalize();
    double d0   = 16.0;
    double d1   = posX + (MathHelper::nextDouble(rand) - 0.5) * 8.0 - vec3d.getx() * 16.0;
    double d2   = posY + (double)(rand(16) - 8) - vec3d.gety() * 16.0;
    double d3   = posZ + (MathHelper::nextDouble(rand) - 0.5) * 8.0 - vec3d.getz() * 16.0;
    return teleportTo(d1, d2, d3);
}

SoundEvent EntityEnderman::getAmbientSound()
{
    return isScreaming() ? SoundEvents::ENTITY_ENDERMEN_SCREAM : SoundEvents::ENTITY_ENDERMEN_AMBIENT;
}

SoundEvent EntityEnderman::getHurtSound(DamageSource::DamageSource damageSourceIn)
{
    return SoundEvents::ENTITY_ENDERMEN_HURT;
}

SoundEvent EntityEnderman::getDeathSound()
{
    return SoundEvents::ENTITY_ENDERMEN_DEATH;
}

void EntityEnderman::dropEquipment(bool wasRecentlyHit, int32_t lootingModifier)
{
    EntityMob::dropEquipment(wasRecentlyHit, lootingModifier);
    IBlockState *iblockstate = getHeldBlockState();
    if (iblockstate != nullptr)
    {
        Item *item = Item::getItemFromBlock(iblockstate->getBlock());
        auto i     = item->getHasSubtypes() ? iblockstate->getBlock()->getMetaFromState(iblockstate) : 0;
        entityDropItem(ItemStack(item, 1, i), 0.0F);
    }
}

std::optional<ResourceLocation> EntityEnderman::getLootTable()
{
    return LootTableList::ENTITIES_ENDERMAN;
}

bool EntityEnderman::shouldAttackPlayer(EntityPlayer *player)
{
    ItemStack itemstack = (ItemStack)player->inventory.armorInventory.get(3);
    if (itemstack.getItem() == Item::getItemFromBlock(Blocks::PUMPKIN))
    {
        return false;
    }
    else
    {
        Vec3d vec3d  = player->getLook(1.0F).normalize();
        Vec3d vec3d1 = Vec3d(posX - player->posX,
                             getEntityBoundingBox().getminY() + (double)getEyeHeight() -
                                 (player->posY + (double)player->getEyeHeight()),
                             posZ - player->posZ);
        double d0    = vec3d1.length();
        vec3d1       = vec3d1.normalize();
        double d1    = vec3d.dotProduct(vec3d1);
        return d1 > 1.0 - 0.025 / d0 ? player->canEntityBeSeen(this) : false;
    }
}

bool EntityEnderman::teleportTo(double x, double y, double z)
{
    bool flag = attemptTeleport(x, y, z);
    if (flag)
    {
        world->playSound(nullptr, prevPosX, prevPosY, prevPosZ, SoundEvents::ENTITY_ENDERMEN_TELEPORT,
                         getSoundCategory(), 1.0F, 1.0F);
        playSound(SoundEvents::ENTITY_ENDERMEN_TELEPORT, 1.0F, 1.0F);
    }

    return flag;
}
