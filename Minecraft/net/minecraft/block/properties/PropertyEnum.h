#pragma once
#include "PropertyHelper.h"
#include <unordered_map>
#include <stdexcept>

template <typename T>
class PropertyEnum : public PropertyHelper<T> {
public:
  std::set<std::any> getAllowedValues() const override;
    std::optional<std::any> parseValue(std::string_view value) override;
  std::string getName(std::any value) const override;

    static PropertyEnum<T> create(std::string_view name) {
        return create(name, []() { return true; });
    }

    static PropertyEnum<T> create(std::string_view name, std::set<T> &values);

protected:
    ~PropertyEnum() override = default;
    PropertyEnum(std::string_view name, std::set<T> &allowedValues);
private:
    std::set<T> allowedValues;
    std::unordered_map<std::string, T> nameToValue;
};

template <typename T> std::set<std::any> PropertyEnum<T>::getAllowedValues() const {
    return allowedValues;
}

template <typename T>
std::optional<std::any> PropertyEnum<T>::parseValue(std::string_view value) {
    return std::optional<std::any>(nameToValue[value]);
}

template <typename T> std::string PropertyEnum<T>::getName(std::any value) const
{
    return std::any_cast<T>(value).getName();
}

template <typename T>
PropertyEnum<T> PropertyEnum<T>::create(std::string_view name, std::set<T> &values) {
    return PropertyEnum(name, values);
}

template <typename T>
PropertyEnum<T>::PropertyEnum(std::string_view name, std::set<T> &allowedValues) :
    PropertyHelper<T>(name), allowedValues(allowedValues) {

    for (auto t : allowedValues) {
        auto s = t.getName();
        if (nameToValue.contains(s)) {
            throw std::logic_error("Multiple values have the same name '" + s + "'");
        }

        nameToValue.emplace(s, t);
    }
}
