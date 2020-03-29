class PathNodeType
{
public:

  static PathNodeType BLOCKED;
  static PathNodeType OPEN;
  static PathNodeType WALKABLE;
  static PathNodeType TRAPDOOR;
  static PathNodeType FENCE;
  static PathNodeType LAVA;
  static PathNodeType WATER;
  static PathNodeType RAIL;
  static PathNodeType DANGER_FIRE;
  static PathNodeType DAMAGE_FIRE;
  static PathNodeType DANGER_CACTUS;
  static PathNodeType DAMAGE_CACTUS;
  static PathNodeType DANGER_OTHER;
  static PathNodeType DAMAGE_OTHER;
  static PathNodeType DOOR_OPEN;
  static PathNodeType DOOR_WOOD_CLOSED;
  static PathNodeType DOOR_IRON_CLOSED;

  explicit PathNodeType(float priorityIn);
  float getPriority() const;
private:
  float priority;
};