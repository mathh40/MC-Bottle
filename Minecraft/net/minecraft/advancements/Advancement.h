#pragma once
#include <string>
#include "../util/ByteBuffer.h"

class Advancment
{
public:

    class Builder
  {
    public:
        Builder(std::optional<ResourceLocation> parentIdIn, std::optional<DisplayInfo> displayIn, AdvancementRewards rewardsIn, Map criteriaIn, String[][] requirementsIn);
        template<typename T>
        bool resolveParent(T lookup);
        Advancement build(const ResourceLocation& id);
        void writeTo(ByteBuffer& buf);
        std::string toString()



    private:
      ResourceLocation parentId;
      Advancement parent;
      DisplayInfo display;
      AdvancementRewards rewards;
      Map criteria;
      String[][] requirements;
  };

    Advancement(ResourceLocation id, @Nullable Advancement parentIn, @Nullable DisplayInfo displayIn, AdvancementRewards rewardsIn, Map criteriaIn, String[][] requirementsIn);
    Advancement* getParent();
    DisplayInfo getDisplay();
    AdvancementRewards getRewards();
    std::string toString()
    Iterable getChildren();
    Map getCriteria();
    uint32_t getRequirementCount()
    void addChild(Advancemen* advancementIn);
    ResourceLocation getId();
    String[][] getRequirements();
    ITextComponent* getDisplayText();

    friend bool operator==(const Advancement& a,const Advancement& b);


private:
   Advancement* parent;
   DisplayInfo display;
   AdvancementRewards rewards;
   ResourceLocation id;
   Map criteria;
   String[][] requirements;
   Set children = Sets.newLinkedHashSet();
   ITextComponent* displayText;
};

bool operator==(const Advancement& a, const Advancement& b) {
    return a.id = b.id;
}


namespace std {
  template <> struct hash<Foo>
  {
    size_t operator()(const Foo & x) const noexcept
    {
      return std::hash<ResourceLocation>(id);
    }
  };
}