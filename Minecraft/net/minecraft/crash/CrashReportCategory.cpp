#include "CrashReportCategory.h"

#include "Block.h"
#include "fmt/format.h"
#include "math/BlockPos.h"

#include <thread>

CrashReportCategory::CrashReportCategory(const CrashReport &report, std::string_view name)
    : crashReport(report), name(name)
{
}

std::string CrashReportCategory::getCoordinateInfo(double x, double y, double z)
{
    return fmt::format("{:2f},{:2f},{:2f} - {:s}", x, y, z, getCoordinateInfo(BlockPos(x, y, z)));
}

std::string CrashReportCategory::getCoordinateInfo(const BlockPos &pos)
{
    return getCoordinateInfo(pos.getx(), pos.gety(), pos.getz());
}

std::string CrashReportCategory::getCoordinateInfo(int32_t x, int32_t y, int32_t z)
{

    std::string stringbuilder = "";

    try
    {
        stringbuilder += fmt::format("World: (%d,%d,%d)", x, y, z);
    }
    catch (std::exception &e)
    {
        stringbuilder += "(Error finding world loc)";
    }

    stringbuilder += ", ";

    int k2;
    int l2;
    int i3;
    int j3;
    int k3;
    int l3;
    int i4;
    int j4;
    int k4;
    try
    {
        k2 = x >> 4;
        l2 = z >> 4;
        i3 = x & 15;
        j3 = y >> 4;
        k3 = z & 15;
        l3 = k2 << 4;
        i4 = l2 << 4;
        j4 = (k2 + 1 << 4) - 1;
        k4 = (l2 + 1 << 4) - 1;
        stringbuilder +=
            fmt::format("Chunk: (at {:d},{:d},{:d} in {:d},{:d}; contains blocks {:d},0,{:d} to {:d},255,{:d})", i3, j3,
                        k3, k2, l2, l3, i4, j4, k4);
    }
    catch (std::exception &e)
    {
        stringbuilder += "(Error finding chunk loc)";
    }

    stringbuilder += (", ");

    try
    {
        k2     = x >> 9;
        l2     = z >> 9;
        i3     = k2 << 5;
        j3     = l2 << 5;
        k3     = (k2 + 1 << 5) - 1;
        l3     = (l2 + 1 << 5) - 1;
        i4     = k2 << 9;
        j4     = l2 << 9;
        k4     = (k2 + 1 << 9) - 1;
        int j2 = (l2 + 1 << 9) - 1;
        stringbuilder += (fmt::format("Region: (%d,%d; contains chunks %d,%d to %d,%d, blocks %d,0,%d to %d,255,%d)",
                                      k2, l2, i3, j3, k3, l3, i4, j4, k4, j2));
    }
    catch (std::exception &e)
    {
        stringbuilder += "(Error finding world loc)";
    }

    return stringbuilder;
}

void CrashReportCategory::addCrashSection(std::string_view sectionName, std::string_view value)
{
    children.emplace_back(CrashReportCategory::Entry(sectionName, value));
}

void CrashReportCategory::addCrashSectionThrowable(std::string_view sectionName, std::exception &throwable)
{
    addCrashSection(sectionName, throwable.what());
}

int32_t CrashReportCategory::getPrunedStackTrace(int32_t size)
{
    StackTraceElement[] astacktraceelement = Thread.currentThread().getStackTrace();
    if (astacktraceelement.length <= 0)
    {
        return 0;
    }
    else
    {
        stackTrace = new StackTraceElement[astacktraceelement.length - 3 - size];
        System.arraycopy(astacktraceelement, 3 + size, stackTrace, 0, stackTrace.length);
        return stackTrace.length;
    }
}

bool CrashReportCategory::firstTwoElementsOfStackTraceMatch(const StackTraceElement &s1, const StackTraceElement &s2)
{
    if (stackTrace.size() != 0 && s1 != nullptr)
    {
        auto stacktraceelement = stackTrace[0];
        if (stacktraceelement.isNativeMethod() == s1.isNativeMethod() &&
            stacktraceelement.getClassName().equals(s1.getClassName()) &&
            stacktraceelement.getFileName().equals(s1.getFileName()) &&
            stacktraceelement.getMethodName().equals(s1.getMethodName()))
        {
            if (s2 != nullptr != this.stackTrace.length > 1)
            {
                return false;
            }
            else if (s2 != nullptr && !stackTrace[1].equals(s2))
            {
                return false;
            }
            else
            {
                stackTrace[0] = s1;
                return true;
            }
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

void CrashReportCategory::trimStackTraceEntriesFromBottom(int32_t amount)
{
    StackTraceElement[] astacktraceelement = new StackTraceElement[stackTrace.size() - amount];
    System.arraycopy(stackTrace, 0, astacktraceelement, 0, astacktraceelement.length);
    stackTrace = astacktraceelement;
}

void CrashReportCategory::appendToStringBuilder(std::string &builder)
{
    builder.append("-- ").append(name).append(" --\n");
    builder.append("Details:");
    for (auto &entry : children)
    {
        builder.append("\n\t");
        builder.append(entry.getKey());
        builder.append(": ");
        builder.append(entry.getValue());
    }

    if (stackTrace != nullptr && stackTrace.size() > 0)
    {
        builder.append("\nStacktrace:");

        for (auto stacktraceelement : stackTrace)
        {
            builder.append("\n\tat ");
            builder.append(stacktraceelement);
        }
    }
}

std::vector<StackTraceElement> CrashReportCategory::getStackTrace() const
{
    return stackTrace;
}

void CrashReportCategory::addBlockInfo(CrashReportCategory &category, const BlockPos &pos, const Block *blockIn,
                                       const int32_t blockData)
{
    auto i = Block::getIdFromBlock(blockIn);
    category.addDetail("Block type", [&]() {
        try
        {
            return fmt::format("ID #{:d} ({:s} // {:s})", i, blockIn->getTranslationKey(),
                               blockIn->getClass().getCanonicalName());
        }
        catch (std::exception &var2)
        {
            return "ID #" + i;
        }
    });

    category.addDetail("Block data value", [&]() {
        if (blockData < 0)
        {
            return fmt::format("Unknown? (Got {})", blockData);
        }
        else
        {
            return fmt::format("{:d} / {:#X} / {:#b}", blockData);
        }
    });

    category.addDetail("Block location", [&]() { return CrashReportCategory::getCoordinateInfo(pos); });
}

void CrashReportCategory::addBlockInfo(CrashReportCategory &category, const BlockPos &pos, const IBlockState *state)
{

    category.addDetail("Block", [&]() { return state->toString(); });

    category.addDetail("Block location", [&]() { return CrashReportCategory::getCoordinateInfo(pos); });
}

CrashReportCategory::Entry::Entry(std::string_view key, std::string_view value)
{
    key = key;
    if (value.empty())
    {
        value = "~~NULL~~";
    }
    else if (value instanceof Throwable)
    {
        Throwable throwable = (Throwable)value;
        value               = "~~ERROR~~ " + throwable.getClass().getSimpleName() + ": " + throwable.getMessage();
    }
    else
    {
        value = value;
    }
}

std::string_view CrashReportCategory::Entry::getKey() const
{
    return key;
}

std::string_view CrashReportCategory::Entry::getValue() const
{
    return value;
}
