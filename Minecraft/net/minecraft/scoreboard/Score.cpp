#include "Score.h"

#include <stdexcept>

bool operator>(const Score &a, const Score &b)
{
    return a.getScorePoints() > b.getScorePoints();
}

Score::Score(Scoreboard scoreboard, ScoreObjective objective, std::string_view playerName)
    : scoreboard(scoreboard), objective(objective), scorePlayerName(playerName), forceUpdate(true)
{
}

void Score::increaseScore(int32_t amount)
{
    if (objective.getCriteria().isReadOnly())
    {
        throw std::logic_error("Cannot modify read-only score");
    }
    else
    {
        setScorePoints(getScorePoints() + amount);
    }
}

void Score::decreaseScore(int32_t amount)
{
    if (objective.getCriteria().isReadOnly())
    {
        throw std::logic_error("Cannot modify read-only score");
    }
    else
    {
        setScorePoints(getScorePoints() - amount);
    }
}

void Score::incrementScore()
{
    if (objective.getCriteria().isReadOnly())
    {
        throw std::logic_error("Cannot modify read-only score");
    }
    else
    {
        increaseScore(1);
    }
}

int32_t Score::getScorePoints() const
{
    return scorePoints;
}

void Score::setScorePoints(int32_t points)
{
    int32_t i   = scorePoints;
    scorePoints = points;
    if (i != points || forceUpdate)
    {
        forceUpdate = false;
        getScoreScoreboard().onScoreUpdated(this);
    }
}

ScoreObjective Score::getObjective()
{
    return objective;
}

std::string Score::getPlayerName() const
{
    return scorePlayerName;
}

Scoreboard Score::getScoreScoreboard()
{
    return scoreboard;
}

bool Score::isLocked() const
{
    return locked;
}

void Score::setLocked(bool locked)
{
    locked = locked;
}
