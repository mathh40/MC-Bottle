#include "AbstractSkeleton.h"

#include "../SharedMonsterAttributes.h"
#include "../ai/EntityAIAvoidEntity.h"
#include "../ai/EntityAIBeg.h"
#include "../ai/EntityAIFleeSun.h"
#include "../ai/EntityAIFollowGolem.h"
#include "../ai/EntityAIHurtByTarget.h"
#include "../ai/EntityAILookIdle.h"
#include "../ai/EntityAINearestAttackableTarget.h"
#include "../ai/EntityAIRestrictSun.h"
#include "../ai/EntityAISwimming.h"
#include "../ai/EntityAIWanderAvoidWater.h"
#include "../ai/EntityAIWatchClosest.h"
#include "../ai/attributes/IAttributeInstance.h"
#include "../item/EntityBoat.h"

DataParameter AbstractSkeleton::SWINGING_ARMS =
    EntityDataManager.createKey(AbstractSkeleton.class, DataSerializers::BOOLEAN);

EntityAIAttackOnCollide::EntityAIAttackOnCollide(AbstractSkeleton *entitySkeletonIn, double speedIn,
                                                 bool useLongMemoryIn)
    : EntityAIAttackMelee(entitySkeletonIn, speedIn, useLongMemoryIn)
{
}

void EntityAIAttackOnCollide::resetTask()
{
    EntityAIAttackMelee::resetTask();
    attacker->setSwingingArms(false);
}

void EntityAIAttackOnCollide::startExecuting()
{
    EntityAIAttackMelee::startExecuting();
    attacker->setSwingingArms(true);
}

AbstractSkeleton::AbstractSkeleton(World *worldIn) : EntityMob(worldIn)
{
    setSize(0.6F, 1.99F);
    setCombatTask();
}

EnumCreatureAttribute AbstractSkeleton::getCreatureAttribute()
{
    return EnumCreatureAttribute::UNDEAD;
}

void AbstractSkeleton::onLivingUpdate()
{
    if (world->isDaytime() && !world->isRemote)
    {
        float f           = getBrightness();
        BlockPos blockpos = Util:: instanceof
            <EntityBoat>(getRidingEntity()) ? (BlockPos(posX, (double)MathHelper::round(posY), posZ)).up()
                                            : BlockPos(posX, (double)MathHelper::round(posY), posZ);
        if (f > 0.5F && MathHelper::nextFloat(rand) * 30.0F < (f - 0.4F) * 2.0F && world->canSeeSky(blockpos))
        {
            bool flag           = true;
            ItemStack itemstack = getItemStackFromSlot(EntityEquipmentSlot::HEAD);
            if (!itemstack.isEmpty())
            {
                if (itemstack.isItemStackDamageable())
                {
                    itemstack.setItemDamage(itemstack.getItemDamage() + rand(2));
                    if (itemstack.getItemDamage() >= itemstack.getMaxDamage())
                    {
                        renderBrokenItemStack(itemstack);
                        setItemStackToSlot(EntityEquipmentSlot::HEAD, ItemStack::EMPTY);
                    }
                }

                flag = false;
            }

            if (flag)
            {
                setFire(8);
            }
        }
    }

    EntityMob::onLivingUpdate();
}

void AbstractSkeleton::updateRidden()
{
    EntityMob::updateRidden();
    if (Util:: instanceof <EntityCreature>(getRidingEntity()))
    {
        EntityCreature entitycreature = (EntityCreature *)getRidingEntity();
        renderYawOffset               = entitycreature.renderYawOffset;
    }
}

IEntityLivingData *AbstractSkeleton::onInitialSpawn(DifficultyInstance difficulty, IEntityLivingData *livingdata)
{
    livingdata = EntityMob::onInitialSpawn(difficulty, livingdata);
    setEquipmentBasedOnDifficulty(difficulty);
    setEnchantmentBasedOnDifficulty(difficulty);
    setCombatTask();
    setCanPickUpLoot(MathHelper::nextFloat(rand) < 0.55F * difficulty.getClampedAdditionalDifficulty());
    if (getItemStackFromSlot(EntityEquipmentSlot::HEAD).isEmpty())
    {
        Calendar calendar = world->getCurrentDate();
        if (calendar.get(2) + 1 == 10 && calendar.get(5) == 31 && MathHelper::nextFloat(rand) < 0.25F)
        {
            setItemStackToSlot(EntityEquipmentSlot::HEAD,
                               ItemStack(MathHelper::nextFloat(rand) < 0.1F ? Blocks::LIT_PUMPKIN : Blocks::PUMPKIN));
            inventoryArmorDropChances[EntityEquipmentSlot::HEAD.getIndex()] = 0.0F;
        }
    }

    return livingdata;
}

void AbstractSkeleton::setCombatTask()
{
    if (world != nullptr && !world->isRemote)
    {
        tasks.removeTask(aiAttackOnCollide);
        tasks.removeTask(aiArrowAttack);
        ItemStack itemstack = getHeldItemMainhand();
        if (itemstack.getItem() == Items::BOW)
        {
            int i = 20;
            if (world->getDifficulty() != EnumDifficulty::HARD)
            {
                i = 40;
            }

            aiArrowAttack->setAttackCooldown(i);
            tasks.addTask(4, aiArrowAttack);
        }
        else
        {
            tasks.addTask(4, aiAttackOnCollide);
        }
    }
}

void AbstractSkeleton::attackEntityWithRangedAttack(EntityLivingBase *target, float distanceFactor)
{
    EntityArrow *entityarrow = getArrow(distanceFactor);
    double d0                = target->posX - posX;
    double d1 = target->getEntityBoundingBox().getminY() + (double)(target->height / 3.0F) - entityarrow->posY;
    double d2 = target->posZ - posZ;
    double d3 = (double)MathHelper::sqrt(d0 * d0 + d2 * d2);
    entityarrow->shoot(d0, d1 + d3 * 0.20000000298023224, d2, 1.6F, (float)(14 - world->getDifficulty().getId() * 4));
    playSound(SoundEvents::ENTITY_SKELETON_SHOOT, 1.0F, 1.0F / (MathHelper::nextFloat(getRNG()) * 0.4F + 0.8F));
    world->spawnEntity(entityarrow);
}

void AbstractSkeleton::readEntityFromNBT(NBTTagCompound *compound)
{
    EntityMob::readEntityFromNBT(compound);
    setCombatTask();
}

void AbstractSkeleton::setItemStackToSlot(EntityEquipmentSlot slotIn, ItemStack stack)
{
    EntityMob::setItemStackToSlot(slotIn, stack);
    if (!world->isRemote && slotIn == EntityEquipmentSlot::MAINHAND)
    {
        setCombatTask();
    }
}

float AbstractSkeleton::getEyeHeight() const
{
    return 1.74F;
}

double AbstractSkeleton::getYOffset()
{
    return -0.6;
}

bool AbstractSkeleton::isSwingingArms()
{
    return dataManager.get(SWINGING_ARMS);
}

void AbstractSkeleton::setSwingingArms(bool swingingArms)
{
    dataManager.set(SWINGING_ARMS, swingingArms);
}

void AbstractSkeleton::initEntityAI()
{
    tasks.addTask(1, new EntityAISwimming(this));
    tasks.addTask(2, new EntityAIRestrictSun(this));
    tasks.addTask(3, new EntityAIFleeSun(this, 1.0));
    tasks.addTask(3, new EntityAIAvoidEntity(this, EntityWolf.class, 6.0F, 1.0, 1.2));
    tasks.addTask(5, new EntityAIWanderAvoidWater(this, 1.0));
    tasks.addTask(6, new EntityAIWatchClosest(this, EntityPlayer.class, 8.0F));
    tasks.addTask(6, new EntityAILookIdle(this));
    targetTasks.addTask(1, new EntityAIHurtByTarget(this, false, new Class[0]));
    targetTasks.addTask(2, new EntityAINearestAttackableTarget(this, EntityPlayer.class, true));
    targetTasks.addTask(3, new EntityAINearestAttackableTarget(this, EntityIronGolem.class, true));
}

void AbstractSkeleton::applyEntityAttributes()
{
    EntityMob::applyEntityAttributes();
    getEntityAttribute(SharedMonsterAttributes::MOVEMENT_SPEED.get())->setBaseValue(0.25);
}

void AbstractSkeleton::entityInit()
{
    EntityMob::entityInit();
    dataManager.registe(SWINGING_ARMS, false);
}

void AbstractSkeleton::playStepSound(const BlockPos &pos, Block *blockIn)
{
    playSound(getStepSound(), 0.15F, 1.0F);
}

void AbstractSkeleton::setEquipmentBasedOnDifficulty(DifficultyInstance difficulty)
{
    EntityMob::setEquipmentBasedOnDifficulty(difficulty);
    setItemStackToSlot(EntityEquipmentSlot::MAINHAND, ItemStack(Items::BOW));
}

EntityArrow *AbstractSkeleton::getArrow(float p_190726_1_)
{
    EntityTippedArrow *entitytippedarrow = new EntityTippedArrow(world, this);
    entitytippedarrow->setEnchantmentEffectsFromEntity(this, p_190726_1_);
    return entitytippedarrow;
}
