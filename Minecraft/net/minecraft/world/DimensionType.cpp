#include "DimensionType.h"
#include <stdexcept>

DimensionType DimensionType::OVERWORLD(0, "overworld", "", WorldProviderSurface);
DimensionType DimensionType::NETHER(-1, "the_nether", "_nether", WorldProviderHell);
DimensionType DimensionType::THE_END(1, "the_end", "_end", WorldProviderEnd);

uint32_t DimensionType::getId() const
{
	return id;
}

std::string DimensionType::getName() const
{
	return name;
}

std::string DimensionType::getSuffix() const
{
	return suffix;
}

WorldProvider* DimensionType::createDimension() const
{
	return clazz.get();
}

DimensionType& DimensionType::getById(uint32_t id) const
{
	for(auto dimensiontype : value)
	{
		if (dimensiontype->getId() == id) 
		{
			return *dimensiontype;
		}
	}

	throw std::runtime_error("Invalid dimension id " + std::to_string(id));
}

DimensionType& DimensionType::byName(std::string nameIn) const
{
	for (auto dimensiontype : value)
	{
		if (dimensiontype->getName() == name)
		{
			return *dimensiontype;
		}
	}

	throw std::runtime_error("Invalid dimension " + nameIn);
}

DimensionType::DimensionType(uint32_t idIn, std::string nameIn, std::string suffixIn, std::unique_ptr<WorldProvider>& clazzIn)
	:id(idIn), name (nameIn), suffix(suffixIn), clazz(std::move(clazzIn))
{
	value.emplace_back(*this);
}
