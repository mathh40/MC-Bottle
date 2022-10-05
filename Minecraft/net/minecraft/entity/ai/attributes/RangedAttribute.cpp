#include "RangedAttribute.h"

#include "math/MathHelper.h"

#include <stdexcept>

RangedAttribute::RangedAttribute(IAttribute *parentIn, std::string_view unlocalizedNameIn, double defaultValue,
                                 double minimumValueIn, double maximumValueIn)
    : BaseAttribute(parentIn, unlocalizedNameIn, defaultValue), minimumValue(minimumValueIn),
      maximumValue(maximumValueIn)
{
    if (minimumValueIn > maximumValueIn)
    {
        throw std::logic_error("Minimum value cannot be bigger than maximum value!");
    }
    else if (defaultValue < minimumValueIn)
    {
        throw std::logic_error("Default value cannot be lower than minimum value!");
    }
    else if (defaultValue > maximumValueIn)
    {
        throw std::logic_error("Default value cannot be bigger than maximum value!");
    }
}

RangedAttribute RangedAttribute::setDescription(std::string_view descriptionIn)
{
    description = descriptionIn;
    return *this;
}

std::string RangedAttribute::getDescription() const
{
    return description;
}

double RangedAttribute::clampValue(double value) const
{
    value = MathHelper::clamp(value, minimumValue, maximumValue);
    return value;
}

void RangedAttribute::setMinValue(double minValue)
{
    minimumValue = minValue;
}

void RangedAttribute::setMaxValue(double maxValue)
{
    maximumValue = maxValue;
}
