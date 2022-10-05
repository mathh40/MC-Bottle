#pragma once
#include "EntityMob.h"

class AbstractIllager : public EntityMob
{
  public:
    enum IllagerArmPose
    {
        CROSSED,
        ATTACKING,
        SPELLCASTING,
        BOW_AND_ARROW
    };

    AbstractIllager(World *world);
    EnumCreatureAttribute getCreatureAttribute() override;
    virtual AbstractIllager::IllagerArmPose getArmPose();

  protected:
    void entityInit() override;
    bool isAggressive(int32_t mask);
    void setAggressive(int32_t mask, bool value);

    static DataParameter AGGRESSIVE;

  private:
};
