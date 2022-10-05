#include "EntityFallingBlock.h"

#include "DamageSource.h"
#include "ITileEntityProvider.h"

DataParameter EntityFallingBlock::ORIGIN =
    EntityDataManager::createKey(EntityFallingBlock.class, DataSerializers::BLOCK_POS);

EntityFallingBlock::EntityFallingBlock(World *worldIn) : Entity(worldIn)
{
}

EntityFallingBlock::EntityFallingBlock(World *worldIn, double x, double y, double z, IBlockState *fallingBlockState)
    : Entity(worldIn), fallTile(fallingBlockState)
{
    preventEntitySpawning = true;
    setSize(0.98F, 0.98F);
    setPosition(x, y + (double)((1.0F - height) / 2.0F), z);
    motionX  = 0.0;
    motionY  = 0.0;
    motionZ  = 0.0;
    prevPosX = x;
    prevPosY = y;
    prevPosZ = z;
    setOrigin(BlockPos(this));
}

bool EntityFallingBlock::canBeAttackedWithItem()
{
    return false;
}

void EntityFallingBlock::setOrigin(const BlockPos &p_184530_1_)
{
    dataManager.set(ORIGIN, p_184530_1_);
}

BlockPos EntityFallingBlock::getOrigin()
{
    return dataManager.get(ORIGIN);
}

bool EntityFallingBlock::canBeCollidedWith()
{
    return !isDead;
}

void EntityFallingBlock::onUpdate()
{
    Block *block = fallTile->getBlock();
    if (fallTile->getMaterial() == Material::AIR)
    {
        setDead();
    }
    else
    {
        prevPosX = posX;
        prevPosY = posY;
        prevPosZ = posZ;
        BlockPos blockpos1;
        if (fallTime++ == 0)
        {
            blockpos1 = BlockPos(this);
            if (world->getBlockState(blockpos1)->getBlock() == block)
            {
                world->setBlockToAir(blockpos1);
            }
            else if (!world->isRemote)
            {
                setDead();
                return;
            }
        }

        if (!hasNoGravity())
        {
            motionY -= 0.03999999910593033;
        }

        move(MoverType::SELF, motionX, motionY, motionZ);
        if (!world->isRemote)
        {
            blockpos1  = BlockPos(this);
            bool flag  = fallTile->getBlock() == Blocks::CONCRETE_POWDER;
            bool flag1 = flag && world->getBlockState(blockpos1)->getMaterial() == Material::WATER;
            double d0  = motionX * motionX + motionY * motionY + motionZ * motionZ;
            if (flag && d0 > 1.0)
            {
                auto raytraceresult =
                    world->rayTraceBlocks(Vec3d(prevPosX, prevPosY, prevPosZ), Vec3d(posX, posY, posZ), true);
                if (raytraceresult != std::nullopt &&
                    world->getBlockState(raytraceresult.getBlockPos()).getMaterial() == Material::WATER)
                {
                    blockpos1 = raytraceresult.getBlockPos();
                    flag1     = true;
                }
            }

            if (!onGround && !flag1)
            {
                if (fallTime > 100 && !world->isRemote && (blockpos1.gety() < 1 || blockpos1.gety() > 256) ||
                    fallTime > 600)
                {
                    if (shouldDropItem && world->getGameRules().getBoolean("doEntityDrops"))
                    {
                        entityDropItem(ItemStack(block, 1, block->damageDropped(fallTile)), 0.0F);
                    }

                    setDead();
                }
            }
            else
            {
                IBlockState *iblockstate = world->getBlockState(blockpos1);
                if (!flag1 && BlockFalling::canFallThrough(
                                  world->getBlockState(BlockPos(posX, posY - 0.009999999776482582, posZ))))
                {
                    onGround = false;
                    return;
                }

                motionX *= 0.699999988079071;
                motionZ *= 0.699999988079071;
                motionY *= -0.5;
                if (iblockstate->getBlock() != Blocks::PISTON_EXTENSION)
                {
                    setDead();
                    if (dontSetBlock)
                    {
                        if (Util:: instanceof <BlockFalling>(block))
                        {
                            ((BlockFalling *)block)->onBroken(world, blockpos1);
                        }
                    }
                    else if (world->mayPlace(block, blockpos1, true, EnumFacing::UP, nullptr) &&
                             (flag1 || !BlockFalling::canFallThrough(world->getBlockState(blockpos1.down()))) &&
                             world->setBlockState(blockpos1, fallTile, 3))
                    {
                        if (Util:: instanceof <BlockFalling>(block))
                        {
                            ((BlockFalling *)block)->onEndFalling(world, blockpos1, fallTile, iblockstate);
                        }

                        if (tileEntityData != nullptr && Util:: instanceof <ITileEntityProvider>(block))
                        {
                            TileEntity tileentity = world->getTileEntity(blockpos1);
                            if (tileentity != nullptr)
                            {
                                NBTTagCompound *nbttagcompound = tileentity.writeToNBT(new NBTTagCompound());

                                for (auto s : tileEntityData->getKeySet())
                                {
                                    NBTBase *nbtbase = tileEntityData->getTag(s);
                                    if (!"x" == s && !"y" == s && !"z" == s)
                                    {
                                        nbttagcompound->setTag(s, nbtbase.copy());
                                    }
                                }

                                tileentity.readFromNBT(nbttagcompound);
                                tileentity.markDirty();
                            }
                        }
                    }
                    else if (shouldDropItem && world->getGameRules().getBoolean("doEntityDrops"))
                    {
                        entityDropItem(ItemStack(block, 1, block->damageDropped(fallTile)), 0.0F);
                    }
                }
            }
        }

        motionX *= 0.9800000190734863;
        motionY *= 0.9800000190734863;
        motionZ *= 0.9800000190734863;
    }
}

void EntityFallingBlock::fall(float distance, float damageMultiplier)
{
    Block block = fallTile->getBlock();
    if (hurtEntities)
    {
        auto i = MathHelper::ceil(distance - 1.0F);
        if (i > 0)
        {
            auto list = world->getEntitiesWithinAABBExcludingEntity(this, getEntityBoundingBox());
            bool flag = block == Blocks.ANVIL;
            DamageSource::DamageSource damagesource = flag ? DamageSource::ANVIL : DamageSource::FALLING_BLOCK;

            for (auto entity : list)
            {
                entity->attackEntityFrom(
                    damagesource, (float)MathHelper::min(MathHelper::floor((float)i * fallHurtAmount), fallHurtMax));
            }

            if (flag && (double)MathHelper::nextFloat(rand) < 0.05000000074505806 + (double)i * 0.05)
            {
                auto j = fallTile->getValue(BlockAnvil::DAMAGE);
                ++j;
                if (j > 2)
                {
                    dontSetBlock = true;
                }
                else
                {
                    fallTile = fallTile->withProperty(BlockAnvil::DAMAGE, j);
                }
            }
        }
    }
}

void EntityFallingBlock::registerFixesFallingBlock(DataFixer fixer)
{
}

World *EntityFallingBlock::getWorldObj() const
{
    return world;
}

void EntityFallingBlock::setHurtEntities(bool p_145806_1_)
{
    hurtEntities = p_145806_1_;
}

bool EntityFallingBlock::canRenderOnFire()
{
    return false;
}

void EntityFallingBlock::addEntityCrashInfo(CrashReportCategory category)
{
    Entity::addEntityCrashInfo(category);
    if (fallTile != nullptr)
    {
        Block block = fallTile->getBlock();
        category.addCrashSection("Immitating block ID", Block::getIdFromBlock(block));
        category.addCrashSection("Immitating block data", block.getMetaFromState(fallTile));
    }
}

IBlockState *EntityFallingBlock::getBlock() const
{
    return fallTile;
}

bool EntityFallingBlock::ignoreItemEntityData()
{
    return true;
}

bool EntityFallingBlock::canTriggerWalking()
{
    return false;
}

void EntityFallingBlock::entityInit()
{
    dataManager.registe(ORIGIN, BlockPos::ORIGIN);
}

void EntityFallingBlock::writeEntityToNBT(NBTTagCompound *compound)
{
    Block block           = fallTile != nullptr ? fallTile->getBlock() : Blocks::AIR;
    auto resourcelocation = Block::REGISTRY.getNameForObject(block);
    compound->setString("Block", resourcelocation == std::nullopt ? "" : resourcelocation.to_string());
    compound->setByte("Data", block.getMetaFromState(fallTile));
    compound->setInteger("Time", fallTime);
    compound->setBoolean("DropItem", shouldDropItem);
    compound->setBoolean("HurtEntities", hurtEntities);
    compound->setFloat("FallHurtAmount", fallHurtAmount);
    compound->setInteger("FallHurtMax", fallHurtMax);
    if (tileEntityData != nullptr)
    {
        compound->setTag("TileEntityData", tileEntityData);
    }
}

void EntityFallingBlock::readEntityFromNBT(NBTTagCompound *compound)
{
    auto i = compound->getByte("Data") & 255;
    if (compound->hasKey("Block", 8))
    {
        fallTile = Block::getBlockFromName(compound->getString("Block"))->getStateFromMeta(i);
    }
    else if (compound->hasKey("TileID", 99))
    {
        fallTile = Block::getBlockById(compound->getInteger("TileID"))->getStateFromMeta(i);
    }
    else
    {
        fallTile = Block::getBlockById(compound->getByte("Tile") & 255)->getStateFromMeta(i);
    }

    fallTime     = compound->getInteger("Time");
    Block *block = fallTile->getBlock();
    if (compound->hasKey("HurtEntities", 99))
    {
        hurtEntities   = compound->getBoolean("HurtEntities");
        fallHurtAmount = compound->getFloat("FallHurtAmount");
        fallHurtMax    = compound->getInteger("FallHurtMax");
    }
    else if (block == Blocks::ANVIL)
    {
        hurtEntities = true;
    }

    if (compound->hasKey("DropItem", 99))
    {
        shouldDropItem = compound->getBoolean("DropItem");
    }

    if (compound->hasKey("TileEntityData", 10))
    {
        tileEntityData = compound->getCompoundTag("TileEntityData");
    }

    if (block == nullptr || block->getDefaultState()->getMaterial() == Material::AIR)
    {
        fallTile = Blocks::SAND.getDefaultState();
    }
}
