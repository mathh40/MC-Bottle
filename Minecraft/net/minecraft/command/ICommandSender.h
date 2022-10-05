#pragma once
#include "math/BlockPos.h"
#include "math/Vec3d.h"

class ITextComponent;
class World;
class MinecraftServer;
class CommandResultStatsType;
class ICommandSender
{
  public:
    virtual std::string getName() const            = 0;
    virtual ITextComponent *getDisplayName() const = 0;

    virtual ~ICommandSender() = default;

    virtual void sendMessage(ITextComponent *component);

    virtual bool canUseCommand(int32_t var1, std::string_view var2) = 0;

    virtual BlockPos getPosition();

    virtual Vec3d getPositionVector();

    virtual World *getEntityWorld() = 0;

    virtual Entity *getCommandSenderEntity();

    virtual bool sendCommandFeedback();

    virtual void setCommandStat(const CommandResultStatsType &type, int32_t amount);

    virtual MinecraftServer *getServer() = 0;
};
