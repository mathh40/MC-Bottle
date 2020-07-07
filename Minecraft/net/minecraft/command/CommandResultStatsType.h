#pragma once
#include <string>
#include <vector>

class CommandResultStatsType
{
public:
    CommandResultStatsType(int32_t id, std::string_view name);
    int32_t getTypeID() const;
    std::string getTypeName() const;
    static std::vector<std::string> getTypeNames();
    static std::vector<CommandResultStatsType*> values();
    static CommandResultStatsType* getTypeByName(std::string_view name);
private:
    int32_t typeID;
    std::string typeName;
    static std::vector<CommandResultStatsType*> value;
};
