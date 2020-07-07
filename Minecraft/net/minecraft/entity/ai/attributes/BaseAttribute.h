#pragma once
#include "IAttribute.h"

class BaseAttribute :public IAttribute
{
public:
    std::string getName() const override;
    double getDefaultValue() const override;
    bool getShouldWatch() const override;
    BaseAttribute* setShouldWatch(bool shouldWatchIn);
    IAttribute* getParent() const override;
    std::size_t hash_code() const;
    friend bool operator==(const BaseAttribute& lhs,const BaseAttribute& rhs) noexcept;

protected:
    BaseAttribute(IAttribute* parentIn, std::string_view unlocalizedNameIn, double defaultValueIn);
private:
    IAttribute* parent;
    std::string translationKey;
    double defaultValue;
    bool shouldWatch;
};

namespace std
{
    template<> struct hash<BaseAttribute>
    {
        std::size_t operator()(BaseAttribute const& s) const noexcept
        {
            return s.hash_code();
        }
    };
}