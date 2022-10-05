#include "CryptManager.h"

#include "../cryptopp/aes.h"
#include "../cryptopp/osrng.h"

std::shared_ptr<spdlog::logger> CryptManager::LOGGER = spdlog::get("Minecraft")->clone("CryptManager");

CryptoPP::SecByteBlock CryptManager::createNewSharedKey()
{
    CryptoPP::AutoSeededRandomPool prng;
    CryptoPP::SecByteBlock key(CryptoPP::AES::DEFAULT_KEYLENGTH);
    CryptoPP::SecByteBlock iv(CryptoPP::AES::BLOCKSIZE);

    prng.GenerateBlock(key, key.size());
    prng.GenerateBlock(iv, iv.size());

    return key;
}

X509_PUBKEY *CryptManager::generateKeyPair()
{

    CryptoPP::OID oid = CryptoPP::ASN1::secp256r1();
    CryptoPP::AutoSeededRandomPool prng;

    CryptoPP::ECDH<CryptoPP::ECP>::Domain ecdh(oid);

    CryptoPP::SecByteBlock privKey(ecdh.PrivateKeyLength()), pubKey(ecdh.PublicKeyLength());
    ecdh.GenerateKeyPair(prng, privKey, pubKey);

    X509_PUBKEY *localKeypair;

    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);

    if (EVP_PKEY_keygen_init(ctx) <= 0)
    {
        LOGGER->error("Key pair generation failed!");
    }

    if (EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, 1024) <= 0)
    {
        LOGGER->error("Key pair generation failed!");
    }

    if (EVP_PKEY_keygen(ctx, &amp; localKeypair) <= 0)
    {
        LOGGER->error("Key pair generation failed!");
    }

    EVP_PKEY_CTX_free(ctx);

    return localKeypair;
}

std::array<unsigned char, SHA_DIGEST_LENGTH> CryptManager::getServerIdHash(
    std::string serverId, X509_PUBKEY *publicKey, std::array<unsigned char, (128 / 8)> secretKey)
{
    auto salt = digestOperation("SHA-1", serverId, secretKey, publicKey);
    std::array<unsigned char, SHA_DIGEST_LENGTH> obuf;
    SHA1(salt.data(), salt.size(), obuf.data());
    return obuf;
}

EVP_PKEY *CryptManager::decodePublicKey(std::vector<unsigned char> encodedKey)
{
    EVP_PKEY *key = EVP_PKEY_new();

    if (d2i_PUBKEY(&key, encodedKey, encodedKey.size()) != 0)
    {
        LOGGER->error("Public key reconstitute failed!");
    }

    return key;
}

SecretKey CryptManager::decryptSharedKey(EVP_PKEY *key, std::vector<unsigned char> secretKeyEncrypted)
{
    return decryptDataRSA(key, secretKeyEncrypted);
}

std::vector<unsigned char> CryptManager::encryptDataRSA(EVP_PKEY *key, std::vector<unsigned char> data)
{
    return std::vector<unsigned char>();
}
