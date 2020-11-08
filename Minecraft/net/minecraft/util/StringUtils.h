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
}
