#pragma once
#include "BaseAttribute.h"

class RangedAttribute :public BaseAttribute
{
public:
    RangedAttribute(IAttribute* parentIn, std::string_view unlocalizedNameIn, double defaultValue, double minimumValueIn, double maximumValueIn);
    RangedAttribute setDescription(std::string_view descriptionIn);
    std::string getDescription() const;
    double clampValue(double value) const override;
    void setMinValue(double minValue);
    void setMaxValue(double maxValue);

private:
    double minimumValue;
    double maximumValue;
    std::string description;
};
