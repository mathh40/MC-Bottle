#pragma once
#include "PropertyHelper.h"
#include <unordered_map>
#include <stdexcept>

template <typename T>
class PropertyEnum : public PropertyHelper<T> {
public:
    std::set<std::any> getAllowedValues() override;
    std::optional<std::any> parseValue(const std::string &value) override;
    std::string getName(std::any value) override;

    static PropertyEnum<T> create(const std::string &name) {
        return create(name, []() { return true; });
    }

    static PropertyEnum<T> create(const std::string &name, std::set<T> &values);

protected:
    ~PropertyEnum() = default;
    PropertyEnum(std::string name, std::set<T> &allowedValues);
private:
    std::set<T> allowedValues;
    std::unordered_map<std::string, T> nameToValue;
};

template <typename T>
std::set<std::any> PropertyEnum<T>::getAllowedValues() {
    return allowedValues;
}

template <typename T>
std::optional<std::any> PropertyEnum<T>::parseValue(const std::string &value) {
    return std::optional<std::any>(nameToValue[value]);
}

template <typename T>
std::string PropertyEnum<T>::getName(std::any value) {
    return std::any_cast<T>(value).getName();
}

template <typename T>
PropertyEnum<T> PropertyEnum<T>::create(const std::string &name, std::set<T> &values) {
    return PropertyEnum(name, values);
}

template <typename T>
PropertyEnum<T>::PropertyEnum(std::string name, std::set<T> &allowedValues) :
    PropertyHelper<T>(name), allowedValues(allowedValues) {

    for (auto t : allowedValues) {
        auto s = t.getName();
        if (nameToValue.find(s) != nameToValue.end()) {
            throw std::logic_error("Multiple values have the same name '" + s + "'");
        }

        nameToValue.emplace(s, t);
    }
}
