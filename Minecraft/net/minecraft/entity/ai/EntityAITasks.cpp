#include "EntityAITasks.h"

#include "../../../../../spdlog/include/spdlog/spdlog.h"

std::shared_ptr<spdlog::logger> EntityAITasks::LOGGER = spdlog::get("Minecraft")->clone("EntityAITasks");

EntityAITasks::EntityAITaskEntry::EntityAITaskEntry(int32_t priorityIn, EntityAIBase *task)
    :priority(priorityIn),action(task)
{
}

bool operator==(const EntityAITasks::EntityAITaskEntry &lhs, const EntityAITasks::EntityAITaskEntry &rhs)
{
    return lhs.action == rhs.action;
}

EntityAITasks::EntityAITasks(const Profiler &profilerIn)
    :profiler(profilerIn)
{
}

void EntityAITasks::addTask(int32_t priority, EntityAIBase *task)
{
    taskEntries.emplace(new EntityAITasks::EntityAITaskEntry(priority, task));
}

void EntityAITasks::removeTask(EntityAIBase *task)
{
    auto iterator = taskEntries.begin();

    EntityAITasks::EntityAITaskEntry* entityaitasks$entityaitaskentry;
    EntityAIBase* entityaibase;
    do 
    {
        if (!iterator == taskEntries.end()) 
        {
            return;
        }

        entityaitasks$entityaitaskentry = (EntityAITasks::EntityAITaskEntry*)iterator.next();
        entityaibase = entityaitasks$entityaitaskentry->action;
    }
    while(entityaibase != task);

    if (entityaitasks$entityaitaskentry->isusing) 
    {
        entityaitasks$entityaitaskentry->isusing = false;
        entityaitasks$entityaitaskentry->action->resetTask();
        executingTaskEntries.erase(entityaitasks$entityaitaskentry);
    }

    taskEntries.erase(iterator);
}

void EntityAITasks::onUpdateTasks()
{
    profiler.startSection("goalSetup");
    Iterator iterator;
    EntityAITasks::EntityAITaskEntry* entityaitasks$entityaitaskentry;
    if (tickCount++ % tickRate == 0) 
    {
        iterator = taskEntries.begin();

    
        while(true) 
        {
            do 
            {
                while(true) 
                {
                    if (!iterator.hasNext()) 
                    {
                        goto label57;
                    }

                    entityaitasks$entityaitaskentry = (EntityAITasks.EntityAITaskEntry)iterator.next();
                    if (entityaitasks$entityaitaskentry->isusing) 
                    {
                        break;
                    }

                    if (canUse(entityaitasks$entityaitaskentry) && entityaitasks$entityaitaskentry->action->shouldExecute()) 
                    {
                        entityaitasks$entityaitaskentry->isusing = true;
                        entityaitasks$entityaitaskentry->action->startExecuting();
                        executingTaskEntries.emplace(entityaitasks$entityaitaskentry);
                    }
                }
            }
            while(canUse(entityaitasks$entityaitaskentry) && canContinue(entityaitasks$entityaitaskentry));

            entityaitasks$entityaitaskentry->isusing = false;
            entityaitasks$entityaitaskentry->action->resetTask();
            executingTaskEntries.erase(entityaitasks$entityaitaskentry);
        }
    }
    else 
    {
        iterator = executingTaskEntries.iterator();

        while(iterator.hasNext()) 
        {
            entityaitasks$entityaitaskentry = (EntityAITasks.EntityAITaskEntry)iterator.next();
            if (!canContinue(entityaitasks$entityaitaskentry)) 
            {
                entityaitasks$entityaitaskentry->isusing = false;
                entityaitasks$entityaitaskentry->action->resetTask();
                iterator.remove();
            }
        }
    }

label57:
    profiler.endSection();
    if (!executingTaskEntries.empty()) 
    {
        profiler.startSection("goalTick");
        iterator = executingTaskEntries.iterator();

        while(iterator.hasNext()) {
            entityaitasks$entityaitaskentry = (EntityAITasks::EntityAITaskEntry*)iterator.next();
            entityaitasks$entityaitaskentry->action->updateTask();
        }

        profiler.endSection();
    }
}

bool EntityAITasks::isControlFlagDisabled(int32_t p_188528_1_)
{
    return (disabledControlFlags & p_188528_1_) > 0;
}

void EntityAITasks::disableControlFlag(int32_t p_188526_1_)
{
    disabledControlFlags |= p_188526_1_;
}

void EntityAITasks::enableControlFlag(int32_t p_188525_1_)
{
    disabledControlFlags &= ~p_188525_1_;
}

void EntityAITasks::setControlFlag(int32_t p_188527_1_, bool p_188527_2_)
{
    if (p_188527_2_) 
    {
        enableControlFlag(p_188527_1_);
    }
    else 
    {
        disableControlFlag(p_188527_1_);
    }
}

bool EntityAITasks::canContinue(EntityAITasks::EntityAITaskEntry *taskEntry)
{
    return taskEntry->action->shouldContinueExecuting();
}

bool EntityAITasks::canUse(EntityAITasks::EntityAITaskEntry *taskEntry)
{
    if (executingTaskEntries.empty()) 
    {
        return true;
    }
    else if (isControlFlagDisabled(taskEntry->action->getMutexBits())) 
    {
        return false;
    }
    else 
    {
        Iterator var2 = executingTaskEntries.iterator();

        while(var2.hasNext()) 
        {
            EntityAITasks::EntityAITaskEntry* entityaitasks$entityaitaskentry = (EntityAITasks::EntityAITaskEntry*)var2.next();
            if (entityaitasks$entityaitaskentry != taskEntry) 
            {
                if (taskEntry->priority >= entityaitasks$entityaitaskentry.priority) 
                {
                    if (!areTasksCompatible(taskEntry, entityaitasks$entityaitaskentry)) 
                    {
                        return false;
                    }
                }
                else if (!entityaitasks$entityaitaskentry.action.isInterruptible()) 
                {
                    return false;
                }
            }
        }

        return true;
    }
}

bool EntityAITasks::areTasksCompatible(EntityAITasks::EntityAITaskEntry *taskEntry1,
    EntityAITasks::EntityAITaskEntry *taskEntry2)
{
    return (taskEntry1->action->getMutexBits() & taskEntry2->action->getMutexBits()) == 0;
}
