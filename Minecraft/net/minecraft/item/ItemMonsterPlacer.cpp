#include "ItemMonsterPlacer.h"

#include "../pathfinding/NodeProcessor.h"
#include "../scoreboard/ServerScoreboard.h"
#include "../stats/StatList.h"
#include "../tileentity/MobSpawnerBaseLogic.h"
#include "../tileentity/TileEntityMobSpawner.h"
#include "ItemStack.h"
#include "text/translation/I18n.h"

ItemMonsterPlacer::ItemMonsterPlacer()
{
    setCreativeTab(CreativeTabs::MISC);
}

std::string ItemMonsterPlacer::getItemStackDisplayName(ItemStack stack) const
{
    std::string s  = ("" + I18n::translateToLocal(getTranslationKey() + ".name"));
    std::string s1 = EntityList::getTranslationName(getNamedIdFrom(stack));
    if (!s1.empty())
    {
        s = s + " " + I18n::translateToLocal("entity." + s1 + ".name");
    }

    return s;
}

EnumActionResult ItemMonsterPlacer::onItemUse(EntityPlayer *player, World *worldIn, BlockPos pos, EnumHand hand,
                                              EnumFacing facing, float hitX, float hitY, float hitZ)
{
    ItemStack itemstack = player->getHeldItem(hand);
    if (worldIn->isRemote)
    {
        return EnumActionResult::SUCCESS;
    }
    else if (!player->canPlayerEdit(pos.offset(facing), facing, itemstack))
    {
        return EnumActionResult::FAIL;
    }
    else
    {
        IBlockState *iblockstate = worldIn->getBlockState(pos);
        Block block              = iblockstate->getBlock();
        if (block == Blocks::MOB_SPAWNER)
        {
            TileEntity *tileentity = worldIn->getTileEntity(pos);
            if (Util:: instanceof <TileEntityMobSpawner>(tileentity))
            {
                MobSpawnerBaseLogic mobspawnerbaselogic = ((TileEntityMobSpawner *)tileentity).getSpawnerBaseLogic();
                mobspawnerbaselogic.setEntityId(getNamedIdFrom(itemstack));
                tileentity->markDirty();
                worldIn->notifyBlockUpdate(pos, iblockstate, iblockstate, 3);
                if (!player->capabilities.isCreativeMode)
                {
                    itemstack.shrink(1);
                }

                return EnumActionResult::SUCCESS;
            }
        }

        BlockPos blockpos = pos.offset(facing);
        double d0         = getYOffset(worldIn, blockpos);
        Entity *entity    = spawnCreature(worldIn, getNamedIdFrom(itemstack), (double)blockpos.getx() + 0.5,
                                          (double)blockpos.gety() + d0, (double)blockpos.getz() + 0.5);
        if (entity != nullptr)
        {
            if (Util:: instanceof <EntityLivingBase>(entity) && itemstack.hasDisplayName())
            {
                entity->setCustomNameTag(itemstack.getDisplayName());
            }

            applyItemEntityDataToEntity(worldIn, player, itemstack, entity);
            if (!player->capabilities.isCreativeMode)
            {
                itemstack.shrink(1);
            }
        }

        return EnumActionResult::SUCCESS;
    }
}

void ItemMonsterPlacer::applyItemEntityDataToEntity(World *entityWorld, EntityPlayer *player, ItemStack stack,
                                                    Entity *targetEntity)
{
    MinecraftServer *minecraftserver = entityWorld->getMinecraftServer();
    if (minecraftserver != nullptr && targetEntity != nullptr)
    {
        NBTTagCompound *nbttagcompound = stack.getTagCompound();
        if (nbttagcompound != nullptr && nbttagcompound->hasKey("EntityTag", 10))
        {
            if (!entityWorld->isRemote && targetEntity->ignoreItemEntityData() &&
                (player == nullptr || !minecraftserver->getPlayerList().canSendCommands(player->getGameProfile())))
            {
                return;
            }

            NBTTagCompound *nbttagcompound1 = targetEntity->writeToNBT(new NBTTagCompound());
            xg::Guid uuid                   = targetEntity->getUniqueID();
            nbttagcompound1->merge(nbttagcompound->getCompoundTag("EntityTag"));
            targetEntity->setUniqueId(uuid);
            targetEntity->readFromNBT(nbttagcompound1);
        }
    }
}

ActionResult ItemMonsterPlacer::onItemRightClick(World *worldIn, EntityPlayer *playerIn, EnumHand handIn)
{
    ItemStack itemstack = playerIn->getHeldItem(handIn);
    if (worldIn->isRemote)
    {
        return new ActionResult(EnumActionResult.PASS, itemstack);
    }
    else
    {
        RayTraceResult raytraceresult = rayTrace(worldIn, playerIn, true);
        if (raytraceresult != nullptr && raytraceresult.typeOfHit == RayTraceResult::Type::BLOCK)
        {
            BlockPos blockpos = raytraceresult.getBlockPos();
            if (!(Util:: instanceof <BlockLiquid>(worldIn->getBlockState(blockpos)->getBlock())))
            {
                return ActionResult(EnumActionResult::PASS, itemstack);
            }
            else if (worldIn->isBlockModifiable(playerIn, blockpos) &&
                     playerIn->canPlayerEdit(blockpos, raytraceresult.sideHit, itemstack))
            {
                Entity *entity = spawnCreature(worldIn, getNamedIdFrom(itemstack), (double)blockpos.getx() + 0.5,
                                               (double)blockpos.gety() + 0.5, (double)blockpos.getz() + 0.5);
                if (entity == nullptr)
                {
                    return ActionResult(EnumActionResult::PASS, itemstack);
                }
                else
                {
                    if (Util:: instanceof <EntityLivingBase>(entity) && itemstack.hasDisplayName())
                    {
                        entity->setCustomNameTag(itemstack.getDisplayName());
                    }

                    applyItemEntityDataToEntity(worldIn, playerIn, itemstack, entity);
                    if (!playerIn->capabilities.isCreativeMode)
                    {
                        itemstack.shrink(1);
                    }

                    playerIn->addStat(StatList::getObjectUseStats(this));
                    return ActionResult(EnumActionResult::SUCCESS, itemstack);
                }
            }
            else
            {
                return ActionResult(EnumActionResult::FAIL, itemstack);
            }
        }
        else
        {
            return ActionResult(EnumActionResult::PASS, itemstack);
        }
    }
}

Entity *ItemMonsterPlacer::spawnCreature(World *worldIn, ResourceLocation entityID, double x, double y, double z)
{
    if (entityID != nullptr && EntityList::ENTITY_EGGS.containsKey(entityID))
    {
        Entity *entity = nullptr;

        for (auto i = 0; i < 1; ++i)
        {
            entity = EntityList::createEntityByIDFromName(entityID, worldIn);
            if (Util:: instanceof <EntityLiving>(entity))
            {
                EntityLiving *entityliving = (EntityLiving *)entity;
                entity->setLocationAndAngles(
                    x, y, z, MathHelper::wrapDegrees(MathHelper::nextFloat(worldIn->rand) * 360.0F), 0.0F);
                entityliving->rotationYawHead = entityliving->rotationYaw;
                entityliving->renderYawOffset = entityliving->rotationYaw;
                entityliving->onInitialSpawn(worldIn->getDifficultyForLocation(BlockPos(entityliving)), nullptr);
                worldIn->spawnEntity(entity);
                entityliving->playLivingSound();
            }
        }

        return entity;
    }
    else
    {
        return nullptr;
    }
}

void ItemMonsterPlacer::getSubItems(const CreativeTabs &tab, std::vector<ItemStack> &items)
{
    if (this.isInCreativeTab(tab))
    {
        for (auto entitylist$entityegginfo : EntityList::ENTITY_EGGS.values())
        {
            ItemStack itemstack = ItemStack(this, 1);
            applyEntityIdToItemStack(itemstack, entitylist$entityegginfo.spawnedID);
            items.push_back(itemstack);
        }
    }
}

void ItemMonsterPlacer::applyEntityIdToItemStack(ItemStack stack, ResourceLocation entityId)
{
    NBTTagCompound *nbttagcompound  = stack.hasTagCompound() ? stack.getTagCompound() : new NBTTagCompound();
    NBTTagCompound *nbttagcompound1 = new NBTTagCompound();
    nbttagcompound1->setString("id", entityId.to_string());
    nbttagcompound->setTag("EntityTag", nbttagcompound1);
    stack.setTagCompound(nbttagcompound);
}

ResourceLocation ItemMonsterPlacer::getNamedIdFrom(ItemStack stack)
{
    NBTTagCompound *nbttagcompound = stack.getTagCompound();
    if (nbttagcompound == nullptr)
    {
        return nullptr;
    }
    else if (!nbttagcompound->hasKey("EntityTag", 10))
    {
        return nullptr;
    }
    else
    {
        NBTTagCompound *nbttagcompound1 = nbttagcompound->getCompoundTag("EntityTag");
        if (!nbttagcompound1->hasKey("id", 8))
        {
            return nullptr;
        }
        else
        {
            std::string s                     = nbttagcompound1->getString("id");
            ResourceLocation resourcelocation = ResourceLocation(s);
            if (!s.contains(":"))
            {
                nbttagcompound1->setString("id", resourcelocation.toString());
            }

            return resourcelocation;
        }
    }
}

double ItemMonsterPlacer::getYOffset(World *p_190909_1_, BlockPos p_190909_2_)
{
    AxisAlignedBB axisalignedbb = (AxisAlignedBB(p_190909_2_)).expand(0.0, -1.0, 0.0);
    auto list                   = p_190909_1_->getCollisionBoxes(nullptr, axisalignedbb);
    if (list->empty())
    {
        return 0.0;
    }
    else
    {
        double d0 = axisalignedbb.getminY();

        for (auto &axisalignedbb1 : list.value())
        {
            d0 = MathHelper::max(axisalignedbb1.getmaxY(), d0);
        }

        return d0 - (double)p_190909_2_.gety();
    }
}
