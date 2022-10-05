#pragma once
#include "../stats/StatBase.h"
#include "ScoreCriteria.h"

class ScoreCriteriaStat : public ScoreCriteria
{
  public:
    ScoreCriteriaStat(const StatBase &statIn);

  private:
    StatBase stat;
};
