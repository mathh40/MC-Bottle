#include "CommandResultStats.h"

#include "../scoreboard/ScoreObjective.h"
#include "../scoreboard/Scoreboard.h"
#include "ICommandSender.h"
#include "World.h"
#include "math/BlockPos.h"
#include "text/ITextComponent.h"

class CommandSender : public ICommandSender
{
  public:
    CommandSender(ICommandSender *senderIn);

    std::string getName() override
    {
        return sender->getName();
    }

    ITextComponent *getDisplayName() override
    {
        return sender->getDisplayName();
    }

    void sendMessage(ITextComponent *component) override
    {
        sender->sendMessage(component);
    }

    bool canUseCommand(int32_t var1, std::string_view var2) override
    {
        return true;
    }

    BlockPos getPosition() override
    {
        return sender->getPosition();
    }

    Vec3d getPositionVector() override
    {
        return sender->getPositionVector();
    }

    World *getEntityWorld() override
    {
        return sender->getEntityWorld();
    }

    Entity *getCommandSenderEntity() override
    {
        return sender->getCommandSenderEntity();
    }

    bool sendCommandFeedback() override
    {
        return sender->sendCommandFeedback();
    }

    void setCommandStat(const CommandResultStatsType &type, int32_t amount) override
    {
        sender->setCommandStat(type, amount);
    }

    MinecraftServer *getServer() override
    {
        return sender->getServer();
    }

  private:
    ICommandSender *sender;
};

CommandSender::CommandSender(ICommandSender *senderIn) : sender(senderIn)
{
}

CommandResultStats::CommandResultStats()
{
    entitiesID = STRING_RESULT_TYPES;
    objectives = STRING_RESULT_TYPES;
}

void CommandResultStats::setCommandStatForSender(MinecraftServer *server, ICommandSender *sender,
                                                 const CommandResultStatsType &typeIn, int32_t p_184932_4_)
{
    std::string s = entitiesID[typeIn.getTypeID()];
    if (!s.empty())
    {
        CommandSender icommandsender(sender);
        std::string s1;
        try
        {
            s1 = CommandBase::getEntityName(server, icommandsender, s);
        }
        catch (std::exception &var12)
        {
            return;
        }

        std::string s2 = objectives[typeIn.getTypeID()];
        if (!s2.empty())
        {
            Scoreboard scoreboard         = sender->getEntityWorld().getScoreboard();
            ScoreObjective scoreobjective = scoreboard.getObjective(s2);
            if (scoreobjective != nullptr && scoreboard.entityHasObjective(s1, scoreobjective))
            {
                Score score = scoreboard.getOrCreateScore(s1, scoreobjective);
                score.setScorePoints(p_184932_4_);
            }
        }
    }
}

void CommandResultStats::readStatsFromNBT(NBTTagCompound *tagcompound)
{
    if (tagcompound->hasKey("CommandStats", 10))
    {
        NBTTagCompound *nbttagcompound = tagcompound->getCompoundTag("CommandStats");
        auto var3                      = CommandResultStats::Type::values();
        auto var4                      = var3.size();

        for (auto var5 = 0; var5 < var4; ++var5)
        {
            CommandResultStats::Type commandresultstats$type = var3[var5];
            std::string s                                    = commandresultstats$type.getTypeName() + "Name";
            std::string s1                                   = commandresultstats$type.getTypeName() + "Objective";
            if (nbttagcompound->hasKey(s, 8) && nbttagcompound->hasKey(s1, 8))
            {
                std::string s2 = nbttagcompound->getString(s);
                std::string s3 = nbttagcompound->getString(s1);
                setScoreBoardStat(this, commandresultstats$type, s2, s3);
            }
        }
    }
}

void CommandResultStats::removeScoreBoardStat(CommandResultStats resultStatsIn,
                                              const CommandResultStatsType &resultTypeIn)
{
    if (resultStatsIn.entitiesID != STRING_RESULT_TYPES && resultStatsIn.objectives != STRING_RESULT_TYPES)
    {
        resultStatsIn.entitiesID[resultTypeIn.getTypeID()] = nullptr;
        resultStatsIn.objectives[resultTypeIn.getTypeID()] = nullptr;
        bool flag                                          = true;
        CommandResultStats::Type[] var3                    = CommandResultStats.Type.values();
        int var4                                           = var3.length;

        for (int var5 = 0; var5 < var4; ++var5)
        {
            CommandResultStats::Type commandresultstats$type = var3[var5];
            if (resultStatsIn.entitiesID[commandresultstats$type.getTypeID()] != nullptr &&
                resultStatsIn.objectives[commandresultstats$type.getTypeID()] != nullptr)
            {
                flag = false;
                break;
            }
        }

        if (flag)
        {
            resultStatsIn.entitiesID = STRING_RESULT_TYPES;
            resultStatsIn.objectives = STRING_RESULT_TYPES;
        }
    }
}

void CommandResultStats::writeStatsToNBT(NBTTagCompound *tagcompound)
{
    NBTTagCompound *nbttagcompound  = new NBTTagCompound();
    CommandResultStats::Type[] var3 = CommandResultStats::Type::values();
    int var4                        = var3.size();

    for (auto var5 = 0; var5 < var4; ++var5)
    {
        CommandResultStats::Type commandresultstats$type = var3[var5];
        std::string s                                    = entitiesID[commandresultstats$type.getTypeID()];
        std::string s1                                   = objectives[commandresultstats$type.getTypeID()];
        if (!s.empty() && !s1.empty())
        {
            nbttagcompound->setString(commandresultstats$type.getTypeName() + "Name", s);
            nbttagcompound->setString(commandresultstats$type.getTypeName() + "Objective", s1);
        }
    }

    if (!nbttagcompound->isEmpty())
    {
        tagcompound->setTag("CommandStats", nbttagcompound);
    }
}

void CommandResultStats::setScoreBoardStat(CommandResultStats stats, const CommandResultStatsType &resultType,
                                           std::optional<std::string_view> entityID,
                                           std::optional<std::string_view> objectiveName)
{
    if (entityID.has_value() && !entityID->empty() && objectiveName.has_value() && !objectiveName->empty())
    {
        if (stats.entitiesID == STRING_RESULT_TYPES || stats.objectives == STRING_RESULT_TYPES)
        {
            stats.entitiesID.resize(NUM_RESULT_TYPES);
            stats.objectives.resize(NUM_RESULT_TYPES);
        }

        stats.entitiesID[resultType.getTypeID()] = entityID;
        stats.objectives[resultType.getTypeID()] = objectiveName;
    }
    else
    {
        removeScoreBoardStat(stats, resultType);
    }
}

void CommandResultStats::addAllStats(CommandResultStats resultStatsIn)
{
    CommandResultStats::Type[] var2 = CommandResultStats::Type::values();
    int var3                        = var2.length;

    for (int var4 = 0; var4 < var3; ++var4)
    {
        CommandResultStats::Type commandresultstats$type = var2[var4];
        setScoreBoardStat(this, commandresultstats$type, resultStatsIn.entitiesID[commandresultstats$type.getTypeID()],
                          resultStatsIn.objectives[commandresultstats$type.getTypeID()]);
    }
}
