#include "BaseAttribute.h"
#include <stdexcept>

std::string BaseAttribute::getName() const
{
    return translationKey;
}

double BaseAttribute::getDefaultValue() const
{
    return defaultValue;
}

bool BaseAttribute::getShouldWatch() const
{
    return shouldWatch;
}

BaseAttribute * BaseAttribute::setShouldWatch(bool shouldWatchIn)
{
    shouldWatch = shouldWatchIn;
    return this;
}

IAttribute * BaseAttribute::getParent() const
{
    return parent;
}

std::size_t BaseAttribute::hash_code() const
{
    return std::hash<std::string>{}(translationKey);
}

BaseAttribute::BaseAttribute(IAttribute *parentIn, std::string_view unlocalizedNameIn, double defaultValueIn)
    :parent(parentIn),translationKey(unlocalizedNameIn),defaultValue(defaultValueIn)
{
    if(unlocalizedNameIn.empty())
    {
        throw std::logic_error("Name cannot be null!");
    }
}

bool operator==(const BaseAttribute &lhs, const BaseAttribute &rhs)
{
    return lhs.translationKey == rhs.getName();
}
