#include "EnumPlayerModelParts.h"

#include "text/TextComponentTranslation.h"

EnumPlayerModelParts EnumPlayerModelParts::CAPE            = EnumPlayerModelParts(0, "cape");
EnumPlayerModelParts EnumPlayerModelParts::JACKET          = EnumPlayerModelParts(1, "jacket");
EnumPlayerModelParts EnumPlayerModelParts::LEFT_SLEEVE     = EnumPlayerModelParts(2, "left_sleeve");
EnumPlayerModelParts EnumPlayerModelParts::RIGHT_SLEEVE    = EnumPlayerModelParts(3, "right_sleeve");
EnumPlayerModelParts EnumPlayerModelParts::LEFT_PANTS_LEG  = EnumPlayerModelParts(4, "left_pants_leg");
EnumPlayerModelParts EnumPlayerModelParts::RIGHT_PANTS_LEG = EnumPlayerModelParts(5, "right_pants_leg");
EnumPlayerModelParts EnumPlayerModelParts::HAT             = EnumPlayerModelParts(6, "hat");

EnumPlayerModelParts::EnumPlayerModelParts(int32_t partIdIn, std::string_view partNameIn)
    : partId(partIdIn), partMask(1 << partIdIn), partName(),
      name(new TextComponentTranslation("options.modelPart." + partNameIn, {}))
{
}

int32_t EnumPlayerModelParts::getPartMask() const
{
    return partMask;
}

int32_t EnumPlayerModelParts::getPartId() const
{
    return partId;
}

std::string_view EnumPlayerModelParts::getPartName() const
{
    return partName;
}

ITextComponent *EnumPlayerModelParts::getName() const
{
    return name;
}
