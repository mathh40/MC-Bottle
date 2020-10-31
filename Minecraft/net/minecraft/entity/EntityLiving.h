#pragma once
#include "EntityBodyHelper.h"
#include "EntityLivingBase.h"
#include "../pathfinding/PathNavigate.h"
#include "ai/EntityAITasks.h"
#include "ai/EntityJumpHelper.h"
#include "ai/EntityLookHelper.h"
#include "ai/EntityMoveHelper.h"

class IEntityLivingData;

enum SpawnPlacementType {
    ON_GROUND,
    IN_AIR,
    IN_WATER
};


class EntityLiving :public EntityLivingBase {
public:
    EntityLiving(World* worldIn);
    float getPathPriority(PathNodeType nodeType);
    void setPathPriority(PathNodeType nodeType, float priority);
    EntityLookHelper& getLookHelper();
    EntityMoveHelper& getMoveHelper();
    EntityJumpHelper& getJumpHelper();
    PathNavigate* getNavigator();
    EntitySenses& getEntitySenses();
    EntityLivingBase* getAttackTarget();
    virtual void setAttackTarget(EntityLivingBase* entitylivingbaseIn);
    bool canAttackClass(std::type_index cls);
    void eatGrassBonus();
    virtual int32_t getTalkInterval();
    void playLivingSound();
    void onEntityUpdate() override;
    void spawnExplosionParticle();
    void handleStatusUpdate(std::byte id) override;
    void onUpdate() override;
    static void registerFixesMob(DataFixer fixer, std::type_index name);
    void writeEntityToNBT(NBTTagCompound* compound) override;
    void readEntityFromNBT(NBTTagCompound* compound) override;
    void setMoveForward(float amount);
    void setMoveVertical(float amount);
    void setMoveStrafing(float amount);
    void setAIMoveSpeed(float speedIn) override;
    void onLivingUpdate() override;
    virtual int32_t getVerticalFaceSpeed();
    int32_t getHorizontalFaceSpeed();
    void faceEntity(Entity* entityIn, float maxYawIncrease, float maxPitchIncrease);
    virtual bool getCanSpawnHere();
    virtual bool isNotColliding();
    float getRenderSizeModifier();
    int32_t getMaxSpawnedInChunk();
    int32_t getMaxFallHeight() override;
    std::vector<ItemStack>& getHeldEquipment() override;
    std::vector<ItemStack>& getArmorInventoryList() override; 
    ItemStack getItemStackFromSlot(EntityEquipmentSlot slotIn) override;
    void setItemStackToSlot(EntityEquipmentSlot slotIn, ItemStack stack) override;
    static EntityEquipmentSlot getSlotForItemStack(ItemStack stack);
    static Item* getArmorByChance(EntityEquipmentSlot slotIn, int32_t chance);
    virtual IEntityLivingData* onInitialSpawn(DifficultyInstance difficulty, IEntityLivingData* livingdata);
    bool canBeSteered();
    void enablePersistence();
    void setDropChance(EntityEquipmentSlot slotIn, float chance);
    bool canPickUpLoot() const;
    void setCanPickUpLoot(bool canPickup);
    bool isNoDespawnRequired() const;
    bool processInitialInteract(EntityPlayer* player, EnumHand hand) final;
    void clearLeashed(bool sendPacket, bool dropLead);
    bool canBeLeashedTo(EntityPlayer* player);
    bool getLeashed() const;
    Entity* getLeashHolder() const;
    void setLeashHolder(Entity* entityIn, bool sendAttachNotification);
    bool startRiding(Entity* entityIn, bool force) override;
    bool replaceItemInInventory(int32_t inventorySlot, ItemStack itemStackIn) override;
    bool canPassengerSteer() override;
    static bool isItemStackInSlot(EntityEquipmentSlot slotIn, ItemStack stack);
    bool isServerWorld();
    void setNoAI(bool disable);
    void setLeftHanded(bool leftHanded);
    bool isAIDisabled();
    bool isLeftHanded();
    EnumHandSide getPrimaryHand() override;


    int32_t livingSoundTime;
protected:
    virtual void initEntityAI();
    void applyEntityAttributes() override;
    virtual PathNavigate* createNavigator(World* worldIn);
    EntityBodyHelper createBodyHelper();
    void entityInit() override;
    void playHurtSound(DamageSource::DamageSource source) override;
    int32_t getExperiencePoints(EntityPlayer* player) override;
    float updateDistance(float p_110146_1_, float p_110146_2_) override;
    virtual SoundEvent getAmbientSound();
    Item* getDropItem();
    void dropFewItems(bool wasRecentlyHit, int32_t lootingModifier) override;
    virtual std::optional<ResourceLocation> getLootTable();
    void dropLoot(bool wasRecentlyHit, int32_t lootingModifier, DamageSource::DamageSource source) override;
    void updateEquipmentIfNeeded(EntityItem* itemEntity);
    bool canEquipItem(ItemStack stack);
    bool canDespawn();
    virtual void despawnEntity();
    void updateEntityActionState() override;
    virtual void updateAITasks();
    void dropEquipment(bool wasRecentlyHit, int32_t lootingModifier) override;
    virtual void setEquipmentBasedOnDifficulty(DifficultyInstance difficulty);
    void setEnchantmentBasedOnDifficulty(DifficultyInstance difficulty);
    virtual bool processInteract(EntityPlayer* player, EnumHand hand);
    virtual void updateLeashedState();


    int32_t experienceValue;
    EntityMoveHelper moveHelper;
    EntityJumpHelper jumpHelper;
    PathNavigate* navigator;
    EntityAITasks tasks;
    EntityAITasks targetTasks;
    std::vector<float> inventoryHandsDropChances;
    std::vector<float> inventoryArmorDropChances;
private:
    void applyEntityAI();
    float updateRotation(float angle, float targetAngle, float maxIncrease) const;
    void recreateLeash();

    static DataParameter AI_FLAGS;
    EntityLookHelper lookHelper;
    EntityBodyHelper bodyHelper;
    EntityLivingBase* attackTarget;
    EntitySenses senses;
    std::vector<ItemStack> inventoryHands;
    std::vector<ItemStack> inventoryArmor;
    bool bcanPickUpLoot;
    bool persistenceRequired;
    std::unordered_map<PathNodeType,float> mapPathPriority;
    std::optional<ResourceLocation> deathLootTable;
    int64_t deathLootTableSeed;
    bool isLeashed;
    Entity* leashHolder;
    NBTTagCompound* leashNBTTag;
};

