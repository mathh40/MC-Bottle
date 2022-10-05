#pragma once
#include "EntityCreature.h"

class DataParameter;

class EntityAgeable : public EntityCreature
{
  public:
    EntityAgeable(World *worldIn);
    virtual EntityAgeable *createChild(EntityAgeable *var1) = 0;
    bool processInteract(EntityPlayer *player, EnumHand hand) override;
    int32_t getGrowingAge();
    void ageUp(int32_t growthSeconds, bool updateForcedAge);
    void addGrowth(int32_t growth);
    void setGrowingAge(int32_t age);
    void writeEntityToNBT(NBTTagCompound *compound) override;
    void readEntityFromNBT(NBTTagCompound *compound) override;
    void notifyDataManagerChange(DataParameter key) override;
    void onLivingUpdate() override;
    bool isChild() override;
    void setScaleForAge(bool child);

  protected:
    bool holdingSpawnEggOfClass(ItemStack stack, std::type_index entityClass);
    void entityInit() override;
    void onGrowingAdult();
    void setSize(float width, float height) override;
    void setScale(float scale);

    int32_t growingAge;
    int32_t forcedAge;
    int32_t forcedAgeTimer;

  private:
    static DataParameter BABY;
    float ageWidth = -1.0F;
    float ageHeight;
};
