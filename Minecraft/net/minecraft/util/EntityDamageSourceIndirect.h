#pragma once
#include "EntityDamageSource.h"

namespace DamageSource
{
class EntityDamageSourceIndirect : public EntityDamageSource
{
  public:
    EntityDamageSourceIndirect(std::string damageTypeIn, Entity source, std::optional<Entity> indirectEntityIn);
    std::optional<Entity> getImmediateSource();
    std::optional<Entity> getTrueSource();
    ITextComponent getDeathMessage(EntityLivingBase entityLivingBaseIn);

  private:
    std::optional<Entity> indirectEntity;
};
} // namespace DamageSource
