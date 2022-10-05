#include "Session.h"

Session::Session(std::string usernameIn, std::string playerIDIn, std::string tokenIn, const std::string &sessionTypeIn)
    : username(std::move(usernameIn)), playerID(std::move(playerIDIn)), token(std::move(tokenIn)),
      sessionType(Type::_from_string(sessionTypeIn.c_str()))
{
}

std::string Session::getSessionID() const
{
    return "token:" + token + ":" + playerID;
}

std::string Session::getPlayerID() const
{
    return playerID;
}

std::string Session::getUsername() const
{
    return username;
}

std::string Session::getToken() const
{
    return token;
}

GameProfile Session::getProfile()
{
    try
    {
        xg::Guid uuid(getPlayerID());
        return GameProfile(uuid, getUsername());
    }
    catch (IllegalArgumentException var2)
    {
        return GameProfile(xg::newGuid(), getUsername());
    }
}
