#pragma once
#include "../../../../authlib/com/mojang/authlib/GameProfile.h"

#include <string>
#include <optional>
#include <vector>

namespace StringUtils
{
	std::string ticksToElapsedTime(int ticks);
	std::string stripControlCodes(const std::string &text);
	bool isNullOrEmpty(std::optional<std::string_view> string);
	std::string getCommonPrefix(std::vector<std::string> strs);
	int indexOfDifference(std::vector<std::string> cs1);
	std::string replace_all(std::string str, std::string_view from, std::string& to);
	bool isBlank(std::string_view str);
    bool isNotBlank(std::string_view str);
    std::string replaceFirstOccurrence(
        std::string& s,
        std::string_view toReplace,
        std::string_view replaceWith);
	// trim from start (in place)
    void ltrim(std::string &s);

    // trim from end (in place)
    void rtrim(std::string &s);

    // trim from both ends (in place)
    void trim(std::string &s);

    // trim from start (copying)
    std::string ltrim_copy(std::string s);

    // trim from end (copying)
    std::string rtrim_copy(std::string s);

    // trim from both ends (copying)
    std::string trim_copy(std::string s);
}
