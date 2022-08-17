#pragma once
#include "registry/RegistryNamespaced.h"
#include "spdlog/logger.h"
#include "../world/World.h"

class CrashReportCategory;

class TileEntity
{
public:
   TileEntity();
   std::optional<ResourceLocation> getKey(TileEntity* clazz) const;
   World* getWorld() const;
   void setWorld(World* worldIn);
   bool hasWorld() const;
   virtual void readFromNBT(NBTTagCompound* compound);
   virtual NBTTagCompound* writeToNBT(NBTTagCompound* compound);
   virtual int32_t getBlockMetadata();
   void markDirty();
   double getDistanceSq(double x, double y, double z) const;
   static double getMaxRenderDistanceSquared();
   BlockPos getPos() const;
   Block* getBlockType();
   virtual std::optional<SPacketUpdateTileEntity> getUpdatePacket();
   virtual NBTTagCompound* getUpdateTag();
   bool isInvalid() const;
   virtual void invalidate();
   virtual void validate();
   virtual bool receiveClientEvent(int32_t id, int32_t type);
   virtual void updateContainingBlockInfo();
   void addInfoToCrashReport(CrashReportCategory reportCategory);
   void setPos(BlockPos posIn);
 virtual   bool onlyOpsCanSetNbt();
   virtual ITextComponent* getDisplayName();
   virtual void rotate(Rotation rotationIn);
   virtual void mirror(Mirror mirrorIn);

   static TileEntity create(World* worldIn, NBTTagCompound* compound);
protected:
   virtual void setWorldCreate(World* worldIn);

   World* world;
   BlockPos pos;
   bool tileEntityInvalid;
   Block* blockType;
private:
   static void regist(std::string_view id, TileEntity* clazz);
   NBTTagCompound* writeInternal(NBTTagCompound* compound);

   static std::shared_ptr<spdlog::logger> LOGGER;
   static RegistryNamespaced<ResourceLocation,TileEntity*> REGISTRY;
   int32_t blockMetadata;
};
