#pragma once
#include <string>
#include "enum.h"
#include "../../../../authlib/com/mojang/authlib/GameProfile.h"

BETTER_ENUM(Type, uint8_t, LEGACY, MOJANG);

class Session
{
public:
	Session(std::string usernameIn, std::string playerIDIn, std::string tokenIn, const std::string& sessionTypeIn);

	std::string getSessionID() const;
	std::string getPlayerID() const;
	std::string getUsername() const;
	std::string getToken() const;
	GameProfile getProfile();

private:

	const std::string username;
	const std::string playerID;
	const std::string token;
	const Type sessionType;
};
