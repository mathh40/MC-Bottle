#include "StringUtils.h"

#include "../../../../compile-time-regular-expressions/single-header/ctre.hpp"
#include "ByteBuffer.h"

#include <algorithm>

static constexpr auto PATTERN_CONTROL_COD = ctll::fixed_string{R"(§[0-9a-fk-or])"};

std::string StringUtils::ticksToElapsedTime(int ticks)
{
    int i = ticks / 20;
    int j = i / 60;
    i %= 60;
    return i < 10 ? j + ":0" + i : j + ":" + i;
}

std::string StringUtils::stripControlCodes(const std::string &text)
{

    auto temp           = ctre::search<PATTERN_CONTROL_COD>(text);
    std::string result  = "";
    std::string replace = temp.str();
    StringUtils::replaceFirstOccurrence(result, replace, "");

    return result;
}

bool StringUtils::isNullOrEmpty(std::optional<std::string_view> string)
{
    return !string.has_value() || string.has_value() ? string.value().empty() : false;
}

std::string StringUtils::getCommonPrefix(std::vector<std::string> strs)
{
    if (strs.empty())
    {
        return "";
    }
    int smallestIndexOfDiff = indexOfDifference(strs);
    if (smallestIndexOfDiff == -1)
    {
        if (strs[0].empty())
        {
            return "";
        }
        return std::string(strs[0]);
    }
    else if (smallestIndexOfDiff == 0)
    {
        return "";
    }
    else
    {
        return std::string(strs[0].substr(0, smallestIndexOfDiff));
    }
}

int StringUtils::indexOfDifference(std::vector<std::string> css)
{
    if (css.size() <= 1)
    {
        return -1;
    }
    bool anyStringNull    = false;
    bool allStringsNull   = true;
    auto arrayLen         = css.size();
    size_t shortestStrLen = std::numeric_limits<size_t>::max();
    size_t longestStrLen  = 0;

    // find the min and max string lengths; this avoids checking to make
    // sure we are not exceeding the length of the string each time through
    // the bottom loop.
    for (auto cs : css)
    {
        if (cs.empty())
        {
            anyStringNull  = true;
            shortestStrLen = 0;
        }
        else
        {
            allStringsNull = false;
            shortestStrLen = std::min(cs.size(), shortestStrLen);
            longestStrLen  = std::max(cs.size(), longestStrLen);
        }
    }

    // handle lists containing all nulls or all empty strings
    if (allStringsNull || longestStrLen == 0 && !anyStringNull)
    {
        return -1;
    }

    // handle lists containing some nulls or some empty strings
    if (shortestStrLen == 0)
    {
        return 0;
    }

    // find the position with the first difference across all strings
    int firstDiff = -1;
    for (int stringPos = 0; stringPos < shortestStrLen; stringPos++)
    {
        const char comparisonChar = css[0][stringPos];
        for (int arrayPos = 1; arrayPos < arrayLen; arrayPos++)
        {
            if (css[arrayPos][stringPos] != comparisonChar)
            {
                firstDiff = stringPos;
                break;
            }
        }
        if (firstDiff != -1)
        {
            break;
        }
    }
    if (firstDiff == -1 && shortestStrLen != longestStrLen)
    {
        // we compared all of the characters up to the length of the
        // shortest string and didn't find a match, but the string lengths
        // vary, so return the length of the shortest string.
        return shortestStrLen;
    }
    return firstDiff;
}

std::string StringUtils::replace_all(std::string str, std::string_view from, std::string &to)
{
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos)
    {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
    }
    return str;
}

bool StringUtils::isNotBlank(std::string_view str)
{
    return !isBlank(str);
}
std::string StringUtils::replaceFirstOccurrence(std::string &s, std::string_view toReplace,
                                                std::string_view replaceWith)
{
    std::size_t pos = s.find(toReplace);
    if (pos == std::string::npos)
        return s;
    return s.replace(pos, toReplace.length(), replaceWith);
}

void StringUtils::ltrim(std::string &s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) { return !std::isspace(ch); }));
}

void StringUtils::rtrim(std::string &s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) { return !std::isspace(ch); }).base(), s.end());
}

void StringUtils::trim(std::string &s)
{
    ltrim(s);
    rtrim(s);
}

std::string StringUtils::ltrim_copy(std::string s)
{
    ltrim(s);
    return s;
}

std::string StringUtils::rtrim_copy(std::string s)
{
    rtrim(s);
    return s;
}

std::string StringUtils::trim_copy(std::string s)
{
    trim(s);
    return s;
}

bool isBlank(std::string_view str)
{
    bool blank = false;
    if (str.empty() || std::all_of(str.begin(), str.end(), [](char c) { return std::isspace(c); }))
    {
        blank = true;
    }
}
