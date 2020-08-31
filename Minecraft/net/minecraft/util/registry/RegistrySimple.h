#pragma once
#include "../../../../../spdlog/include/spdlog/logger.h"
#include "../../../../../spdlog/include/spdlog/spdlog.h"
#include <optional>
#include <set>
#include <unordered_map>

template<typename KEY,typename VALUE>
class RegistrySimple
{
public:
	RegistrySimple();

	virtual ~RegistrySimple() = default;
	virtual std::optional<VALUE> getObject(std::optional<KEY> name);
	void putObject(KEY key, VALUE value);
	std::set<KEY> getKeys();
	virtual std::optional<VALUE> getRandomObject(pcg32& random);
	virtual bool containsKey(KEY key);
	typename std::unordered_map<KEY, VALUE>::iterator begin();
	typename std::unordered_map<KEY, VALUE>::iterator end();
protected:
	std::unordered_map<KEY, VALUE> registryObjects;
	std::shared_ptr<spdlog::logger> LOGGER;
	std::vector<VALUE> values;
};

template <typename KEY, typename VALUE>
RegistrySimple<KEY, VALUE>::RegistrySimple()
{
	LOGGER = spdlog::get("Minecraft")->clone("Registry");
}

template <typename KEY, typename VALUE>
std::optional<VALUE> RegistrySimple<KEY, VALUE>::getObject(std::optional<KEY> name)
{
	if (name.has_value())
		return registryObjects[name.value()];
	else
		return std::nullopt;
}

template <typename KEY, typename VALUE>
void RegistrySimple<KEY, VALUE>::putObject(KEY key, VALUE value)
{
	if (registryObjects.containsKey(key)) {
		LOGGER->debug("Adding duplicate key '{}' to registry", key);
	}
	registryObjects.emplace(key, value);
}

template <typename KEY, typename VALUE>
std::set<KEY> RegistrySimple<KEY, VALUE>::getKeys()
{
	return std::set<KEY>(registryObjects.begin(), registryObjects.end());
}

template <typename KEY, typename VALUE>
std::optional<VALUE> RegistrySimple<KEY, VALUE>::getRandomObject(pcg32& random)
{
	if (values.empty()) {
		if (registryObjects.empty()) {
			return std::nullopt;
		}
		values.resize(registryObjects.size());
		for (auto& v : registryObjects)
		{
			values.emplace_back(v);
		}
	}

	return values[random(values.size())];
}

template <typename KEY, typename VALUE>
bool RegistrySimple<KEY, VALUE>::containsKey(KEY key)
{
	return registryObjects.find(key) != registryObjects.end();
}

template <typename KEY, typename VALUE>
typename std::unordered_map<KEY, VALUE>::iterator RegistrySimple<KEY, VALUE>::begin()
{
	return registryObjects.begin();
}

template <typename KEY, typename VALUE>
typename std::unordered_map<KEY, VALUE>::iterator RegistrySimple<KEY, VALUE>::end()
{
	return registryObjects.end();
}
