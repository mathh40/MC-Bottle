#include "StatBasic.h"

StatBasic::StatBasic(std::string_view statIdIn, ITextComponent *statNameIn, IStatType *typeIn)
    : StatBase(statIdIn, statNameIn, typeIn)
{
}

StatBasic::StatBasic(std::string_view statIdIn, ITextComponent *statNameIn) : StatBase(statIdIn, statNameIn)
{
}

StatBase &StatBasic::registerStat()
{
    StatBase::registerStat();
    StatList::BASIC_STATS.add(this);
    return *this;
}
