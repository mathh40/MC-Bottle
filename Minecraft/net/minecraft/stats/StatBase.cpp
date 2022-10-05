#include "StatBase.h"

#include "text/Style.h"
#include "text/TextFormatting.h"

#include <stdexcept>

StatBase::StatBase(std::string statIdIn, ITextComponent *statNameIn, IStatType *formatterIn)
    : statId(statIdIn), statName(statNameIn), formatter(formatterIn), objectiveCriteria()
{
    IScoreCriteria::INSTANCES.put(objectiveCriteria->getName(), objectiveCriteria);
}

StatBase::StatBase(std::string_view statIdIn, ITextComponent *statNameIn)
    : StatBase(statIdIn, statNameIn, simpleStatType)
{
}

StatBase &StatBase::initIndependentStat()
{
    isIndependent = true;
    return *this;
}

StatBase &StatBase::registerStat()
{
    if (StatList::ID_TO_STAT_MAP::containsKey(statId))
    {
        throw std::runtime_error("Duplicate stat id: \"" + ((StatBase)StatList.ID_TO_STAT_MAP.get(statId)).statName +
                                 "\" and \"" + statName + "\" at id " + statId);
    }
    else
    {
        StatList::ALL_STATS.add(this);
        StatList::ID_TO_STAT_MAP.put(statId, this);
        return *this;
    }
}

std::string StatBase::format(int32_t number)
{
    return formatter.format(number);
}

ITextComponent *StatBase::getStatName() const
{
    const auto itextcomponent = statName->createCopy();
    itextcomponent->getStyle().setColor(TextFormatting::GRAY);
    return itextcomponent;
}

std::string StatBase::toString()
{
    return "Stat{id=" + statId + ", nameId=" + statName + ", awardLocallyOnly=" + isIndependent +
           ", formatter=" + formatter + ", objectiveCriteria=" + objectiveCriteria + '}';
}

bool operator==(const StatBase &a, const StatBase &b)
{
}

IScoreCriteria *StatBase::getCriteria()
{
    return objectiveCriteria;
}
Class *StatBase::getSerializableClazz()
{
    return serializableClazz;
}
