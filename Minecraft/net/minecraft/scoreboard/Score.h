#pragma once
#include <cstdint>
#include <string>

class Score
{
public:

	Score(Scoreboard scoreboard, ScoreObjective objective, std::string_view playerName);
	void increaseScore(int32_t amount);
	void decreaseScore(int32_t amount);
	void incrementScore();
	int32_t getScorePoints() const;
	void setScorePoints(int32_t points);
	ScoreObjective getObjective();
	std::string getPlayerName() const;
	Scoreboard getScoreScoreboard();
	bool isLocked() const;
	void setLocked(bool locked);
	
	friend bool operator>(const Score& a,const Score& b);
private:
	Scoreboard scoreboard;
	ScoreObjective objective;
	std::string scorePlayerName;
	int32_t scorePoints;
	bool locked;
	bool forceUpdate;
};
