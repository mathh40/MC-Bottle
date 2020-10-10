#include "PropertyBool.h"

std::unique_ptr<PropertyBool> PropertyBool::create(const std::string &name) {
    return std::make_unique<PropertyBool>(name);
}

std::optional<std::any> PropertyBool::parseValue(const std::string &value) {
    return "true" != (value) && "false" != (value)
               ? std::nullopt
               : std::optional<std::any>([&]()
               {
                   return value == "true";
               });
}

std::string PropertyBool::getName(std::any value) {
    if (std::any_cast<bool>(value)) {
        return "true";
    }
    return "false";
}

PropertyBool::PropertyBool(const std::string &name) :
    PropertyHelper<bool>(name, false) {
}

bool operator==(const PropertyBool &lhs, const PropertyBool &rhs) {
    return lhs.allowedValues == rhs.allowedValues;
}
