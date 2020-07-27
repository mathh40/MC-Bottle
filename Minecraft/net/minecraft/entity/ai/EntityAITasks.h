#pragma once
#include <unordered_set>
#include "../../../../../spdlog/include/spdlog/logger.h"
#include "../../profiler/Profiler.h"

class EntityAIBase;

class EntityAITasks
{
public:
    class EntityAITaskEntry
    {
    public:
        EntityAITaskEntry(int32_t priorityIn, EntityAIBase* task);       

        EntityAIBase* action;
        int32_t priority;
        bool isusing;
    };

    EntityAITasks(const Profiler& profilerIn);
    void addTask(int32_t priority, EntityAIBase* task);
    void removeTask(EntityAIBase* task);
    void onUpdateTasks();
    bool isControlFlagDisabled(int32_t p_188528_1_);
    void disableControlFlag(int32_t p_188526_1_);
    void enableControlFlag(int32_t p_188525_1_);
    void setControlFlag(int32_t p_188527_1_, bool p_188527_2_);


private:
    bool canContinue(EntityAITasks::EntityAITaskEntry* taskEntry);
    bool canUse(EntityAITasks::EntityAITaskEntry* taskEntry);
    bool areTasksCompatible(EntityAITasks::EntityAITaskEntry* taskEntry1, EntityAITasks::EntityAITaskEntry* taskEntry2);



    static std::shared_ptr<spdlog::logger> EntityAITasks::LOGGER;
    std::unordered_set<EntityAITaskEntry*> taskEntries;
    std::unordered_set<EntityAITaskEntry*> executingTaskEntries;
    Profiler profiler;
    int32_t tickCount;
    int32_t tickRate = 3;
    int32_t disabledControlFlags;
};

bool operator==(const EntityAITasks::EntityAITaskEntry& lhs, const EntityAITasks::EntityAITaskEntry& rhs);
