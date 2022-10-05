#include "TileEntitySkull.h"

#include "Mirror.h"
#include "NBTUtil.h"
#include "Rotation.h"
#include "StringUtils.h"

void TileEntitySkull::setProfileCache(const PlayerProfileCache &profileCacheIn)
{
    profileCache = profileCacheIn;
}

void TileEntitySkull::setSessionService(const MinecraftSessionService &sessionServiceIn)
{
    sessionService = sessionServiceIn;
}

NBTTagCompound *TileEntitySkull::writeToNBT(NBTTagCompound *compound)
{
    TileEntity::writeToNBT(compound);
    compound->setByte("SkullType", (uint8_t)(skullType & 255));
    compound->setByte("Rot", (uint8_t)(skullRotation & 255));
    if (playerProfile.has_value())
    {
        NBTTagCompound *nbttagcompound = new NBTTagCompound();
        NBTUtil::writeGameProfile(nbttagcompound, playerProfile);
        compound->setTag("Owner", nbttagcompound);
    }

    return compound;
}

void TileEntitySkull::readFromNBT(NBTTagCompound *compound)
{
    TileEntity::readFromNBT(compound);
    skullType     = compound->getByte("SkullType");
    skullRotation = compound->getByte("Rot");
    if (skullType == 3)
    {
        if (compound->hasKey("Owner", 10))
        {
            playerProfile = NBTUtil::readGameProfileFromNBT(compound->getCompoundTag("Owner"));
        }
        else if (compound->hasKey("ExtraType", 8))
        {
            std::string s = compound->getString("ExtraType");
            if (!StringUtils::isNullOrEmpty(s))
            {
                playerProfile = GameProfile(nullptr, s);
                updatePlayerProfile();
            }
        }
    }
}

void TileEntitySkull::update()
{
    if (skullType == 5)
    {
        if (world->isBlockPowered(pos))
        {
            dragonAnimated = true;
            ++dragonAnimatedTicks;
        }
        else
        {
            dragonAnimated = false;
        }
    }
}

float TileEntitySkull::getAnimationProgress(float p_184295_1_)
{
    return dragonAnimated ? (float)dragonAnimatedTicks + p_184295_1_ : (float)dragonAnimatedTicks;
}

std::optional<GameProfile> TileEntitySkull::getPlayerProfile()
{
    return playerProfile;
}

std::optional<SPacketUpdateTileEntity> TileEntitySkull::getUpdatePacket()
{
    return SPacketUpdateTileEntity(pos, 4, getUpdateTag());
}

NBTTagCompound *TileEntitySkull::getUpdateTag()
{
    return writeToNBT(new NBTTagCompound());
}

void TileEntitySkull::setType(int32_t type)
{
    skullType     = type;
    playerProfile = std::nullopt;
}

void TileEntitySkull::setPlayerProfile(std::optional<GameProfile> playerProfile)
{
    skullType     = 3;
    playerProfile = playerProfile;
    updatePlayerProfile();
}

void TileEntitySkull::updatePlayerProfile()
{
    playerProfile = updateGameProfile(playerProfile);
    markDirty();
}

std::optional<GameProfile> TileEntitySkull::updateGameProfile(std::optional<GameProfile> input)
{
    if (input.has_value() && !StringUtils::isNullOrEmpty(input.getName()))
    {
        if (input.isComplete() && input.getProperties().containsKey("textures"))
        {
            return input;
        }
        else if (profileCache != nullptr && sessionService != nullptr)
        {
            GameProfile gameprofile = profileCache.getGameProfileForUsername(input.getName());
            if (gameprofile == nullptr)
            {
                return input;
            }
            else
            {
                Property property =
                    (Property)Iterables.getFirst(gameprofile.getProperties().get("textures"), (Object) nullptr);
                if (property == nullptr)
                {
                    gameprofile = sessionService.fillProfileProperties(gameprofile, true);
                }

                return gameprofile;
            }
        }
        else
        {
            return input;
        }
    }
    else
    {
        return input;
    }
}

int32_t TileEntitySkull::getSkullType() const
{
    return skullType;
}

int32_t TileEntitySkull::getSkullRotation() const
{
    return skullRotation;
}

void TileEntitySkull::setSkullRotation(int32_t rotation)
{
    skullRotation = rotation;
}

void TileEntitySkull::mirror(Mirror mirrorIn)
{
    if (world != nullptr && world->getBlockState(getPos()).getValue(BlockSkull::FACING) == EnumFacing::UP)
    {
        skullRotation = mirrorIn.mirrorRotation(skullRotation, 16);
    }
}

void TileEntitySkull::rotate(Rotation rotationIn)
{
    if (world != nullptr && world->getBlockState(getPos()).getValue(BlockSkull::FACING) == EnumFacing::UP)
    {
        skullRotation = rotationIn.rotate(skullRotation, 16);
    }
}
