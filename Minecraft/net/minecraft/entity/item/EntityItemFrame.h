#pragma once
#include "../EntityHanging.h"

class EntityItemFrame : public EntityHanging
{
  public:
    EntityItemFrame(World *worldIn);
    EntityItemFrame(World *worldIn, BlockPos pos, EnumFacing facing);
    float getCollisionBorderSize() override;
    bool attackEntityFrom(DamageSource::DamageSource source, float amount) override;
    int32_t getWidthPixels() override;
    int32_t getHeightPixels() override;
    bool isInRangeToRenderDist(double distance) override;
    void onBroken(Entity *brokenEntity) override;
    void playPlaceSound() override;
    void dropItemOrSelf(Entity *entityIn, bool p_146065_2_);
    ItemStack getDisplayedItem();
    void setDisplayedItem(ItemStack stack);
    void notifyDataManagerChange(DataParameter key) override;
    int32_t getRotation();
    void setItemRotation(int32_t rotationIn);
    static void registerFixesItemFrame(DataFixer fixer);
    void writeEntityToNBT(NBTTagCompound *compound) override;
    void readEntityFromNBT(NBTTagCompound *compound) override;
    bool processInitialInteract(EntityPlayer *player, EnumHand hand) override;
    int32_t getAnalogOutput();

  protected:
    void entityInit() override;

  private:
    void removeFrameFromMap(ItemStack stack);
    void setDisplayedItemWithUpdate(ItemStack stack, bool p_174864_2_);
    void setRotation(int32_t rotationIn, bool p_174865_2_);

    static DataParameter ITEM;
    static DataParameter ROTATION;
    float itemDropChance = 1.0F;
};
