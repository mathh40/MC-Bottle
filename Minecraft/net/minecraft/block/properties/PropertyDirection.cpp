#include "PropertyDirection.h"

PropertyDirection PropertyDirection::create(std::string_view name)
{
    return create(name, [] { return true; });
}

PropertyDirection PropertyDirection::create(std::string_view name, std::function<bool()> filter)
{
    return create(name, Collections2.filter(Lists.newArrayList(EnumFacing::values()), filter));
}

PropertyDirection PropertyDirection::create(std::string_view name, std::set<EnumFacing> &values)
{
    return PropertyDirection(name, values);
}

PropertyDirection::PropertyDirection(std::string_view name, std::set<EnumFacing> &values)
    : PropertyEnum<EnumFacing>(name, values)
{
}
