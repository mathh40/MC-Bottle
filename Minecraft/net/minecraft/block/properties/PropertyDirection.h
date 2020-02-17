#pragma once
#include "../../util/EnumFacing.h"

#include "PropertyEnum.h"


class PropertyDirection final :public PropertyEnum<EnumFacing>
{
public:
	static PropertyDirection create(const std::string& name);
	static PropertyDirection create(const std::string& name, Predicate filter);
	static PropertyDirection create(const std::string& name, std::set<EnumFacing>& values);
protected:
	~PropertyDirection() = default;
	PropertyDirection(const std::string& name, std::set<EnumFacing>& values);
};
