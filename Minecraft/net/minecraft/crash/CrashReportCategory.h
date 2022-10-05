#pragma once

#include <string_view>
#include <vector>
class IBlockState;
class Block;
class ICrashReportDetail;
class BlockPos;
class CrashReport;
class StackTraceElement;
class CrashReportCategory
{

  public:
    CrashReportCategory(const CrashReport &report, std::string_view name);
    static std::string getCoordinateInfo(double x, double y, double z);
    static std::string getCoordinateInfo(const BlockPos &pos);
    static std::string getCoordinateInfo(int32_t x, int32_t y, int32_t z);
    template <class T> void addDetail(std::string_view nameIn, T detail);
    void addCrashSection(std::string_view sectionName, std::string_view value);
    void addCrashSectionThrowable(std::string_view sectionName, std::exception &throwable);
    int32_t getPrunedStackTrace(int32_t size);
    bool firstTwoElementsOfStackTraceMatch(const StackTraceElement &s1, const StackTraceElement &s2);
    void trimStackTraceEntriesFromBottom(int32_t amount);
    void appendToStringBuilder(std::string &builder);
    std::vector<StackTraceElement> getStackTrace() const;
    static void addBlockInfo(CrashReportCategory &category, const BlockPos &pos, const Block *blockIn,
                             const int32_t blockData);

    static void addBlockInfo(CrashReportCategory &category, const BlockPos &pos, const IBlockState *state);

    static class Entry
    {

      public:
        Entry(std::string_view key, std::string_view value);
        std::string_view getKey() const;
        std::string_view getValue() const;

      private:
        std::string_view key;
        std::string_view value;
    };

  private:
    CrashReport crashReport;
    std::string_view name;
    std::vector<CrashReportCategory::Entry> children;
    std::vector<StackTraceElement> stackTrace;
};

template <class T> void CrashReportCategory::addDetail(std::string_view nameIn, T detail)
{
    try
    {
        addCrashSection(nameIn, detail());
    }
    catch (std::exception &var4)
    {
        addCrashSectionThrowable(nameIn, var4);
    }
}
