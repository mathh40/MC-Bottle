#include "CryptManager.h"

std::shared_ptr<spdlog::logger>  CryptManager::LOGGER = spdlog::get("Minecraft")->clone("CryptManager");

SecretKey CryptManager::createNewSharedKey() {
    SecretKey aesKey;
    SecretKey aesIV;

	if (RAND_bytes(aesKey, aesKey.data()) == 0) {
		throw std::runtime_error("NoSuchAlgorithmException");
	}

	if (RAND_bytes(aesIV, aesKey.data()) == 0) {
		throw std::runtime_error("NoSuchAlgorithmException");
	}

	return aesKey;
}

X509_PUBKEY  * CryptManager::generateKeyPair()
{
	X509_PUBKEY  *localKeypair;

	EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);


	if (EVP_PKEY_keygen_init(ctx) <= 0) {
		LOGGER->error("Key pair generation failed!");
	}

	if (EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, 1024) <= 0) {
		LOGGER->error("Key pair generation failed!");
	}

	if (EVP_PKEY_keygen(ctx, &amp; localKeypair) <= 0) {
		LOGGER->error("Key pair generation failed!");
	}

	EVP_PKEY_CTX_free(ctx);

	return localKeypair;

}

std::array<unsigned char, SHA_DIGEST_LENGTH> CryptManager::getServerIdHash(std::string serverId, X509_PUBKEY * publicKey, std::array<unsigned char, (128 / 8)> secretKey)
{
	auto salt = digestOperation("SHA-1", serverId, secretKey, publicKey);
	std::array<unsigned char ,SHA_DIGEST_LENGTH> obuf;
	SHA1(salt.data(), salt.size(), obuf.data());
	return obuf;
}

EVP_PKEY* CryptManager::decodePublicKey(std::vector<unsigned char> encodedKey)
{
	EVP_PKEY * key = EVP_PKEY_new();

	if(d2i_PUBKEY(&key, encodedKey, encodedKey.size()) != 0)
	{
		LOGGER->error("Public key reconstitute failed!");
	}
	
	return key;
}

SecretKey CryptManager::decryptSharedKey(EVP_PKEY *key, std::vector<unsigned char> secretKeyEncrypted) {
	return decryptDataRSA(key, secretKeyEncrypted);
}

std::vector<unsigned char> CryptManager::encryptDataRSA(EVP_PKEY * key, std::vector<unsigned char> data)
{
	return std::vector<unsigned char>();
}
