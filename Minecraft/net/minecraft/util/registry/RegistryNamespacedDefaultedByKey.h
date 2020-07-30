#pragma once
#include "RegistryNamespaced.h"

template<typename KEY, typename VALUE>
class RegistryNamespacedDefaultedByKey :public RegistryNamespaced<KEY, VALUE>
{
public:
	explicit RegistryNamespacedDefaultedByKey(KEY defaultValueKeyIn);
	void registe(int32_t id, KEY key, VALUE value) override;
	void validateKey();
	std::optional<VALUE> getObjectById(int32_t id) override;
	std::optional<KEY> getNameForObject(VALUE value) override;
	std::optional<VALUE> getObject(std::optional<KEY> name) override;
	int32_t getIDForObject(VALUE value) override;
	std::optional<VALUE> getRandomObject(pcg32& random) override;
private:
	KEY defaultValueKey;
	std::optional<VALUE> defaultValue;
};

template <typename KEY, typename VALUE>
RegistryNamespacedDefaultedByKey<KEY, VALUE>::RegistryNamespacedDefaultedByKey(KEY defaultValueKeyIn)
	:defaultValueKey(defaultValueKeyIn)
{

}

template <typename KEY, typename VALUE>
void RegistryNamespacedDefaultedByKey<KEY, VALUE>::registe(int32_t id, KEY key, VALUE value)
{
	if (defaultValueKey== key) {
		defaultValue = value;
	}

	RegistryNamespaced<KEY, VALUE>::registe(id, key, value);
}

template <typename KEY, typename VALUE>
void RegistryNamespacedDefaultedByKey<KEY, VALUE>::validateKey()
{
	if(!defaultValue.has_value())
	{
		LOGGER->debug("Missing default of DefaultedMappedRegistry: " + defaultValueKey);
	}
}

template <typename KEY, typename VALUE>
std::optional<VALUE> RegistryNamespacedDefaultedByKey<KEY, VALUE>::getObjectById(int32_t id)
{
	auto v = RegistryNamespaced<KEY, VALUE>::getObjectById(id);
	return !v ? defaultValue : v;
}

template <typename KEY, typename VALUE>
int32_t RegistryNamespacedDefaultedByKey<KEY, VALUE>::getIDForObject(VALUE value)
{
	auto i = RegistryNamespaced<KEY, VALUE>::getIDForObject(value);
	return i == -1 ? RegistryNamespaced<KEY, VALUE>::getIDForObject(defaultValue) : i;
}

template <typename KEY, typename VALUE>
std::optional<VALUE> RegistryNamespacedDefaultedByKey<KEY, VALUE>::getRandomObject(pcg32& random)
{
	auto v = RegistryNamespaced<KEY, VALUE>::getRandomObject(random);
	return !v ? defaultValue : v;
}

template <typename KEY, typename VALUE>
std::optional<KEY> RegistryNamespacedDefaultedByKey<KEY, VALUE>::getNameForObject(VALUE value)
{
	auto k = RegistryNamespaced<KEY, VALUE>::getNameForObject(value);
	return !k.has_value() ? defaultValueKey : k;
}

template <typename KEY, typename VALUE>
std::optional<VALUE> RegistryNamespacedDefaultedByKey<KEY, VALUE>::getObject(std::optional<KEY> name)
{
	auto v = RegistryNamespaced<KEY, VALUE>::getObject(name);
	return !v.has_value() ? defaultValue : v;
}
