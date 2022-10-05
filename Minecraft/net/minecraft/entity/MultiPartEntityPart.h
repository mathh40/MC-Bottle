#pragma once
#include "Entity.h"

class IEntityMultiPart;

class MultiPartEntityPart : public Entity
{
  public:
    MultiPartEntityPart(IEntityMultiPart *parent, std::string_view partName, float width, float height);
    bool canBeCollidedWith() override;
    bool attackEntityFrom(DamageSource::DamageSource source, float amount) override;
    bool isEntityEqual(Entity *entityIn) override;

    IEntityMultiPart *parent;
    std::string partName;

  protected:
    void entityInit() override;
    void readEntityFromNBT(NBTTagCompound *compound) override;
    void writeEntityToNBT(NBTTagCompound *compound) override;

  private:
};
