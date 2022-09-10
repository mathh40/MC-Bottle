#pragma once
#include "IProperty.h"
#include "../../../../../fmt/include/fmt/format.h"

template <typename T>
class PropertyHelper : public IProperty {
protected:
    ~PropertyHelper() override = default;
    PropertyHelper(std::string_view nameIn);
public:
    std::string getName() const override;
    std::any getValueClass() const override;
    std::string toString() const;
private:
    T valueClass;
    std::string name;
};

template <typename T>
PropertyHelper<T>::PropertyHelper(std::string_view nameIn) :
    name(nameIn) {

}

template <typename T>
std::string PropertyHelper<T>::getName() const{
    return name;
}

template <typename T>
std::any PropertyHelper<T>::getValueClass() const{
    return valueClass;
}

template <typename T>
std::string PropertyHelper<T>::toString() const{
  return fmt::format("name:{} clazz:{} values:{}", name, valueClass, getAllowedValues());
}

template <typename T>
bool operator==(const PropertyHelper<T> &lhs, const PropertyHelper<T> &rhs) {
    return lhs.getValueClass() == rhs.getValueClass() && lhs.getName() == rhs.getName();
}
