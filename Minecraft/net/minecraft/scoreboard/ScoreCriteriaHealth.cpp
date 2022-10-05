#include "ScoreCriteriaHealth.h"

ScoreCriteriaHealth::ScoreCriteriaHealth(std::string_view name) : ScoreCriteria(name)
{
}

bool ScoreCriteriaHealth::isReadOnly()
{
    return true;
}

IScoreCriteria::EnumRenderType ScoreCriteriaHealth::getRenderType()
{
    return IScoreCriteria::EnumRenderType::HEARTS;
}
