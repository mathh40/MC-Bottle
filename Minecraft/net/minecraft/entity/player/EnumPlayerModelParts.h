#pragma once
#include <string_view>

class ITextComponent;

class EnumPlayerModelParts {
public:
    static EnumPlayerModelParts CAPE;
    static EnumPlayerModelParts JACKET;
    static EnumPlayerModelParts LEFT_SLEEVE;
    static EnumPlayerModelParts RIGHT_SLEEVE;
    static EnumPlayerModelParts LEFT_PANTS_LEG;
    static EnumPlayerModelParts RIGHT_PANTS_LEG;
    static EnumPlayerModelParts HAT;
    EnumPlayerModelParts(int32_t partIdIn, std::string_view partNameIn);

   int32_t getPartMask() const;

   int32_t getPartId() const;

   std::string_view getPartName() const;

   ITextComponent *getName() const;

private:
   int32_t partId;
   int32_t partMask;
   std::string_view partName;
   ITextComponent* name;

};

