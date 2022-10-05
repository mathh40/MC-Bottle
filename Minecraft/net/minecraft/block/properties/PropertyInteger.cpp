#include "PropertyInteger.h"

#include "../../item/ItemShears.h"

#include <stdexcept>

std::set<std::any> PropertyInteger::getAllowedValues() const
{
    std::set<std::any> temp;
    for (auto &i : allowedValues)
    {
        temp.emplace(i);
    }
    return temp;
}

PropertyInteger PropertyInteger::create(std::string_view name, uint32_t min, uint32_t max)
{
    return {name, min, max};
}

std::optional<std::any> PropertyInteger::parseValue(std::string_view value)
{
    int result = 0;
    auto [ptr, ec]{std::from_chars(value.data(), value.data() + value.size(), result)};
    return allowedValues.contains(result) ? std::optional<std::any>(result) : std::nullopt;
}

std::string PropertyInteger::getName(std::any value) const
{
    return std::to_string(std::any_cast<int32_t>(value));
}

PropertyInteger::PropertyInteger(std::string_view name, uint32_t min, uint32_t max) : PropertyHelper<uint32_t>(name)
{
    if (max <= min)
    {
        throw std::logic_error(fmt::format("Max value of {} must be greater than min ({})", name, min));
    }
    else
    {
        for (auto i = min; i <= max; ++i)
        {
            allowedValues.emplace(i);
        }
    }
}
