#include "TileEntity.h"
#include "ITickable.h"

class DataFixer;

class TileEntityPiston :public TileEntity,ITickable
{
public:
	TileEntityPiston() = default;
   TileEntityPiston(IBlockState* pistonStateIn, EnumFacing pistonFacingIn, bool extendingIn, bool shouldHeadBeRenderedIn);
   IBlockState* getPistonState() const;
   NBTTagCompound* getUpdateTag() override;
   int32_t getBlockMetadata() override;
   bool isExtending() const;
   EnumFacing getFacing() const;
   bool shouldPistonHeadBeRendered() const;
   float getProgress(float ticks) const;
   float getOffsetX(float ticks) const;
   float getOffsetY(float ticks) const;
   float getOffsetZ(float ticks) const;
   AxisAlignedBB getAABB(IBlockAccess* p_184321_1_, BlockPos p_184321_2_);
   AxisAlignedBB getAABB(IBlockAccess* p_184319_1_, BlockPos p_184319_2_, float p_184319_3_);
   void clearPistonTileEntity();
   void update() override;
   static void registerFixesPiston(DataFixer fixer);
   void readFromNBT(NBTTagCompound* compound) override;
   NBTTagCompound* writeToNBT(NBTTagCompound* compound) override;
   void addCollissionAABBs(World* p_190609_1_, BlockPos p_190609_2_, AxisAlignedBB p_190609_3_, std::vector<> p_190609_4_, Entity* p_190609_5_);


private:
   float getExtendedProgress(float p_184320_1_) const;
   IBlockState* getCollisionRelatedBlockState();
   void moveCollidedEntities(float p_184322_1_);
   AxisAlignedBB getMinMaxPiecesAABB(std::vector<AxisAlignedBB> p_191515_1_);
   double getMovement(AxisAlignedBB p_190612_1_, EnumFacing facing, AxisAlignedBB p_190612_3_);
   AxisAlignedBB moveByPositionAndProgress(AxisAlignedBB p_190607_1_);
   AxisAlignedBB getMovementArea(AxisAlignedBB p_190610_1_, EnumFacing p_190610_2_, double p_190610_3_);
   void fixEntityWithinPistonBase(Entity* p_190605_1_, EnumFacing p_190605_2_, double p_190605_3_);
   double getDeltaX(AxisAlignedBB p_190611_0_, EnumFacing facing, AxisAlignedBB p_190611_2_);
   double getDeltaY(AxisAlignedBB p_190608_0_, EnumFacing facing, AxisAlignedBB p_190608_2_);
   double getDeltaZ(AxisAlignedBB p_190604_0_, EnumFacing facing, AxisAlignedBB p_190604_2_);


   IBlockState* pistonState;
   EnumFacing pistonFacing;
   bool extending;
   bool shouldHeadBeRendered;
   static ThreadLocal MOVING_ENTITY = new ThreadLocal()
   {
   protected:
      EnumFacing initialValue() {
         return nullptr;
      }
   };
   float progress;
   float lastProgress;
};
