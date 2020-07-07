#include "CommandResultStatsType.h"

CommandResultStatsType::CommandResultStatsType(int32_t id, std::string_view name)
    :typeID(id),typeName(name)
{
    value.emplace_back(this);
}

int32_t CommandResultStatsType::getTypeID() const
{
    return typeID;
}

std::string CommandResultStatsType::getTypeName() const
{
    return typeName;
}

std::vector<std::string> CommandResultStatsType::getTypeNames()
{
    std::vector<std::string> astring(value.size());
    int32_t i = 0;

    for(auto commandresultstats$type : values())
    {
        astring[i++] = commandresultstats$type->getTypeName();
    }

    return astring;
}

std::vector<CommandResultStatsType *> CommandResultStatsType::values()
{
    return value;
}

CommandResultStatsType* CommandResultStatsType::getTypeByName(std::string_view name)
{
    for(auto commandresultstats$type : values())
    {
        if (commandresultstats$type->getTypeName() == name) 
        {
            return commandresultstats$type;
        }
    }

    return nullptr;
}
