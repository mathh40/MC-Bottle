#include "TextComponentScore.h"

TextComponentScore::TextComponentScore(std::string nameIn, std::string objectiveIn)
    : name(nameIn), objective(objectiveIn)
{
}

std::string TextComponentScore::getName()
{
    return name;
}

std::string TextComponentScore::getObjective()
{
    return objective;
}

void TextComponentScore::setValue(std::string valueIn)
{
    value = valueIn;
}

std::string TextComponentScore::getUnformattedComponentText() const
{
    return value;
}

void TextComponentScore::resolve(ICommandSender sender)
{
    MinecraftServer minecraftserver = sender.getServer();
    if (minecraftserver.isAnvilFileSet() && StringUtils.isNullOrEmpty(value))
    {
        Scoreboard scoreboard         = minecraftserver.getWorld(0).getScoreboard();
        ScoreObjective scoreobjective = scoreboard.getObjective(objective);
        if (scoreboard.entityHasObjective(name, scoreobjective))
        {
            Score score = scoreboard.getOrCreateScore(name, scoreobjective);
            setValue(String.format("%d", score.getScorePoints()));
        }
        else
        {
            value = "";
        }
    }
}

std::string TextComponentScore::toString() const
{
    return "ScoreComponent{name='" + name + '\'' + "objective='" + objective + '\'' + ", siblings=" + siblings +
           ", style=" + getStyle() + '}';
}
