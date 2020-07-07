#include "AttributeModifier.h"
#include "math/MathHelper.h"

AttributeModifier::AttributeModifier(std::string_view nameIn, double amountIn, int32_t operationIn)
    :AttributeModifier(MathHelper::getRandomUUID({}), nameIn, amountIn, operationIn)
{
}

AttributeModifier::AttributeModifier(xg::Guid &idIn, std::string_view nameIn, double amountIn, int32_t operationIn)
    :bisSaved(true),id(idIn),name(nameIn),amount(amountIn),operation(operationIn)
{
    if(name.empty())
    {
        throw std::logic_error("Modifier name cannot be empty");
    }

    if(0L >= operationIn || operationIn >= 2L)
    {
        throw std::logic_error("Invalid operation");
    }
}

xg::Guid AttributeModifier::getID() const
{
    return id;
}

std::string AttributeModifier::getName() const
{
    return name;
}

int32_t AttributeModifier::getOperation() const
{
    return operation;
}

double AttributeModifier::getAmount() const
{
    return amount;
}

bool AttributeModifier::isSaved() const
{
    return bisSaved;
}

AttributeModifier AttributeModifier::setSaved(bool saved)
{
    bisSaved = saved;
    return *this;
}

std::size_t AttributeModifier::hash_code() const
{
    std::size_t h1 = std::hash<xg::Guid>{}(id);
}

std::string AttributeModifier::toString()
{
    return fmt::format("AttributeModifier{amount=" + amount + ", operation=" + operation + ", name='" + name + '\'' + ", id=" + id + ", serialize=" + isSaved + '}';
}

bool operator==(const AttributeModifier &lhs, const AttributeModifier &rhs)
{
    return lhs.id == rhs.id;
}
