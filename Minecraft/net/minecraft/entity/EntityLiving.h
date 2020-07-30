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
    int32_t livingSoundTime;
protected:
    int32_t experienceValue;
    EntityMoveHelper moveHelper;
    EntityJumpHelper jumpHelper;
    PathNavigate* navigator;
    EntityAITasks tasks;
    EntityAITasks targetTasks;
    std::vector<float> inventoryHandsDropChances;
    std::vector<float> inventoryArmorDropChances;
private:
    static DataParameter AI_FLAGS;
    EntityLookHelper lookHelper;
    EntityBodyHelper bodyHelper;
    EntityLivingBase* attackTarget;
    EntitySenses senses;
    std::vector<ItemStack> inventoryHands;
    std::vector<ItemStack> inventoryArmor;
    bool canPickUpLoot;
    bool persistenceRequired;
    std::unordered_map<> mapPathPriority;
    ResourceLocation deathLootTable;
    int64_t deathLootTableSeed;
    bool isLeashed;
    Entity* leashHolder;
    NBTTagCompound* leashNBTTag;
};
