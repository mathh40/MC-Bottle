#include "Util.h"
#include <chrono>
#include <string_view>
#include <locale>

Util::EnumOS Util::getOSType()
{
#ifdef _WIN32
	return EnumOS::WINDOWS;
#elif _WIN64
	return EnumOS::WINDOWS;
#elif __unix || __unix__
	return EnumOS.LINUX;
#elif __APPLE__ || __MACH__
	return EnumOS::OSX;
#elif __linux__
	return EnumOS.LINUX;
#elif __FreeBSD__
	return EnumOS.UNKNOWN;
#else
	return EnumOS.UNKNOWN;
#endif
}

constexpr bool Util::startwith(std::string_view prefix, std::string_view toCheck)
{
	return std::mismatch(prefix.begin(), prefix.end(), toCheck.begin()).first == prefix.end();
}

uint64_t Util::getStringHash(std::string_view str) 
{
	auto size = str.size();
	uint64_t hash = 0;
	auto index = 0;
	for(auto substract = 1; substract < size + 1;++substract)
	{
		hash += str[index] * std::pow(31.0, size - substract);
		++index;
	}
	return hash;
}

int64_t Util::currentTimeMillis()
{
	return getcurrent_time();
}

std::string_view Util::toLowerCase(std::string_view str, const std::locale& loc)
{
	std::transform(str.begin(), str.end(), str.begin(), [&](const char c) 
	{
		return std::tolower(c, loc); 
	});
	return str;
}

std::vector<std::string> Util::split(const std::string_view str, std::string_view delimiter,size_t limit)
{
	std::vector<std::string> tokens;
	size_t prev = 0;
	size_t pos = 0;
	size_t i = 0;
	do
	{
		pos = str.find(delimiter, prev);
		if (pos == std::string::npos)
		{
			pos = str.length();
		}
		auto token = str.substr(prev, pos - prev);
		if (!token.empty())
		{
			tokens.emplace_back(token);
		}
		prev = pos + 1;
		++i;
	}
	while (pos < str.length() && prev < str.length() && limit > 0 ? i = limit : true);
	return tokens;
}

bool Util::endsWith(std::string_view s, std::string_view suffix)
{
	return s.rfind(suffix) == (s.size() - suffix.size());
}

Object runTask(FutureTask task, Logger logger)
{
	try {
		task.run();
		return task.get();
	}
	catch (ExecutionException var3) {
		logger.fatal("Error executing task", var3);
	}
	catch (InterruptedException var4) {
		logger.fatal("Error executing task", var4);
	}

	return nullptr;
}

int64_t Util::getcurrent_time()
{
	auto now = std::chrono::system_clock::now();
	auto now_ms = std::chrono::time_point_cast<std::chrono::nanoseconds>(now);
	auto epoch = now_ms.time_since_epoch();
	return epoch.count();
}
