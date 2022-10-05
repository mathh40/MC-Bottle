#pragma once
#include <unordered_map>

class IScoreCriteria
{
  public:
    class EnumRenderType
    {
      private:
        static std::unordered_map<std::string, IScoreCriteria::EnumRenderType> BY_NAME;
        std::string renderType;

      public:
        explicit EnumRenderType(std::string_view renderTypeIn);
        std::string getRenderType() const;
        static IScoreCriteria::EnumRenderType getByName(std::string name);

        static EnumRenderType INTEGER;
        static EnumRenderType HEARTS;
    };

    std::unordered_map<> INSTANCES;
    IScoreCriteria DUMMY;
    IScoreCriteria TRIGGER;
    IScoreCriteria DEATH_COUNT;
    IScoreCriteria PLAYER_KILL_COUNT;
    IScoreCriteria TOTAL_KILL_COUNT;
    IScoreCriteria HEALTH;
    IScoreCriteria FOOD;
    IScoreCriteria AIR;
    IScoreCriteria ARMOR;
    IScoreCriteria XP;
    IScoreCriteria LEVEL;
    std::vector<IScoreCriteria> TEAM_KILL;
    std::vector<IScoreCriteria> KILLED_BY_TEAM;
    virtual std::string getName()                          = 0;
    virtual bool isReadOnly()                              = 0;
    virtual IScoreCriteria::EnumRenderType getRenderType() = 0;
};
