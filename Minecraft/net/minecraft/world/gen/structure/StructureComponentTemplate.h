#pragma once
#include "StructureComponent.h"
#include "math/BlockPos.h"

class TemplateManager;
class World;

class StructureComponentTemplate : public StructureComponent
{
  public:
    StructureComponentTemplate();
    explicit StructureComponentTemplate(int32_t type);

    void offset(int32_t x, int32_t y, int32_t z) override;

  protected:
    Template templates;
    PlacementSettings placeSettings;
    MutableBlockPos templatePosition;
    void setup(const Template &templateIn, BlockPos &pos, const PlacementSettings &settings);
    void writeStructureToNBT(NBTTagCompound *tagCompound) override;
    void readStructureFromNBT(NBTTagCompound *tagCompound, const TemplateManager &p_143011_2_) override;
    bool addComponentParts(World *worldIn, pcg32 &randomIn, StructureBoundingBox structureBoundingBoxIn) override;
    virtual void handleDataMarker(std::string_view var1, BlockPos var2, World *var3, pcg32 &var4,
                                  const StructureBoundingBox &var5) = 0;

  private:
    static const PlacementSettings DEFAULT_PLACE_SETTINGS;
    void setBoundingBoxFromTemplate();
};
