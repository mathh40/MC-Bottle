#pragma once
#include "../util/ByteBuffer.h"
#include "../util/ResourceLocation.h"

#include <optional>
#include <set>
#include <string>

class ITextComponent;

class Advancement
{
  public:
    class Builder
    {
      public:
        Builder(std::optional<ResourceLocation> parentIdIn, std::optional<DisplayInfo> displayIn,
                AdvancementRewards rewardsIn, Map criteriaIn, std::string[][] requirementsIn);
        template <typename T> bool resolveParent(T lookup);
        Advancement build(const ResourceLocation &id);
        void writeTo(ByteBuffer &buf);
        std::string toString();
        static Builder deserialize(Json json);
        static Builder readFrom(ByteBuffer &buf);

      private:
        ResourceLocation parentId;
        Advancement parent;
        DisplayInfo display;
        AdvancementRewards rewards;
        Map criteria;
        std::string[][] requirements;
    };

    Advancement(ResourceLocation id, std::optional<Advancement> parentIn, std::optional<DisplayInfo> displayIn,
                AdvancementRewards rewardsIn, Map criteriaIn, std::string[][] requirementsIn);
    Advancement *getParent();
    DisplayInfo getDisplay();
    AdvancementRewards getRewards();
    std::string toString() Iterable getChildren();
    Map getCriteria();
    uint32_t getRequirementCount();
    void addChild(Advancement *advancementIn);
    ResourceLocation getId();
    std::string[][] getRequirements();
    ITextComponent *getDisplayText();

    friend bool operator==(const Advancement &a, const Advancement &b);

  private:
    Advancement *parent;
    DisplayInfo display;
    AdvancementRewards rewards;
    ResourceLocation id;
    Map criteria;
    std::string[][] requirements;
    std::set<Advancement> children;
    ITextComponent *displayText;
};

template <typename T> bool Advancement::Builder::resolveParent(T lookup)
{
    return false;
}

bool operator==(const Advancement &a, const Advancement &b)
{
    return a.id == b.id;
}

namespace std
{
template <> struct hash<Advancement>
{
    size_t operator()(const Advancement &x) const noexcept
    {
        return std::hash<ResourceLocation>(x);
    }
};
} // namespace std
