#pragma once
#include <unordered_set>

#include "Scoreboard.h"
#include "Team.h"

class ScorePlayerTeam :public Team
{
public:
  ScorePlayerTeam(Scoreboard* scoreboardIn, std::string_view name);
  std::string getName() override;
  std::string getDisplayName() const;
  void setDisplayName(std::string_view name);
  Collection getMembershipCollection();
  std::string getPrefix() const;
  void setPrefix(std::string_view prefix);
  std::string getSuffix() const;
  void setSuffix(std::string_view suffix);
  std::string formatString(const std::string& input) override;
  bool getAllowFriendlyFire() override;
  void setAllowFriendlyFire(bool friendlyFire);
  bool getSeeFriendlyInvisiblesEnabled() override;
  void setSeeFriendlyInvisiblesEnabled(bool friendlyInvisibles);
  Team::EnumVisible getNameTagVisibility() override;
  Team::EnumVisible getDeathMessageVisibility() override;
  void setNameTagVisibility(Team::EnumVisible visibility);
  void setDeathMessageVisibility(Team::EnumVisible visibility);
  Team::CollisionRule getCollisionRule() override;
  void setCollisionRule(Team::CollisionRule rule);
  int32_t getFriendlyFlags();
  void setFriendlyFlags(int32_t flags);
  void setColor(TextFormatting color);
  TextFormatting getColor() override;

private:
  Scoreboard* scoreboard;
  std::string name;
  std::unordered_set<> membershipSet;
  std::string displayName;
  std::string prefix = "";
  std::string suffix = "";
  bool allowFriendlyFire = true;
  bool canSeeFriendlyInvisibles = true;
  Team::EnumVisible nameTagVisibility;
  Team::EnumVisible deathMessageVisibility;
  TextFormatting color;
  Team::CollisionRule collisionRule;
};

std::string formatPlayerName(Team* teamIn, const std::string& string);
