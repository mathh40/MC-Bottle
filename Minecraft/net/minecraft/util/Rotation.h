#pragma once
#include "EnumFacing.h"

class Rotation
{
  public:
    static const Rotation NONE;
    static const Rotation CLOCKWISE_90;
    static const Rotation CLOCKWISE_180;
    static const Rotation COUNTERCLOCKWISE_90;

    Rotation add(const Rotation &rotation) const;
    explicit operator int() const;
    EnumFacing rotate(EnumFacing facing) const;
    int32_t rotate(int32_t p_185833_1_, int32_t p_185833_2_) const;
    std::string getname() const;

  private:
    std::string name;
    static int32_t count;
    const int32_t id;
    static std::vector<std::string> rotationNames;
    explicit Rotation(std::string_view nameIn);
};
