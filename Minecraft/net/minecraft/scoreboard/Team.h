#pragma once
#include <string>

#include "text/TextFormatting.h"

class Team
{
public:

   class EnumVisible
   {
   public:
      static EnumVisible ALWAYS;
      static EnumVisible NEVER;
      static EnumVisible HIDE_FOR_OTHER_TEAMS;
      static EnumVisible HIDE_FOR_OWN_TEAM;

      static std::vector<std::string> getNames();
      static Team::EnumVisible getByName(std::string_view nameIn);


      std::string internalName;
      int32_t id;
   private:

      EnumVisible(std::string_view nameIn, int32_t idIn);

      static std::unordered_map<std::string,Team::EnumVisible> nameMap;
   };


   friend bool operator==(const Team& other);

   virtual std::string getName() = 0;

   virtual std::string formatString(std::string_view var1) = 0;

   virtual bool getSeeFriendlyInvisiblesEnabled() = 0;

   virtual bool getAllowFriendlyFire() = 0;

   virtual Team::EnumVisible getNameTagVisibility() = 0;

   virtual TextFormatting getColor() = 0;

   virtual Collection getMembershipCollection() = 0;

   virtual Team::EnumVisible getDeathMessageVisibility() = 0;

   virtual Team::CollisionRule getCollisionRule() = 0;
private:

};
