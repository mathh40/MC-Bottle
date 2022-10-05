#pragma once
#include "NBTTagCompound.h"

#include <unordered_map>

enum class ValueType
{
    ANY_VALUE,
    BOOLEAN_VALUE,
    NUMERICAL_VALUE,
    FUNCTION
};

class Value
{
  public:
    Value(std::string value, ValueType type);
    void setValue(std::string value);
    std::string getString() const;
    bool getBoolean() const;
    int32_t getInt() const;
    ValueType getType() const;

  private:
    std::string valueString;
    bool valueBoolean;
    int32_t valueInteger;
    double valueDouble;
    ValueType type;
};

class GameRules
{
  public:
    GameRules();
    void addGameRule(std::string key, std::string value, ValueType type);
    void setOrCreateGameRule(std::string key, std::string ruleValue);
    std::string getString(std::string name);
    bool getBoolean(std::string name);
    int32_t getInt(std::string name);
    std::unique_ptr<NBTTagCompound> writeToNBT();
    void readFromNBT(NBTTagCompound *nbt);
    std::vector<std::string> getRules();
    bool hasRule(std::string name);
    bool areSameType(std::string key, ValueType otherValue);

  private:
    std::unordered_map<std::string, Value> rules{};
};
