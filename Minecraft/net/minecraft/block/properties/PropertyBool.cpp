#include "PropertyBool.h"

std::unique_ptr<PropertyBool> PropertyBool::create(std::string_view name)
{
    return std::make_unique<PropertyBool>(name);
}

std::optional<std::any> PropertyBool::parseValue(std::string_view value)
{
    return "true" != value && "false" != value ? std::nullopt
                                               : std::optional<std::any>([&]() { return value == "true"; });
}

std::string PropertyBool::getName(std::any value) const
{
    if (std::any_cast<bool>(value))
    {
        return "true";
    }
    return "false";
}

PropertyBool::PropertyBool(std::string_view name) : PropertyHelper<bool>(name)
{
}

bool operator==(const PropertyBool &lhs, const PropertyBool &rhs)
{
    return lhs.allowedValues == rhs.allowedValues;
}
