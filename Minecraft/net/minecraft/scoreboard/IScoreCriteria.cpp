#include "IScoreCriteria.h"

IScoreCriteria::EnumRenderType INTEGER("integer");
IScoreCriteria::EnumRenderType HEARTS("hearts");

IScoreCriteria::EnumRenderType::EnumRenderType(std::string_view renderTypeIn) : renderType(renderTypeIn)
{
    BY_NAME.insert(getRenderType(), *this);
}

std::string IScoreCriteria::EnumRenderType::getRenderType() const
{
    return renderType;
}

IScoreCriteria::EnumRenderType IScoreCriteria::EnumRenderType::getByName(std::string name)
{
    return BY_NAME.find(name) != BY_NAME.end() ? INTEGER : BY_NAME.at(name);
}
