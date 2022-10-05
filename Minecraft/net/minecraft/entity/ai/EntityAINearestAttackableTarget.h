#pragma once
#include "EntityAITarget.h"

#include <functional>
#include <typeindex>

class Entity;

class EntityAINearestAttackableTarget : public EntityAITarget
{
  public:
    EntityAINearestAttackableTarget(EntityCreature *creature, std::type_index classTarget, bool checkSight);
    EntityAINearestAttackableTarget(EntityCreature *creature, std::type_index classTarget, bool checkSight,
                                    bool onlyNearby);
    EntityAINearestAttackableTarget(EntityCreature *creature, std::type_index classTarget, int32_t chance,
                                    bool checkSight, bool onlyNearby, std::function<bool()> targetSelector);
    bool shouldExecute() override;

  protected:
    class Sorter
    {
      private:
        Entity *entity;

      public:
        Sorter(Entity *entityIn);

        bool operator()(const Entity *p_compare_1_, const Entity *p_compare_2_) const;
    };

    std::type_index targetClass;
    EntityAINearestAttackableTarget::Sorter sorter;
    std::function<bool(EntityLivingBase *)> targetEntitySelector;
    EntityLivingBase *targetEntity;

  private:
    int32_t targetChance;
};
