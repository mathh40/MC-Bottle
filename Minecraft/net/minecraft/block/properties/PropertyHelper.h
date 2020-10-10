#pragma once
#include "IProperty.h"

template <typename T>
class PropertyHelper : public IProperty {
protected:
    ~PropertyHelper() = default;
    PropertyHelper(std::string nameIn);
public:
    std::string getName() override;
    std::any getValueClass() override;
    std::string toString();
private:
    T valueClass;
    std::string name;
};

template <typename T>
PropertyHelper<T>::PropertyHelper(std::string nameIn) :
    name(nameIn) {

}

template <typename T>
std::string PropertyHelper<T>::getName() {
    return name;
}

template <typename T>
std::any PropertyHelper<T>::getValueClass() {
    return valueClass;
}

template <typename T>
std::string PropertyHelper<T>::toString() {
    return fmt::to_string(this).add("name", name).add("clazz", valueClass).add("values", getAllowedValues()).toString();
}

template <typename T>
bool operator==(const PropertyHelper<T> &lhs, const PropertyHelper<T> &rhs) {
    return lhs.getValueClass() == rhs.getValueClass() && lhs.getName() == rhs.getName();
}
