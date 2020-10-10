#include "PropertyInteger.h"
#include <stdexcept>

std::set<std::any> PropertyInteger::getAllowedValues() {
    return allowedValues;
}

PropertyInteger PropertyInteger::create(const std::string &name, int32_t min, int32_t max) {
    return PropertyInteger(name, min, max);
}

std::optional<std::any> PropertyInteger::parseValue(const std::string &value) {
    return allowedValues.find(std::stoi(value)) != allowedValues.end()
               ? std::optional<std::any>(std::stoi(value))
               : std::nullopt;
}

std::string PropertyInteger::getName(std::any value) {
    return std::to_string(std::any_cast<int32_t>(value));
}

PropertyInteger::PropertyInteger(const std::string &name, int32_t min, int32_t max) :
    PropertyHelper<int32_t>(name) {
    if (min < 0) {
        throw std::logic_error("Min value of " + name + " must be 0 or greater");
    } else if (max <= min) {
        throw std::logic_error("Max value of " + name + " must be greater than min (" + std::to_string(min) + ")");
    } else {
        for (int i = min; i <= max; ++i) {
            allowedValues.emplace(i);
        }
    }
}
