#pragma once
#include "../Entity.h"

class EntityItem : public Entity
{
  public:
    EntityItem(World *worldIn, double x, double y, double z);
    EntityItem(World *worldIn, double x, double y, double z, ItemStack stack);
    explicit EntityItem(World *worldIn);
    void onUpdate() override;
    void setAgeToCreativeDespawnTime();
    bool handleWaterMovement();
    bool attackEntityFrom(DamageSource::DamageSource source, float amount) override;
    static void registerFixesItem(DataFixer fixer);
    void writeEntityToNBT(NBTTagCompound *compound) override;
    void readEntityFromNBT(NBTTagCompound *compound) override;
    void onCollideWithPlayer(EntityPlayer *entityIn) override;
    std::string getName() override;
    bool canBeAttackedWithItem() override;
    Entity *changeDimension(int32_t dimensionIn) override;
    ItemStack getItem();
    void setItem(ItemStack stack);
    std::string getOwner() const;
    void setOwner(std::string_view ownerIn);
    std::string getThrower() const;
    void setThrower(std::string_view throwerIn);
    int32_t getAge() const;
    void setDefaultPickupDelay();
    void setNoPickupDelay();
    void setInfinitePickupDelay();
    void setPickupDelay(int32_t ticks);
    bool cannotPickup() const;
    void setNoDespawn();
    void makeFakeItem();

    float hoverStart;

  protected:
    bool canTriggerWalking() override;
    void entityInit() override;
    void dealFireDamage(int32_t amount) override;

  private:
    void searchForOtherItemsNearby();
    bool combineItems(EntityItem *other);

    static std::shared_ptr<spdlog::logger> LOGGER;
    static DataParameter ITEM;
    int32_t age;
    int32_t pickupDelay;
    int32_t health;
    std::string thrower;
    std::string owner;
};
