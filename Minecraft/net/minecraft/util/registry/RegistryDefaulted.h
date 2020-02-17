#pragma once
#include "RegistrySimple.h"


template<typename KEY, typename VALUE>
class RegistryDefaulted :public RegistrySimple<KEY,VALUE>
{	
public:
	explicit RegistryDefaulted(VALUE defaultObjectIn);
	std::optional<VALUE> getObject(std::optional<KEY> name) override;
private:
	VALUE defaultObject;
};

template <typename KEY, typename VALUE>
RegistryDefaulted<KEY, VALUE>::RegistryDefaulted(VALUE defaultObjectIn)
	:RegistrySimple<KEY, VALUE>(),defaultObject(defaultObjectIn)
{
}

template <typename KEY, typename VALUE>
std::optional<VALUE> RegistryDefaulted<KEY, VALUE>::getObject(std::optional<KEY> name)
{
	auto v = RegistrySimple< KEY, VALUE>::getObject(name);
	return !v.has_value() ? defaultObject : v;
}
