#pragma once
#include <stdexcept>
#include <unordered_map>

namespace MapPopulator
{
template <typename C, typename T> std::unordered_map<T, C> createMap(T keys, C values)
{
    return populateMap(keys, values, std::unordered_map<T, C>{});
}

template <typename Map, typename C, typename T> Map populateMap(T keys, C values, Map map)
{
    auto iterator = values.begin();
    auto k        = keys.begin();

    while (k != keys.end())
    {
        map.put(*k, ++iterator);
    }

    if (iterator != values.end())
    {
        throw std::logic_error("Size Key not Size Values");
    }
    else
    {
        return map;
    }
}
} // namespace MapPopulator
