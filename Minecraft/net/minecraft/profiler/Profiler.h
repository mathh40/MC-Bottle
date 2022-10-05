#pragma once
#include "spdlog/logger.h"

#include <map>
#include <memory>
#include <string>

class Profiler
{
  public:
    class Result
    {
      public:
        Result(std::string_view profilerName, double usePercentage, double totalUsePercentage);
        friend bool operator<(const Result &a, const Result &b);
        int32_t getColor() const;

        double usePercentage;
        double totalUsePercentage;
        std::string profilerName;
    };

    void clearProfiling();
    void startSection(std::string_view name);
    void func_194340_a(Supplier p_194340_1_);
    void endSection();
    std::vector<Profiler::Result> getProfilingData(std::string_view profilerName);
    void endStartSection(std::string_view name);
    void func_194339_b(Supplier p_194339_1_);
    std::string getNameOfLastSection();

    bool profilingEnabled;

  private:
    static std::shared_ptr<spdlog::logger> LOGGER;
    std::vector<std::string> sectionList;
    std::vector<int64_t> timestampList;
    std::string profilingSection;
    std::map<std::string, int64_t> profilingMap;
};
