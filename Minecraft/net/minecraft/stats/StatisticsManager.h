#pragma once
#include <unordered_map>

class EntityPlayer;
class StatBase;

class StatisticsManager
{
  protected:
    std::unordered_map<StatBase, TupleIntJsonSerializable> statsData;

  public:
    void increaseStat(EntityPlayer *player, StatBase stat, int32_t amount);
    virtual void unlockAchievement(EntityPlayer *playerIn, StatBase statIn, int32_t p_150873_3_);
    int32_t readStat(StatBase stat);
};
