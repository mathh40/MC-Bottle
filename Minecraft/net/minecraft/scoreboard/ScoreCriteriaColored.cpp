#include "ScoreCriteriaColored.h"
#include "text/TextFormatting.h"

ScoreCriteriaColored::ScoreCriteriaColored(std::string name, const TextFormatting& format)
   :goalName(name + format.getFriendlyName())
{
   IScoreCriteria::INSTANCES.put(goalName, this);
}

std::string ScoreCriteriaColored::getName()
{
   return goalName;
}

bool ScoreCriteriaColored::isReadOnly()
{
   return false;
}

IScoreCriteria::EnumRenderType ScoreCriteriaColored::getRenderType()
{
   return IScoreCriteria::EnumRenderType::INTEGER;
}
