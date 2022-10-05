#pragma once
#include "IScoreCriteria.h"

class TextFormatting;

class ScoreCriteriaColored : public IScoreCriteria
{
  public:
    ScoreCriteriaColored(std::string name, const TextFormatting &format);
    std::string getName() override;
    bool isReadOnly() override;
    IScoreCriteria::EnumRenderType getRenderType() override;

  private:
    std::string goalName;
};
