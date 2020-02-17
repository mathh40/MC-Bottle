#pragma once
#include <string>
#include <vector>

class PlayerCapabilities;
class GameType
{
public:
	static GameType NOT_SET;
	static GameType SURVIVAL;
	static GameType CREATIVE;
	static GameType ADVENTURE;
	static GameType SPECTATOR;

	int32_t getID() const;
	std::string getName() const;

	void configurePlayerCapabilities(PlayerCapabilities& capabilities) const;
	friend auto operator==(const GameType& lhs, const GameType& rhs) -> bool;
	bool hasLimitedInteractions() const;
	bool isCreative() const;
	bool isSurvivalOrAdventure() const;
	static GameType getByID(int32_t idIn);
	static GameType parseGameTypeWithDefault(int32_t targetId, GameType fallback);
private:
	int32_t id;
	std::string name;
	std::string shortName;
	static std::vector< GameType*> value;

	GameType(int32_t idIn, std::string nameIn, std::string shortNameIn);
};
