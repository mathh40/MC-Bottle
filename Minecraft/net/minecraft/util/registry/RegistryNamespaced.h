#pragma once
#include "RegistrySimple.h"

template<typename KEY, typename VALUE>
class RegistryNamespaced :public RegistrySimple<KEY, VALUE>
{
public:
	virtual void registe(int32_t id, KEY key, VALUE value);
	virtual std::optional<KEY> getNameForObject(VALUE value);
	virtual int32_t getIDForObject(VALUE value);
	virtual std::optional<VALUE> getObjectById(int32_t id);
	typename std::unordered_map<int32_t, VALUE>::iterator begin();
	typename std::unordered_map<int32_t, VALUE>::iterator end();
private:
	std::unordered_map<int32_t, VALUE> underlyingIntegerMap;
};

template <typename KEY, typename VALUE>
void RegistryNamespaced<KEY, VALUE>::registe(int32_t id, KEY key, VALUE value)
{
	underlyingIntegerMap.emplace(value, id);
	putObject(key, value);
}

template <typename KEY, typename VALUE>
std::optional<KEY> RegistryNamespaced<KEY, VALUE>::getNameForObject(VALUE value)
{
	auto result = std::find_if(
        this->registryObjects.begin(),
        this->registryObjects.end(),
		[value](const auto & mo) {return mo.second == value; });

	if (result != this->registryObjects.end())
		return result->first;
	else
		return std::nullopt;
}

template <typename KEY, typename VALUE>
int32_t RegistryNamespaced<KEY, VALUE>::getIDForObject(VALUE value)
{
	auto result = std::find_if(
		underlyingIntegerMap.begin(),
		underlyingIntegerMap.end(),
		[value](const auto & mo) {return mo.second == value; });

	if (result != underlyingIntegerMap.end())
		return result->first;
	else
		return 0;
}

template <typename KEY, typename VALUE>
std::optional<VALUE> RegistryNamespaced<KEY, VALUE>::getObjectById(int32_t id)
{
	auto result = underlyingIntegerMap.find(id);

	if (result != underlyingIntegerMap.end())
		return result->second;
	else
		return std::nullopt;
}

template <typename KEY, typename VALUE>
typename std::unordered_map<int32_t, VALUE>::iterator RegistryNamespaced<KEY, VALUE>::begin()
{
	return underlyingIntegerMap.begin();
}

template <typename KEY, typename VALUE>
typename std::unordered_map<int32_t, VALUE>::iterator RegistryNamespaced<KEY, VALUE>::end()
{
	return underlyingIntegerMap.end();
}
