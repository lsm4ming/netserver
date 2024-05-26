//
// Created by 刘时明 on 2024/5/27.
//
#include <iostream>
#include <cryptopp/md5.h>
#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/hex.h>
#include <cryptopp/filters.h>
#include <string>

using namespace CryptoPP;

using p = CryptoPP::byte *;
using cp = const CryptoPP::byte *;
using String = std::string;

MD5 hash;

inline size_t min(size_t a, size_t b)
{
    return a > b ? b : a;
}

static String md5(const String &plain, bool lowerCase = false)
{
    String digest;
    StringSource give_me_a_name(plain, true, new HashFilter(hash, new HexEncoder(new StringSink(digest))));
    if (lowerCase)
    {
        transform(digest.begin(), digest.end(), digest.begin(), ::tolower);
    }
    return digest;
}

static String aesEncoder(const String &keyStr, const String &plain)
{
    byte key[AES::DEFAULT_KEYLENGTH];
    memcpy(key, keyStr.data(), min(keyStr.length(), AES::DEFAULT_KEYLENGTH));
    byte iv[AES::BLOCKSIZE] = {0};
    CBC_Mode<AES>::Encryption encryption(key, keyStr.length(), iv);
    String cipher, digest;
    StringSource give_me_a_name1(plain, true, new StreamTransformationFilter(encryption, new StringSink(cipher)));
    StringSource give_me_a_name2(cipher, true, new HexEncoder(new StringSink(digest)));
    return digest;
}

static String aesDecoder(const String &keyStr, const String &cipher)
{
    byte key[AES::DEFAULT_KEYLENGTH];
    memcpy(key, keyStr.data(), min(keyStr.length(), static_cast<size_t>(AES::DEFAULT_KEYLENGTH)));
    byte iv[AES::BLOCKSIZE] = {0};
    CBC_Mode<AES>::Decryption decryption(key, sizeof(key), iv);
    String decrypted, digest;
    StringSource give_me_a_name1(cipher, true, new HexDecoder(new StringSink(digest)));
    StringSource give_me_a_name2(digest, true, new StreamTransformationFilter(decryption, new StringSink(decrypted)));
    return decrypted;
}

int main()
{
    std::cout << md5("hello", true) << std::endl;
    std::cout << md5("hello") << std::endl;

    String key = "this_is_a_secret";
    String plain = "Hello, AES!";

    // 加密
    String encrypted = aesEncoder(key, plain);
    std::cout << "Encrypted text: " << encrypted << std::endl;

    // 解密
    String decrypted = aesDecoder(key, encrypted);
    std::cout << "Decrypted text: " << decrypted << std::endl;
    return 0;
}