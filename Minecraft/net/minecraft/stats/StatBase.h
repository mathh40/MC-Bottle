#pragma once
#include "IStatType.h"

#include <string>

class ITextComponent;
class IScoreCriteria;

class StatBase
{
  public:
    StatBase(std::string_view statIdIn, ITextComponent *statNameIn, IStatType *formatterIn);
    StatBase(std::string_view statIdIn, ITextComponent *statNameIn);
    StatBase &initIndependentStat();
    virtual StatBase &registerStat();
    std::string format(int32_t number);
    ITextComponent *getStatName() const;
    std::string toString();
    IScoreCriteria *getCriteria();
    Class *getSerializableClazz();

    static IStatType *timeStatType;
    static IStatType *distanceStatType;
    static IStatType *divideByTen;
    static IStatType *simpleStatType;
    bool isIndependent;
    std::string statId;

    friend bool operator==(const StatBase &a, const StatBase &b);

  private:
    ITextComponent *statName;
    IStatType *formatter;
    IScoreCriteria *objectiveCriteria;
    Class serializableClazz;
    static NumberFormat numberFormat;
    static DecimalFormat decimalFormat;
};
