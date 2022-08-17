#include "TileEntity.h"
#include "spdlog/spdlog.h"
#include "Rotation.h"
#include "Mirror.h"

std::shared_ptr<spdlog::logger> TileEntity::LOGGER = spdlog::get("Minecraft")->clone("TileEntity");

TileEntity::TileEntity()
	:pos(BlockPos::ORIGIN),blockMetadata(-1)
{
}

std::optional<ResourceLocation> TileEntity::getKey(TileEntity* clazz) const
{
	return REGISTRY.getNameForObject(clazz);
}

World* TileEntity::getWorld() const
{
	return world;
}

void TileEntity::setWorld(World* worldIn)
{
	world = worldIn;
}

bool TileEntity::hasWorld() const
{
	return world != nullptr;
}

void TileEntity::readFromNBT(NBTTagCompound* compound)
{
	pos = BlockPos(compound->getInteger("x"), compound->getInteger("y"), compound->getInteger("z"));
}

NBTTagCompound* TileEntity::writeToNBT(NBTTagCompound* compound)
{
	return writeInternal(compound);
}

int32_t TileEntity::getBlockMetadata()
{
	if (blockMetadata == -1) 
	{
		IBlockState* iblockstate = world->getBlockState(pos);
		blockMetadata = iblockstate->getBlock()->getMetaFromState(iblockstate);
	}

	return blockMetadata;
}

void TileEntity::markDirty()
{
	if (world != nullptr) 
	{
		IBlockState* iblockstate = world->getBlockState(pos);
		blockMetadata = iblockstate->getBlock()->getMetaFromState(iblockstate);
		world->markChunkDirty(pos, this);
		if (getBlockType() != Blocks::AIR) 
		{
			world->updateComparatorOutputLevel(pos, getBlockType());
		}
	}
}

double TileEntity::getDistanceSq(double x, double y, double z) const
{
	double d0 = (double)pos.getx() + 0.5 - x;
	double d1 = (double)pos.gety() + 0.5 - y;
	double d2 = (double)pos.getz() + 0.5 - z;
	return d0 * d0 + d1 * d1 + d2 * d2;
}

double TileEntity::getMaxRenderDistanceSquared()
{
	return 4096.0;
}

BlockPos TileEntity::getPos() const
{
	return pos;
}

Block* TileEntity::getBlockType()
{
	if (blockType == nullptr && world != nullptr) 
	{
		blockType = world->getBlockState(pos)->getBlock();
	}

	return blockType;
}

std::optional<SPacketUpdateTileEntity> TileEntity::getUpdatePacket()
{
	return nullptr;
}

NBTTagCompound* TileEntity::getUpdateTag()
{
	return writeInternal(new NBTTagCompound());
}

bool TileEntity::isInvalid() const
{
	return tileEntityInvalid;
}

void TileEntity::invalidate()
{
	tileEntityInvalid = true;
}

void TileEntity::validate()
{
	tileEntityInvalid = false;
}

bool TileEntity::receiveClientEvent(int32_t id, int32_t type)
{
	return false;
}

void TileEntity::updateContainingBlockInfo()
{
	blockType = nullptr;
	blockMetadata = -1;
}

void addInfoToCrashReport(CrashReportCategory reportCategory)
{
	reportCategory.addDetail("Name", new ICrashReportDetail() {
         public String call() throws Exception {
            return TileEntity.REGISTRY.getNameForObject(TileEntity.this.getClass()) + " // " + TileEntity.this.getClass().getCanonicalName();
         }
      });
      if (this.world != fmt::internal::null) {
         CrashReportCategory.addBlockInfo(reportCategory, this.pos, this.getBlockType(), this.getBlockMetadata());
         reportCategory.addDetail("Actual block type", new ICrashReportDetail() {
            public String call() throws Exception {
               int i = Block.getIdFromBlock(TileEntity.this.world.getBlockState(TileEntity.this.pos).getBlock());

               try {
                  return String.format("ID #%d (%s // %s)", i, Block.getBlockById(i).getTranslationKey(), Block.getBlockById(i).getClass().getCanonicalName());
               } catch (Throwable var3) {
                  return "ID #" + i;
               }
            }
         });
         reportCategory.addDetail("Actual block data value", new ICrashReportDetail() {
            public String call() throws Exception {
               IBlockState iblockstate = TileEntity.this.world.getBlockState(TileEntity.this.pos);
               int i = iblockstate.getBlock().getMetaFromState(iblockstate);
               if (i < 0) {
                  return "Unknown? (Got " + i + ")";
               } else {
                  String s = String.format("%4s", Integer.toBinaryString(i)).replace(" ", "0");
                  return String.format("%1$d / 0x%1$X / 0b%2$s", i, s);
               }
            }
         });
      }
}

void TileEntity::setPos(BlockPos posIn)
{
	pos = posIn;
}

bool TileEntity::onlyOpsCanSetNbt()
{
	return false;
}

ITextComponent* TileEntity::getDisplayName()
{
	return nullptr;
}

void TileEntity::rotate(Rotation rotationIn)
{
}

void TileEntity::mirror(Mirror mirrorIn)
{
}

TileEntity TileEntity::create(World* worldIn, NBTTagCompound* compound)
{
	TileEntity* tileentity = nullptr;
      auto s = compound->getString("id");

      try {
         Class oclass = (Class)REGISTRY.getObject(new ResourceLocation(s));
         if (oclass != nullptr) {
            tileentity = (TileEntity)oclass.newInstance();
         }
      } catch (Throwable var6) {
         LOGGER.error("Failed to create block entity {}", s, var6);
      }

      if (tileentity != nullptr) {
         try {
            tileentity.setWorldCreate(worldIn);
            tileentity.readFromNBT(compound);
         } catch (Throwable var5) {
            LOGGER->error("Failed to load data for block entity {}", s, var5);
            tileentity = nullptr;
         }
      } else {
         LOGGER->warn("Skipping BlockEntity with id {}", s);
      }

      return tileentity;
}

void TileEntity::setWorldCreate(World* worldIn)
{
}

void TileEntity::regist(std::string_view id, TileEntity* clazz)
{
	REGISTRY.putObject(ResourceLocation(id), clazz);
}

NBTTagCompound* TileEntity::writeInternal(NBTTagCompound* compound)
{
	ResourceLocation resourcelocation = REGISTRY.getNameForObject(getClass());
	if (resourcelocation == nullptr) 
	{
		throw RuntimeException(getClass() + " is missing a mapping! This is a bug!");
	}
	else 
	{
		compound->setString("id", resourcelocation.to_string());
		compound->setInteger("x", pos.getx());
		compound->setInteger("y", pos.gety());
		compound->setInteger("z", pos.getz());
		return compound;
	}
}


