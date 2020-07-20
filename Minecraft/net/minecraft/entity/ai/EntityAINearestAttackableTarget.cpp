#include "EntityAINearestAttackableTarget.h"

#include "EntitySelectors.h"

EntityAINearestAttackableTarget::EntityAINearestAttackableTarget(EntityCreature *creature, std::type_index classTarget,
                                                                 bool checkSight)
        :EntityAINearestAttackableTarget(creature, classTarget, checkSight, false)
{
}

EntityAINearestAttackableTarget::EntityAINearestAttackableTarget(EntityCreature *creature, std::type_index classTarget,
    bool checkSight, bool onlyNearby)
        :EntityAINearestAttackableTarget(creature, classTarget, 10, checkSight, onlyNearby, nullptr)
{
}

bool EntityAINearestAttackableTarget::shouldExecute()
{
    if (this.targetChance > 0 && this.taskOwner.getRNG().nextInt(this.targetChance) != 0) {
        return false;
    } else if (this.targetClass != EntityPlayer.class && this.targetClass != EntityPlayerMP.class) {
        List list = this.taskOwner.world.getEntitiesWithinAABB(this.targetClass, this.getTargetableArea(this.getTargetDistance()), this.targetEntitySelector);
        if (list.isEmpty()) {
            return false;
        } else {
            Collections.sort(list, this.sorter);
            this.targetEntity = (EntityLivingBase)list.get(0);
            return true;
        }
    } else {
        this.targetEntity = this.taskOwner.world.getNearestAttackablePlayer(this.taskOwner.posX, this.taskOwner.posY + (double)this.taskOwner.getEyeHeight(), this.taskOwner.posZ, this.getTargetDistance(), this.getTargetDistance(), new Function() {
            @Nullable
            public Double apply(@Nullable EntityPlayer p_apply_1_) {
                ItemStack itemstack = p_apply_1_.getItemStackFromSlot(EntityEquipmentSlot.HEAD);
                if (itemstack.getItem() == Items.SKULL) {
                    int i = itemstack.getItemDamage();
                    boolean flag = EntityAINearestAttackableTarget.this.taskOwner instanceof EntitySkeleton && i == 0;
                    boolean flag1 = EntityAINearestAttackableTarget.this.taskOwner instanceof EntityZombie && i == 2;
                    boolean flag2 = EntityAINearestAttackableTarget.this.taskOwner instanceof EntityCreeper && i == 4;
                    if (flag || flag1 || flag2) {
                        return 0.5D;
                    }
                }

                return 1.0D;
            }
        }, this.targetEntitySelector);
        return this.targetEntity != null;
    }
}

EntityAINearestAttackableTarget::EntityAINearestAttackableTarget(EntityCreature* creature, std::type_index classTarget, int32_t chance, bool checkSight, bool onlyNearby, std::function<bool(EntityLivingBase*)> targetSelector)
    :EntityAITarget(creature, checkSight, onlyNearby),targetClass(classTarget),targetChance(chance),sorter(EntityAINearestAttackableTarget::Sorter(creature))
{
    setMutexBits(1);
    targetEntitySelector([&](EntityLivingBase* p_apply_1_)->bool
    {
        if (p_apply_1_ == nullptr) 
        {
            return false;
        }
        else if (targetSelector != nullptr && !targetSelector(p_apply_1_)) 
        {
            return false;
        }
        else 
        {
            return !EntitySelectors::NOT_SPECTATING(p_apply_1_) ? false : isSuitableTarget(p_apply_1_, false);
        }
    });
}

EntityAINearestAttackableTarget::Sorter::Sorter(Entity *entityIn)
{
    entity = entityIn;
}

bool EntityAINearestAttackableTarget::Sorter::operator()(const Entity *p_compare_1_, const Entity *p_compare_2_) const
{
    double d0 = entity->getDistanceSq(p_compare_1_);
    double d1 = entity->getDistanceSq(p_compare_2_);
    return d0 < d1;
}
