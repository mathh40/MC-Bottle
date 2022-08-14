#pragma once
#include <cstdint>
#include <optional>
#include <random>

namespace spdlog {
	class logger;
}

class FutureTask;

namespace Util
{
	enum class EnumOS 
	{
		LINUX,
		SOLARIS,
		WINDOWS,
		OSX,
		UNKNOWN
	};

	EnumOS getOSType();

	template<class T>
	std::optional<T> runTask(FutureTask task, spdlog::logger& logger);

	int64_t getcurrent_time();

	template<typename Base, typename T>
	bool instanceof(const T* a) 
	{
		return std::is_base_of<Base, T>::value;
	}

	template<typename Collection1, typename Collection2>
	void removeAll(Collection1& c1, const Collection2& c2)
	{
		for (auto&& i : c2)
			c1.erase(std::remove_if(c1.begin(), c1.end(), [i](decltype(i) i1) {return i == i1; }), c1.end());
	}

	std::uint32_t float2ul(float x)
	{
		std::uint32_t ieee;
		memcpy(&ieee, &x, sizeof(float));
		return ieee;
	}

	constexpr bool startwith(std::string_view prefix,std::string_view toCheck);

	uint64_t getStringHash(std::string_view);

	template <typename U = uint64_t>
	class RandomizerWithSentinelShift 
	{
	public:
		template <typename Rng> bool operator()(Rng& rng) 
		{
			if (UNLIKELY(1 == m_rand)) 
			{
				m_rand = std::uniform_int_distribution<U>{}(rng) | s_mask_left1;
			}
			bool const ret = m_rand & 1;
			m_rand >>= 1;
			return ret;
		}

	private:
		static constexpr const U s_mask_left1 = U(1) << (sizeof(U) * 8 - 1);
		U m_rand = 1;
	};

	int64_t currentTimeMillis();

	std::string_view toLowerCase(std::string_view str, const std::locale& loc);

	std::vector<std::string> split(std::string_view s, std::string_view delimiter,size_t limit = 0);
	bool endsWith(std::string_view s, std::string_view suffix);

	template<typename T, typename G>
    void KeySet(T& KeySet, G maps)
	{
	    for(auto& entry : maps)
	    {
	        KeySet.emplace(entry.first);
	    }
	}

    template<typename T, typename G>
    void Collection(T& Collection, G maps)
	{
	    for(auto& entry : maps)
	    {
	        KeySet.emplace(entry.first);
	    }
	}

	template<typename T,typename G>
	void erase(T& v,G predig)
	{
	    v.erase(std::remove(v.begin(), v.end(), predig), v.end());
	}

    template<typename T,typename G>
	void erase_if(T& v,G predig)
	{
	    v.erase(std::remove_if(v.begin(), v.end(), predig), v.end());
	}
}
