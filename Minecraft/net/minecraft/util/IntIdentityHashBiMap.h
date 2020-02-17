#pragma once
#include <vector>
#include <optional>

template<typename KEY, typename VALUE>
class IntIdentityHashBiMap
{
public:
	explicit IntIdentityHashBiMap(int64_t initialCapacity);
	int64_t getId(std::optional<VALUE> p_186815_1_);
	std::optional<VALUE> get(int64_t idIn);
	int64_t add(VALUE objectIn);
	void put(VALUE objectIn, int64_t intKey);
	void clear();
	int64_t size() const;
private:
	std::vector<std::optional<VALUE>> values;
	std::vector<int64_t> intKeys;
	std::vector<VALUE> byId;
	int64_t nextFreeIndex;
	int64_t mapSize;

	int64_t getValue(int64_t p_186805_1_);
	int64_t nextId();
	void grow(int64_t capacity);
	int64_t hashObject(std::optional<VALUE> obectIn);
	int64_t getIndex(std::optional<VALUE> objectIn, int64_t p_186816_2_);
	int64_t findEmpty(int64_t p_186806_1_);
};

template <typename KEY, typename VALUE>
IntIdentityHashBiMap<KEY, VALUE>::IntIdentityHashBiMap(int64_t initialCapacity)
{
	initialCapacity = (int)((float)initialCapacity / 0.8F);
	values.resize(initialCapacity);
	intKeys.resize(initialCapacity);
	byId.resize(initialCapacity);
}

template <typename KEY, typename VALUE>
int64_t IntIdentityHashBiMap<KEY, VALUE>::getId(std::optional<VALUE> p_186815_1_)
{
	return getValue(getIndex(p_186815_1_, hashObject(p_186815_1_)));
}

template <typename KEY, typename VALUE>
std::optional<VALUE> IntIdentityHashBiMap<KEY, VALUE>::get(int64_t idIn)
{
	return idIn >= 0 && idIn < byId.size() ? byId[idIn] : std::nullopt;
}

template <typename KEY, typename VALUE>
int64_t IntIdentityHashBiMap<KEY, VALUE>::add(VALUE objectIn)
{
	int64_t i = nextId();
	put(objectIn, i);
	return i;
}

template <typename KEY, typename VALUE>
void IntIdentityHashBiMap<KEY, VALUE>::put(VALUE objectIn, int64_t intKey)
{
	auto i = MathHelper::max(intKey, mapSize + 1);
	int j;
	if ((float)i >= (float)values.size() * 0.8F) 
	{
		for (j = values.size() << 1; j < intKey; j <<= 1) 
		{
		}

		grow(j);
	}

	j = findEmpty(hashObject(objectIn));
	values[j] = objectIn;
	intKeys[j] = intKey;
	byId[intKey] = objectIn;
	++mapSize;
	if (intKey == nextFreeIndex) 
	{
		++nextFreeIndex;
	}
}

template <typename KEY, typename VALUE>
void IntIdentityHashBiMap<KEY, VALUE>::clear()
{
	std::fill(values.begin(), values.end(), std::nullopt);
	std::fill(byId.begin(), byId.end(), std::nullopt);
	nextFreeIndex = 0;
	mapSize = 0;
}

template <typename KEY, typename VALUE>
int64_t IntIdentityHashBiMap<KEY, VALUE>::size() const
{
	return mapSize;
}

template <typename KEY, typename VALUE>
int64_t IntIdentityHashBiMap<KEY, VALUE>::getValue(int64_t p_186805_1_)
{
	return p_186805_1_ == -1 ? -1 : intKeys[p_186805_1_];
}

template <typename KEY, typename VALUE>
int64_t IntIdentityHashBiMap<KEY, VALUE>::nextId()
{
	while (nextFreeIndex < byId.size()) 
	{
		++nextFreeIndex;
	}

	return nextFreeIndex;
}

template <typename KEY, typename VALUE>
void IntIdentityHashBiMap<KEY, VALUE>::grow(int64_t capacity)
{
	auto ak = values;
	auto aint = intKeys;
	values.resize(capacity);
	intKeys.resize(capacity);
	byId.resize(capacity);
	nextFreeIndex = 0;
	mapSize = 0;

	for (auto i = 0; i < ak.size(); ++i) 
	{
		if (ak[i] != std::nullopt)
		{
			put(ak[i], aint[i]);
		}
	}
}

template <typename KEY, typename VALUE>
int64_t IntIdentityHashBiMap<KEY, VALUE>::hashObject(std::optional<VALUE> obectIn)
{
	std::hash<VALUE> hash_fn;
	return (hash_fn(obectIn) & std::numeric_limits<KEY>::max()) % values.size();
}

template <typename KEY, typename VALUE>
int64_t IntIdentityHashBiMap<KEY, VALUE>::getIndex(std::optional<VALUE> objectIn, int64_t p_186816_2_)
{
	for (auto j = p_186816_2_; j < values.size(); ++j) 
	{
		if (values[j] == objectIn) 
		{
			return j;
		}

		if (values[j] == std::nullopt) 
		{
			return -1;
		}
	}

	for (auto j = 0; j < p_186816_2_; ++j) 
	{
		if (values[j] == objectIn) 
		{
			return j;
		}

		if (values[j] == std::nullopt)
		{
			return -1;
		}
	}

	return -1;
}

template <typename KEY, typename VALUE>
int64_t IntIdentityHashBiMap<KEY, VALUE>::findEmpty(int64_t p_186806_1_)
{
	for (auto j = p_186806_1_; j < this.values.length; ++j) 
	{
		if (values[j] == std::nullopt)
		{
			return j;
		}
	}

	for (auto j = 0; j < p_186806_1_; ++j)
	{
		if (values[j] == std::nullopt)
		{
			return j;
		}
	}

	throw std::runtime_error("Overflowed :(");
}
