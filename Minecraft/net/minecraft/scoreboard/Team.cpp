#include "Team.h"

Team::EnumVisible::EnumVisible ALWAYS = Team::EnumVisible::EnumVisible("always", 0);
Team::EnumVisible::EnumVisible NEVER = Team::EnumVisible::EnumVisible("never", 1);
Team::EnumVisible::EnumVisible HIDE_FOR_OTHER_TEAMS = Team::EnumVisible::EnumVisible("hideForOtherTeams", 2);
Team::EnumVisible::EnumVisible HIDE_FOR_OWN_TEAM = Team::EnumVisible::EnumVisible("hideForOwnTeam", 3);


bool operator==(const Team& other)
{
}

std::vector<std::string> Team::EnumVisible::getNames()
{
   std::vector<std::string> temp;
   for(auto pair : nameMap)
   {
      temp.emplace_back(pair.first);
   }

   return temp;
}

Team::EnumVisible Team::EnumVisible::getByName(std::string_view nameIn)
{
   return nameMap.at(nameIn);
}

Team::EnumVisible::EnumVisible(std::string_view nameIn, int32_t idIn)
   :internalName(nameIn),id(idIn)
{
   nameMap.emplace(nameIn,this);
}
