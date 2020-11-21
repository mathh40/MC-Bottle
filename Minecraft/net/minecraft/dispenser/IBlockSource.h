#pragma once
#include "ILocatableSource.h"
#include "math/BlockPos.h"
class IBlockState;
class TileEntity;

class IBlockSource :public ILocatableSource {
public:
   virtual double getx() = 0;
   virtual double gety() = 0;
   virtual double getz() = 0;
   virtual BlockPos getBlockPos() = 0;
   virtual IBlockState* getBlockState() = 0;
   virtual TileEntity* getBlockTileEntity() = 0;
};
