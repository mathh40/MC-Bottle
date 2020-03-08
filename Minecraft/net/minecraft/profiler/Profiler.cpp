#include "Profiler.h"
#include "spdlog/spdlog-inl.h"
#include <chrono>

std::shared_ptr<spdlog::logger> Profiler::LOGGER = spdlog::get("Minecraft")->clone("Profiler");
constexpr auto maxtime = 100000000L;


Profiler::Result::Result(std::string_view profilerName, double usePercentage, double totalUsePercentage)
  :profilerName(profilerName),usePercentage(usePercentage),totalUsePercentage(totalUsePercentage)
{
}

int32_t Profiler::Result::getColor() const
{
  return (std::hash<std::string>()(profilerName) & 11184810) + 4473924;
}

void Profiler::clearProfiling()
{
  profilingMap.clear();
  profilingSection = "";
  sectionList.clear();
}

void Profiler::startSection(std::string_view name)
{
  if (profilingEnabled) {
    if (!profilingSection.empty()) {
      profilingSection = profilingSection + ".";
    }

    profilingSection = profilingSection.append(name);
    sectionList.emplace_back(profilingSection);
    auto start = std::chrono::high_resolution_clock::now();
    timestampList.emplace_back(std::chrono::duration_cast<std::chrono::nanoseconds>(start.time_since_epoch()).count());
  }
}

void Profiler::func_194340_a(Supplier p_194340_1_)
{
  if (profilingEnabled) {
    startSection(p_194340_1_.get());
  }
}

void Profiler::endSection()
{
  if (profilingEnabled) {
    auto start = std::chrono::high_resolution_clock::now();
    auto i = std::chrono::duration_cast<std::chrono::nanoseconds>(start.time_since_epoch()).count();
    auto j = timestampList[timestampList.size()];
    timestampList.pop_back();
    auto k = i - j;
    auto ite = profilingMap.find(profilingSection);
    if (ite != profilingMap.end()) {
      profilingMap.emplace(profilingSection, ite->second + k);
    } else {
      profilingMap.emplace(profilingSection, k);
    }

    if (k > maxtime) {
      LOGGER->warn("Something's taking too long! '{}' took aprox {} ms", profilingSection, (double)k / maxtime);
    }

    profilingSection = sectionList.empty() ? "" : sectionList[sectionList.size() - 1];
  }
}

std::vector<Profiler::Result> Profiler::getProfilingData(std::string_view profilerName)
{
  if (!profilingEnabled) {
    return {};
  } else {
    auto i = profilingMap.containsKey("root") ? profilingMap.get("root") : 0L;
    auto j = profilingMap.containsKey(profilerName) ? profilingMap.get(profilerName) : -1L;
    std::vector<Profiler::Result> list;
    if (!profilerName.empty()) {
      profilerName = profilerName + ".";
    }

    int64_t k = 0L;

    for(auto keySet : profilingMap) {
      auto s = keySet.first;
      if (s.length() > profilerName.length() && s.startsWith(profilerName) && s.indexOf(".", profilerName.length() + 1) < 0) {
        k += keySet.second;
      }
    }

    float f = (float)k;
    if (k < j) {
      k = j;
    }

    if (i < k) {
      i = k;
    }

    std::string s3;
    for(auto keySet : profilingMap) {
      s3 = keySet.first;
      if (s3.size() > profilerName.length() && s3.startsWith(profilerName) && s3.indexOf(".", profilerName.length() + 1) < 0) {
        auto l = keySet.second;
        double d0 = (double)l * 100.0 / (double)k;
        double d1 = (double)l * 100.0 / (double)i;
        auto s2 = s3.substr(profilerName.size());
        list.emplace_back(Profiler::Result(s2, d0, d1));
      }
    }

    for(auto keySet : profilingMap){
      s3 = keySet.first;
      profilingMap.emplace(s3, keySet.second * 999L / 1000L);
    }

    if ((float)k > f) {
      list.emplace_back(Profiler::Result("unspecified", (double)((float)k - f) * 100.0 / (double)k, (double)((float)k - f) * 100.0 / (double)i));
    }

    std::sort(list.begin(),list.end());
    list.emplace_back(0, Profiler::Result(profilerName, 100.0, (double)k * 100.0 / (double)i));
    return list;
  }
}

void Profiler::endStartSection(std::string_view name)
{
  endSection();
  startSection(name);
}

void Profiler::func_194339_b(Supplier p_194339_1_)
{
  endSection();
  func_194340_a(p_194339_1_);
}

std::string Profiler::getNameOfLastSection()
{
  return sectionList.empty() ? "[UNKNOWN]" : sectionList[sectionList.size() - 1];
}

bool operator<(const Profiler::Result &a, const Profiler::Result &b)
{
  return a.usePercentage < b.usePercentage;
}
