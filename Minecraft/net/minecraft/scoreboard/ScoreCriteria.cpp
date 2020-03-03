#include "ScoreCriteria.h"

ScoreCriteria::ScoreCriteria(std::string_view name)
{
   dummyName = name;
   IScoreCriteria::INSTANCES.put(name, this);
}

std::string ScoreCriteria::getName()
{
   return dummyName;
}

bool ScoreCriteria::isReadOnly()
{
   return false;
}

IScoreCriteria::EnumRenderType ScoreCriteria::getRenderType()
{
   return IScoreCriteria::EnumRenderType::INTEGER;
}
