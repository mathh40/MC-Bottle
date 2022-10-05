#pragma once
#include <string>
#include <vector>

class EnumFacing;

class Mirror
{
  public:
    static Mirror NONE;
    static Mirror LEFT_RIGHT;
    static Mirror FRONT_BACK;
    Mirror(std::string nameIn);
    int mirrorRotation(int rotationIn, int rotationCount) const;
    Rotation toRotation(EnumFacing facing);
    EnumFacing mirror(EnumFacing facing) const;
    friend auto operator==(Mirror const &lhs, const Mirror &rhs) -> bool;

  private:
    std::string name;
    static std::vector<std::string> mirrorNames;
};
