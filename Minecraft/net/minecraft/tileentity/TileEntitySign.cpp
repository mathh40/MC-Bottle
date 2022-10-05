#include "TileEntitySign.h"

NBTTagCompound *TileEntitySign::writeToNBT(NBTTagCompound *compound)
{
    TileEntity::writeToNBT(compound);

    for (int i = 0; i < 4; ++i)
    {
        auto s = ITextComponent::Serializer::componentToJson(signText[i]);
        compound->setString("Text" + (i + 1), s);
    }

    stats.writeStatsToNBT(compound);
    return compound;
}

void TileEntitySign::readFromNBT(NBTTagCompound *compound)
{
    isEditable = false;
    TileEntity::readFromNBT(compound);
    Command icommandsender;

    for (int i = 0; i < 4; ++i)
    {
        std::string s                 = compound.getString("Text" + (i + 1));
        ITextComponent itextcomponent = ITextComponent::Serializer::jsonToComponent(s);

        try
        {
            signText[i] = TextComponentUtils.processComponent(icommandsender, itextcomponent, nullptr);
        }
        catch (CommandException var7)
        {
            signText[i] = itextcomponent;
        }
    }

    stats.readStatsFromNBT(compound);
}

std::optional<SPacketUpdateTileEntity> TileEntitySign::getUpdatePacket()
{
    return SPacketUpdateTileEntity(pos, 9, getUpdateTag());
}

NBTTagCompound *TileEntitySign::getUpdateTag()
{
    return writeToNBT(NBTTagCompound());
}

bool TileEntitySign::onlyOpsCanSetNbt()
{
    return true;
}

bool TileEntitySign::getIsEditable() const
{
    return isEditable;
}

void TileEntitySign::setEditable(bool isEditableIn)
{
    isEditable = isEditableIn;
    if (!isEditableIn)
    {
        player = nullptr;
    }
}

void TileEntitySign::setPlayer(EntityPlayer *playerIn)
{
    player = playerIn;
}

EntityPlayer *TileEntitySign::getPlayer() const
{
    return player;
}

bool TileEntitySign::executeCommand(const EntityPlayer *playerIn)
{
    ICommandSender icommandsender = new ICommandSender(){public String getName(){return playerIn.getName();
}

public
ITextComponent getDisplayName()
{
    return playerIn.getDisplayName();
}

public
void sendMessage(ITextComponent component)
{
}

public
boolean canUseCommand(int permLevel, String commandName)
{
    return permLevel <= 2;
}

public
BlockPos getPosition()
{
    return TileEntitySign.this.pos;
}

public
Vec3d getPositionVector()
{
    return new Vec3d((double)TileEntitySign.this.pos.getX() + 0.5D, (double)TileEntitySign.this.pos.getY() + 0.5D,
                     (double)TileEntitySign.this.pos.getZ() + 0.5D);
}

public
World getEntityWorld()
{
    return playerIn.getEntityWorld();
}

public
Entity getCommandSenderEntity()
{
    return playerIn;
}

public
boolean sendCommandFeedback()
{
    return false;
}

public
void setCommandStat(CommandResultStats.Type type, int amount)
{
    if (TileEntitySign.this.world != null && !TileEntitySign.this.world.isRemote)
    {
        TileEntitySign.this.stats.setCommandStatForSender(TileEntitySign.this.world.getMinecraftServer(), this, type,
                                                          amount);
    }
}

public
MinecraftServer getServer()
{
    return playerIn.getServer();
}
}
;
ITextComponent[] var3 = this.signText;
int var4              = var3.length;

for (int var5 = 0; var5 < var4; ++var5)
{
    ITextComponent itextcomponent = var3[var5];
    Style style                   = itextcomponent == null ? null : itextcomponent.getStyle();
    if (style != null && style.getClickEvent() != null)
    {
        ClickEvent clickevent = style.getClickEvent();
        if (clickevent.getAction() == ClickEvent.Action.RUN_COMMAND)
        {
            playerIn.getServer().getCommandManager().executeCommand(icommandsender, clickevent.getValue());
        }
    }
}

return true;
}

void TileEntitySign::setWorldCreate(World *worldIn)
{
    setWorld(worldIn);
}

CommandResultStats TileEntitySign::getStats()
{
    return stats;
}
