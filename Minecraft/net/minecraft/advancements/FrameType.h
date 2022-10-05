#pragma once
#include <string>
#include <string_view>
#include <vector>

class FrameType
{
  public:
    static FrameType TASK;
    static FrameType CHALLENGE;
    static FrameType GOAL;

    std::string_view getName() const;
    uint64_t getIcon() const;
    static FrameType &byName(std::string_view nameIn);
    TextFormatting &getFormat() const;

  private:
    FrameType(std::string_view nameIn, uint64_t iconIn, const TextFormatting &formatIn);

    std::vector<FrameType *> val;
    std::string name;
    uint64_t icon;
    TextFormatting format;
};
