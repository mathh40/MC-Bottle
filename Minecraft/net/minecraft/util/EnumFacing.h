#pragma once
#include "../util/random/Random.h"
#include "math/Vec3i.h"

#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

class EntityLivingBase;
class BlockPos;
class Axis;
class EnumFacing;

class AxisDirection
{
  public:
    static AxisDirection POSITIVE;
    static AxisDirection NEGATIVE;
    AxisDirection(int offset, std::string description);
    int getOffset() const;

  private:
    int offset;
    ;
    std::string description;
};

class Plane
{
  public:
    static Plane HORIZONTAL;
    static Plane VERTICAL;

    std::vector<EnumFacing> facings() const;
    explicit Plane(const std::string &name);

    EnumFacing random(Random &rand) const;
    friend bool operator==(const Plane &lhs, const Plane &rhs);

  private:
    std::string name;
};

class Axis
{
  public:
    static Axis X;
    static Axis Y;
    static Axis Z;

    Axis(std::string name, Plane plane);
    static std::optional<Axis> byName(std::string_view name);
    std::string getName2();
    bool isVertical();
    bool isHorizontal();
    std::string toString();
    Plane getPlane();
    std::string getName();

    friend auto operator==(const Axis &lhs, const Axis &rhs) -> bool;
    friend auto operator!=(const Axis &lhs, const Axis &rhs) -> bool;

  private:
    static std::unordered_map<std::string, Axis> NAME_LOOKUP;
    std::string name;
    Plane plane;
};

class EnumFacing
{
  public:
    static EnumFacing DOWN;
    static EnumFacing UP;
    static EnumFacing NORTH;
    static EnumFacing SOUTH;
    static EnumFacing WEST;
    static EnumFacing EAST;

    int32_t getIndex() const;
    int32_t getHorizontalIndex() const;
    AxisDirection getAxisDirection() const;
    EnumFacing getOpposite() const;
    EnumFacing rotateAround(Axis axis);
    EnumFacing rotateY() const;
    EnumFacing rotateYCCW() const;
    int32_t getXOffset() const;
    int32_t getYOffset() const;
    int32_t getZOffset() const;
    std::string getName2() const;
    Axis getAxis() const;
    static std::optional<EnumFacing> byName(std::string &name);
    static EnumFacing byIndex(int32_t index);
    static EnumFacing byHorizontalIndex(int32_t horizontalIndexIn);
    static EnumFacing fromAngle(double angle);
    float getHorizontalAngle() const;
    static EnumFacing random(Random &rand);
    static EnumFacing getFacingFromVector(float x, float y, float z);
    std::string toString() const;
    std::string getName() const;
    static EnumFacing getFacingFromAxis(AxisDirection axisDirectionIn, Axis axisIn);
    static EnumFacing getDirectionFromEntityLiving(BlockPos pos, EntityLivingBase *placer);
    Vec3i getDirectionVec() const;
    friend void initEnumFacing();
    friend auto operator==(const EnumFacing &lhs, const EnumFacing &rhs) -> bool;

    static std::array<EnumFacing, 6> values();

    friend auto operator!=(const EnumFacing &lhs, const EnumFacing &rhs) -> bool;

    EnumFacing(int32_t indexIn, int32_t oppositeIn, int32_t horizontalIndexIn, std::string_view nameIn,
               AxisDirection axisDirectionIn, Axis axisIn, Vec3i directionVecIn);
    EnumFacing &operator=(const EnumFacing &other);

  private:
    int32_t index;
    int32_t opposite;
    int32_t horizontalIndex;
    std::string name;
    Axis axis;
    AxisDirection axisDirection;
    Vec3i directionVec;
    static std::array<EnumFacing, 6> VALUES;
    static std::array<EnumFacing, 4> HORIZONTALS;
    static std::unordered_map<std::string, EnumFacing> NAME_LOOKUP;

    EnumFacing rotateX() const;
    EnumFacing rotateZ() const;
};
