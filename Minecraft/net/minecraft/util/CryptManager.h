#pragma once
#include <vector>
#include <memory>
#include "..//spdlog/include/spdlog/spdlog.h"
#include "../cryptopp/rijndael.h"

class CryptManager
{
public:
    CryptoPP::SecByteBlock createNewSharedKey();
	EVP_PKEY * generateKeyPair();

	std::array<unsigned char, SHA_DIGEST_LENGTH> getServerIdHash(std::string serverId, EVP_PKEY* publicKey, std::array<unsigned char, (128 / 8)> secretKey);
	template<typename T, class ...Args>
	std::vector<unsigned char> digestOperation(std::string algorithm, T data , Args ... datas);
	template<typename T>
	std::vector<unsigned char> digestOperation(std::string algorithm, T data);

	EVP_PKEY* decodePublicKey(std::vector<unsigned char> encodedKey);
    CryptoPP::SecByteBlock decryptSharedKey(EVP_PKEY *key, std::vector<unsigned char> secretKeyEncrypted);
	std::vector<unsigned char> encryptDataRSA(EVP_PKEY * key, std::vector<unsigned char> data);
	std::vector<unsigned char> encryptDataAES(std::array<unsigned char, (128 / 8)> key, std::vector<unsigned char> data);
	std::vector<unsigned char> decryptDataRSA(EVP_PKEY * key, std::vector<unsigned char> data);
	std::vector<unsigned char> decryptDataAES(std::array<unsigned char, (128 / 8)> key, std::vector<unsigned char> data);

private:
	static std::shared_ptr<spdlog::logger> LOGGER;
};

template<typename T, class ...Args>
inline std::vector<unsigned char> CryptManager::digestOperation(std::string algorithm, T data, Args ...datas)
{
	std::vector<unsigned char> a(data.begin(), data.end());
	std::vector<unsigned char> b = digestOperation(algorithm, datas...);
	a.insert(a.begin(), b.begin(), b.end());
	return a;
}

template<typename T>
inline std::vector<unsigned char> CryptManager::digestOperation(std::string algorithm, T data)
{
	return std::vector<unsigned char>(data.begin(), data.end());
}
