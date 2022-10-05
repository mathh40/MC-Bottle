#include "FlatLayerInfo.h"

#include "Block.h"
#include "ResourceLocation.h"

FlatLayerInfo::FlatLayerInfo(int32_t p_i45467_1_, Block *layerMaterialIn)
    : FlatLayerInfo(3, p_i45467_1_, layerMaterialIn)
{
}

FlatLayerInfo::FlatLayerInfo(int32_t p_i45627_1_, int32_t height, Block *layerMaterialIn)
    : layerCount(1), version(p_i45627_1_), layerCount(height), layerMaterial(layerMaterialIn->getDefaultState())
{
}

FlatLayerInfo::FlatLayerInfo(int32_t p_i45628_1_, int32_t p_i45628_2_, Block *layerMaterialIn, int32_t p_i45628_4_)
    : FlatLayerInfo(p_i45628_1_, p_i45628_2_, layerMaterialIn),
      layerMaterial(layerMaterialIn->getStateFromMeta(p_i45628_4_))
{
}

int32_t FlatLayerInfo::getLayerCount() const
{
    return layerCount;
}

IBlockState *FlatLayerInfo::getLayerMaterial() const
{
    return layerMaterial;
}

int32_t FlatLayerInfo::getMinY() const
{
    return layerMinimumY;
}

void FlatLayerInfo::setMinY(int32_t minY)
{
    layerMinimumY = minY;
}

std::string FlatLayerInfo::toString()
{
    std::string s;
    if (version >= 3)
    {
        auto resourcelocation = Block::REGISTRY.getNameForObject(getLayerMaterialBlock());
        s                     = resourcelocation ? resourcelocation.value().to_string() : "null";
        if (layerCount > 1)
        {
            s = layerCount + "*" + s;
        }
    }
    else
    {
        s = std::to_string(Block::getIdFromBlock(getLayerMaterialBlock()));
        if (layerCount > 1)
        {
            s = layerCount + "x" + s;
        }
    }

    auto i = getFillBlockMeta();
    if (i > 0)
    {
        s = s + ":" + std::to_string(i);
    }

    return s;
}

Block *FlatLayerInfo::getLayerMaterialBlock()
{
    return layerMaterial->getBlock();
}

int32_t FlatLayerInfo::getFillBlockMeta()
{
    return layerMaterial->getBlock()->getMetaFromState(layerMaterial);
}
