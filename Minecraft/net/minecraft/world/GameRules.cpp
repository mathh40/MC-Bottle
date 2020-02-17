#include "GameRules.h"

Value::Value(std::string value, ValueType type)
	: type(type),setValue(value)
{

}

void Value::setValue(std::string value)
{
	valueString = value;
	valueBoolean = value == "true" || value == "1";
	valueInteger = valueBoolean ? 1 : 0;

	try 
	{
		valueInteger = std::stoi(value);
	}
	catch (std::invalid_argument &err) 
	{
	}

	try 
	{
		valueDouble = std::stod(value);
	}
	catch (std::invalid_argument & err) {
	}
}

std::string Value::getString() const
{
	return valueString;
}

bool Value::getBoolean() const
{
	return valueBoolean;
}

int32_t Value::getInt() const
{
	return valueInteger;
}

ValueType Value::getType() const
{
	return type;
}

GameRules::GameRules()
{
	addGameRule("doFireTick", "true", ValueType::BOOLEAN_VALUE);
	addGameRule("mobGriefing", "true", ValueType::BOOLEAN_VALUE);
	addGameRule("keepInventory", "false", ValueType::BOOLEAN_VALUE);
	addGameRule("doMobSpawning", "true", ValueType::BOOLEAN_VALUE);
	addGameRule("doMobLoot", "true", ValueType::BOOLEAN_VALUE);
	addGameRule("doTileDrops", "true", ValueType::BOOLEAN_VALUE);
	addGameRule("doEntityDrops", "true", ValueType::BOOLEAN_VALUE);
	addGameRule("commandBlockOutput", "true", ValueType::BOOLEAN_VALUE);
	addGameRule("naturalRegeneration", "true", ValueType::BOOLEAN_VALUE);
	addGameRule("doDaylightCycle", "true", ValueType::BOOLEAN_VALUE);
	addGameRule("logAdminCommands", "true", ValueType::BOOLEAN_VALUE);
	addGameRule("showDeathMessages", "true", ValueType::BOOLEAN_VALUE);
	addGameRule("randomTickSpeed", "3", ValueType::NUMERICAL_VALUE);
	addGameRule("sendCommandFeedback", "true", ValueType::BOOLEAN_VALUE);
	addGameRule("reducedDebugInfo", "false", ValueType::BOOLEAN_VALUE);
	addGameRule("spectatorsGenerateChunks", "true", ValueType::BOOLEAN_VALUE);
	addGameRule("spawnRadius", "10", ValueType::NUMERICAL_VALUE);
	addGameRule("disableElytraMovementCheck", "false", ValueType::BOOLEAN_VALUE);
	addGameRule("maxEntityCramming", "24", ValueType::NUMERICAL_VALUE);
	addGameRule("doWeatherCycle", "true", ValueType::BOOLEAN_VALUE);
	addGameRule("doLimitedCrafting", "false", ValueType::BOOLEAN_VALUE);
	addGameRule("maxCommandChainLength", "65536", ValueType::NUMERICAL_VALUE);
	addGameRule("announceAdvancements", "true", ValueType::BOOLEAN_VALUE);
	addGameRule("gameLoopFunction", "-", ValueType::FUNCTION);
}

void GameRules::addGameRule(std::string key, std::string value, ValueType type)
{
	rules.emplace(key, Value(value, type));
}

void GameRules::setOrCreateGameRule(std::string key, std::string ruleValue)
{
	auto value = rules.find(key);
	if (value != rules.end())
	{
		value->second.setValue(ruleValue);
	}
	else 
	{
		addGameRule(key, ruleValue, ValueType::ANY_VALUE);
	}
}

std::string GameRules::getString(std::string name)
{
	auto value = rules.find(name);
	return value != rules.end() ? value->second.getString() : "";
}

bool GameRules::getBoolean(std::string name)
{
	auto value = rules.find(name);
	return value != rules.end() ? value->second.getBoolean() : false;
}

int32_t GameRules::getInt(std::string name)
{
	auto value = rules.find(name);
	return value != rules.end() ? value->second.getInt() : 0;
}

std::unique_ptr<NBTTagCompound> GameRules::writeToNBT()
{
	auto nbttagcompound = std::unique_ptr<NBTTagCompound>();

	for(auto rule : rules)
	{
		Value value = rule.second;
		nbttagcompound->setString(rule.first, value.getString());
	}

	return nbttagcompound;
}

void GameRules::readFromNBT(NBTTagCompound* nbt)
{
	for (auto n : nbt->getCompoundMap())
	{
		auto s = n.first;
		setOrCreateGameRule(s, nbt->getString(s));
	}
}

std::vector<std::string> GameRules::getRules()
{
	std::vector<std::string> temp;

	temp.reserve(rules.size());
	for (auto rule : rules)
	{
		temp.emplace_back(rule.first);
	}
	return temp;
}

bool GameRules::hasRule(std::string name)
{
	return rules.find(name) != rules.end();
}

bool GameRules::areSameType(std::string key, ValueType otherValue)
{
	auto value = rules.find(key);
	return value != rules.end() && (value->second.getType() == otherValue || otherValue == ValueType::ANY_VALUE);
}