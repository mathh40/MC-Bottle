#pragma once
#include "../../../../../../crossguid/include/crossguid/guid.hpp"

class AttributeModifier
{
public:
    AttributeModifier(std::string_view nameIn, double amountIn, int32_t operationIn);
    AttributeModifier(xg::Guid& idIn, std::string_view nameIn, double amountIn, int32_t operationIn);
    xg::Guid getID() const;
    std::string getName() const;
    int32_t getOperation() const;
    double getAmount() const;
    bool isSaved() const;
    AttributeModifier setSaved(bool saved);
    std::size_t hash_code() const;
    std::string toString();

    friend bool operator==(const AttributeModifier& lhs,const AttributeModifier& rhs);
private:
    double amount;
    int32_t operation;
    std::string name;
    xg::Guid id;
    bool bisSaved;
};

namespace std
{
    template<> struct hash<AttributeModifier>
    {
        std::size_t operator()(AttributeModifier const& s) const noexcept
        {
            return s.hash_code();
        }
    };
}


