#include "TileEntityEndGateway.h"
#include "NBTUtil.h"

std::shared_ptr<spdlog::logger> BlockAnvil::LOGGER = spdlog::get("Minecraft")->clone("TileEntityEndGateway");

NBTTagCompound* TileEntityEndGateway::writeToNBT(NBTTagCompound* compound)
{
	TileEntityEndPortal::writeToNBT(compound);
      compound->setLong("Age", age);
      if (exitPortal.has_value) {
         compound->setTag("ExitPortal", NBTUtil::createPosTag(exitPortal));
      }

      if (exactTeleport) {
         compound->setBoolean("ExactTeleport", exactTeleport);
      }

      return compound;
}

void TileEntityEndGateway::readFromNBT(NBTTagCompound* compound)
{
    TileEntityEndPortal::readFromNBT(compound);
    age = compound->getLong("Age");
    if (compound->hasKey("ExitPortal", 10)) 
    {
        exitPortal = NBTUtil::getPosFromTag(compound->getCompoundTag("ExitPortal"));
    }

    exactTeleport = compound->getBoolean("ExactTeleport");
}

double TileEntityEndGateway::getMaxRenderDistanceSquared() const
{
    return 65536.0;
}

void TileEntityEndGateway::update()
{
    boolean flag = isSpawning();
    boolean flag1 = isCoolingDown();
    ++age;
    if (flag1) 
    {
        --teleportCooldown;
    } 
    else if (!world->isRemote) 
    {
        auto list = world->getEntitiesWithinAABB<Entity>( AxisAlignedBB(getPos()));
        if (!list.empty()) 
        {
            teleportEntity((Entity)list.get(0));
        }

        if (age % 2400L == 0L) 
        {
            triggerCooldown();
        }
    }

    if (flag != isSpawning() || flag1 != isCoolingDown()) 
    {
        markDirty();
    }
}

bool TileEntityEndGateway::isSpawning()
{
    return age < 200L;
}

bool TileEntityEndGateway::isCoolingDown()
{
    return teleportCooldown > 0;
}

float TileEntityEndGateway::getSpawnPercent(float p_184302_1_)
{
    return MathHelper::clamp(((float)age + p_184302_1_) / 200.0F, 0.0F, 1.0F);
}

float TileEntityEndGateway::getCooldownPercent(float p_184305_1_)
{
    return 1.0F - MathHelper::clamp(((float)teleportCooldown - p_184305_1_) / 40.0F, 0.0F, 1.0F);
}

std::optional<SPacketUpdateTileEntity> TileEntityEndGateway::getUpdatePacket()
{
    return SPacketUpdateTileEntity(pos, 8, getUpdateTag());
}

NBTTagCompound* TileEntityEndGateway::getUpdateTag()
{
    return writeToNBT(new NBTTagCompound());
}

void TileEntityEndGateway::triggerCooldown()
{
    if (!world->isRemote) 
    {
         teleportCooldown = 40;
         world->addBlockEvent(getPos(), getBlockType(), 1, 0);
         markDirty();
    }
}

bool TileEntityEndGateway::receiveClientEvent(int32_t id, int32_t type)
{
    if (id == 1) 
    {
         teleportCooldown = 40;
         return true;
    } 
    else 
    {
        return TileEntityEndPortal::receiveClientEvent(id, type);
    }
}

void TileEntityEndGateway::teleportEntity(Entity* entityIn)
{
    if (!world->isRemote && !isCoolingDown()) 
    {
        teleportCooldown = 100;
        if (!exitPortal.has_value &&  Util::instanceof<WorldProviderEnd>(world->provider)) 
        {
            findExitPortal();
        }

        if (exitPortal.has_value) 
        {
            BlockPos blockpos = exactTeleport ? exitPortal : findExitPosition();
            entityIn->setPositionAndUpdate((double)blockpos.getx() + 0.5, (double)blockpos.gety() + 0.5, (double)blockpos.getz() + 0.5);
        }

        triggerCooldown();
    }
}

BlockPos TileEntityEndGateway::findExitPosition()
{
    BlockPos blockpos = findHighestBlock(world, exitPortal, 5, false);
    LOGGER->debug("Best exit position for portal at {} is {}", exitPortal, blockpos);
    return blockpos.up();
}

void TileEntityEndGateway::findExitPortal()
{
    Vec3d vec3d = (Vec3d((double)getPos().getx(), 0.0, (double)getPos().getz())).normalize();
    Vec3d vec3d1 = vec3d.scale(1024.0);

    int var3;
    for(var3 = 16; getChunk(world, vec3d1).getTopFilledSegment() > 0 && var3-- > 0; vec3d1 = vec3d1.add(vec3d.scale(-16.0))) 
    {
        LOGGER->debug("Skipping backwards past nonempty chunk at {}", vec3d1);
    }

    for(var3 = 16; getChunk(world, vec3d1).getTopFilledSegment() == 0 && var3-- > 0; vec3d1 = vec3d1.add(vec3d.scale(16.0))) 
    {
        LOGGER->debug("Skipping forward past empty chunk at {}", vec3d1);
    }

    LOGGER->debug("Found chunk at {}", vec3d1);
    Chunk chunk = getChunk(world, vec3d1);
    exitPortal = findSpawnpointInChunk(chunk);
    if (!exitPortal.has_value) 
    {
        exitPortal = BlockPos(vec3d1.getx() + 0.5, 75.0, vec3d1.getz ()+ 0.5);
        LOGGER->debug("Failed to find suitable block, settling on {}", exitPortal);
        (WorldGenEndIsland()).generate(world, pcg32(exitPortal.value().toLong), exitPortal);
    } 
    else 
    {
        LOGGER->debug("Found block at {}", exitPortal);
    }

    exitPortal = findHighestBlock(world, exitPortal, 16, true);
    LOGGER->debug("Creating portal at {}", exitPortal);
    exitPortal = exitPortal.value().up(10);
    createExitPortal(exitPortal);
    markDirty();
}

BlockPos TileEntityEndGateway::findHighestBlock(World* p_184308_0_, BlockPos p_184308_1_, int32_t p_184308_2_, bool p_184308_3_)
{
    BlockPos blockpos;

    for(int32_t i = -p_184308_2_; i <= p_184308_2_; ++i) 
    {
        for(int32_t j = -p_184308_2_; j <= p_184308_2_; ++j) 
        {
        if (i != 0 || j != 0 || p_184308_3_) 
        {
            for(int32_t k = 255; k > (blockpos == null ? 0 : blockpos.gety()); --k) 
            {
                BlockPos blockpos1 = BlockPos(p_184308_1_.getx() + i, k, p_184308_1_.getz() + j);
                IBlockState* iblockstate = p_184308_0_->getBlockState(blockpos1);
                if (iblockstate->isBlockNormalCube() && (p_184308_3_ || iblockstate->getBlock() != Blocks::BEDROCK)) 
                {
                    blockpos = blockpos1;
                    break;
                }
            }
        }
        }
    }

    return blockpos == null ? p_184308_1_ : blockpos;
}
