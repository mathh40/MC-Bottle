#include "ICommandSender.h"

#include "CommandResultStats.h"
#include "text/TextComponentString.h"

ITextComponent* ICommandSender::getDisplayName()
{
    return new TextComponentString(getName());
}

void ICommandSender::sendMessage(ITextComponent* component)
{
}

BlockPos ICommandSender::getPosition()
{
    return BlockPos::ORIGIN;
}

Vec3d ICommandSender::getPositionVector()
{
    return Vec3d::ZERO;
}

Entity* ICommandSender::getCommandSenderEntity() 
{
    return nullptr;
}

bool ICommandSender::sendCommandFeedback() 
{
    return false;
}

void ICommandSender::setCommandStat(CommandResultStats::Type type, int32_t amount) 
{
}