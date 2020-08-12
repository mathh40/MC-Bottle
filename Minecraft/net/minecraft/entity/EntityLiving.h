#pragma once
#include "EntityBodyHelper.h"
#include "EntityLivingBase.h"
#include "../pathfinding/PathNavigate.h"
#include "ai/EntityAITasks.h"
#include "ai/EntityJumpHelper.h"
#include "ai/EntityLookHelper.h"
#include "ai/EntityMoveHelper.h"

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
    void setAttackTarget(EntityLivingBase* entitylivingbaseIn);
    bool canAttackClass(std::type_index cls);
    void eatGrassBonus();
    int32_t getTalkInterval();
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
    int32_t getVerticalFaceSpeed();
    int32_t getHorizontalFaceSpeed();
    void faceEntity(Entity* entityIn, float maxYawIncrease, float maxPitchIncrease);
    bool getCanSpawnHere();
    bool isNotColliding();
    float getRenderSizeModifier();
    int32_t getMaxSpawnedInChunk();
    int32_t getMaxFallHeight() override;
    std::vector<ItemStack>& getHeldEquipment() override;
    std::vector<ItemStack>& getArmorInventoryList() override; 

    int32_t livingSoundTime;
protected:
    virtual void initEntityAI();
    void applyEntityAttributes() override;
    PathNavigate* createNavigator(World* worldIn);
    EntityBodyHelper createBodyHelper();
    void entityInit() override;
    void playHurtSound(DamageSource::DamageSource source) override;
    int32_t getExperiencePoints(EntityPlayer* player) override;
    float updateDistance(float p_110146_1_, float p_110146_2_) override;
    SoundEvent getAmbientSound();
    Item* getDropItem();
    void dropFewItems(bool wasRecentlyHit, int32_t lootingModifier) override;
    std::optional<ResourceLocation> getLootTable();
    void dropLoot(bool wasRecentlyHit, int32_t lootingModifier, DamageSource::DamageSource source) override;
    void updateEquipmentIfNeeded(EntityItem* itemEntity);
    bool canEquipItem(ItemStack stack);
    bool canDespawn();
    void despawnEntity();
    void updateEntityActionState() override;
    void updateAITasks();



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

    static DataParameter AI_FLAGS;
    EntityLookHelper lookHelper;
    EntityBodyHelper bodyHelper;
    EntityLivingBase* attackTarget;
    EntitySenses senses;
    std::vector<ItemStack> inventoryHands;
    std::vector<ItemStack> inventoryArmor;
    bool canPickUpLoot;
    bool persistenceRequired;
    std::unordered_map<PathNodeType,float> mapPathPriority;
    std::optional<ResourceLocation> deathLootTable;
    int64_t deathLootTableSeed;
    bool isLeashed;
    Entity* leashHolder;
    NBTTagCompound* leashNBTTag;
};

