#include "StatisticsManager.h"
#include "StatBase.h"

void StatisticsManager::increaseStat(EntityPlayer* player, StatBase stat, int32_t amount)
{
	unlockAchievement(player, stat, readStat(stat) + amount);
}

void StatisticsManager::unlockAchievement(EntityPlayer* playerIn, StatBase statIn, int32_t p_150873_3_)
{
	TupleIntJsonSerializable tupleintjsonserializable = (TupleIntJsonSerializable)this.statsData.get(statIn);
	if (tupleintjsonserializable == nullptr) 
	{
		tupleintjsonserializable = new TupleIntJsonSerializable();
		statsData.put(statIn, tupleintjsonserializable);
	}

	tupleintjsonserializable.setIntegerValue(p_150873_3_);
}

int32_t StatisticsManager::readStat(StatBase stat)
{
	TupleIntJsonSerializable tupleintjsonserializable = (TupleIntJsonSerializable)statsData.get(stat);
      return tupleintjsonserializable == nullptr ? 0 : tupleintjsonserializable.getIntegerValue();
}
