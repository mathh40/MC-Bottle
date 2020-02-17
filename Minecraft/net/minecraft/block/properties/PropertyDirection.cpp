#include "PropertyDirection.h"

PropertyDirection PropertyDirection::create(const std::string& name)
{
	return create(name, Predicates.alwaysTrue());
}

PropertyDirection PropertyDirection::create(const std::string& name, Predicate filter)
{
	return create(name, Collections2.filter(Lists.newArrayList(EnumFacing.values()), filter));
}

PropertyDirection PropertyDirection::create(const std::string& name, std::set<EnumFacing>& values)
{
	return PropertyDirection(name, values);
}

PropertyDirection::PropertyDirection(const std::string& name, std::set<EnumFacing>& values)
	:PropertyEnum<EnumFacing>(name, values)
{

}
