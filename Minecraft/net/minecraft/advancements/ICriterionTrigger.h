#pragma once

class ICriterionTrigger
{
public:
       ResourceLocation getId();

   void addListener(PlayerAdvancements var1, ICriterionTrigger::Listener var2);

   void removeListener(PlayerAdvancements var1, ICriterionTrigger::Listener var2);

   void removeAllListeners(PlayerAdvancements var1);

   ICriterionInstance* deserializeInstance(Json var1);

   class Listener {
   public:
   public
    Listener(ICriterionInstance* criterionInstanceIn, Advancement advancementIn, std::string_view criterionNameIn);
    ICriterionInstance* getCriterionInstance() const;
    void grantCriterion(PlayerAdvancements playerAdvancementsIn);
    friend bool operator==(const Listener& a,const Listener& b);
    uint64_t hashCode();


   private:
      ICriterionInstance* criterionInstance;
      Advancement advancement;
      std::string criterionName;
   }

private:
};
