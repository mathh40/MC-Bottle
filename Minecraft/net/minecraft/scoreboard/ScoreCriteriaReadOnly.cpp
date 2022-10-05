#include "ScoreCriteriaReadOnly.h"

ScoreCriteriaReadOnly::ScoreCriteriaReadOnly(std::string_view name) : ScoreCriteria(name)
{
}

bool ScoreCriteriaReadOnly::isReadOnly()
{
    return true;
}
