//  Copyright 2023 crosscryptfs Project Contributors
//  SPDX-License-Identifier: Apache-2.0
//

#include <catch2/catch_test_macros.hpp>
#include "crosscryptfs/crosscryptfs.h"

// TEST_CASE("OpenSSL FIPS module available", "[ecc][openssl][fips]") {
//     int result = crosscryptfs::ecies::ecies_verify_platform();

//     REQUIRE(0 == result);
// }


void
string_to_binary(const char* buf, uint8_t** binaryOut, size_t* bufferLengthOut)
{
    *bufferLengthOut = strlen(buf);
    int i;
    *binaryOut = new uint8_t[*bufferLengthOut];

    for(i=0;i<strlen(buf);++i) {
        (*binaryOut)[i] = (uint8_t)buf[i];
    }
}

void
binary_to_string(const uint8_t* buf, const size_t bufferLength, char** charOut, size_t* bufferLengthOut)
{
    *bufferLengthOut = bufferLength;
    int i;
    *charOut = new char[*bufferLengthOut];

    for(i=0;i<bufferLength;++i) {
        (*charOut)[i] = (char)buf[i];
    }
}

void
hex_to_binary(const char* buf, uint8_t** binaryOut, size_t* bufferLengthOut)
{
    *bufferLengthOut = strlen(buf) / 2;
    int i;
    char tmp[3];
    tmp[2] = '\0';
    *binaryOut = new uint8_t[*bufferLengthOut];
    uint8_t len_buffer=0;


    for(i=0;i<strlen(buf);i+=2) {
        tmp[0] = buf[i];
        tmp[1] = buf[i+1];
        (*binaryOut)[len_buffer] = strtol(tmp,NULL,16);
        len_buffer++;
    }
}

TEST_CASE("OpenSSL Generate Save Load Public Key", "[file][ecc][openssl]" ) {
    std::string privateKeyDERFilename("ecc_tx_private_key.der");
    uint8_t* publicKey = NULL;
    size_t publicKeyLength = 0;

    crosscryptfs::ecies::ecies_generatepublickey(privateKeyDERFilename,&publicKey,&publicKeyLength);

    REQUIRE(publicKeyLength == 65); // p256 curve uncompressed public key is 65 bytes
    REQUIRE(publicKey[0] != 0);

    // Now save the public key in the context folder
    std::string ctx("encrypted"); // subfolder of tests folder
    std::string module("ecies");
    std::string parameter("txpublickey");
    crosscryptfs::write_context_stream(ctx,module,parameter,publicKey,publicKeyLength);

    uint8_t* publicKeyReloaded = NULL;
    size_t publicKeyReloadedLength = 0;

    crosscryptfs::read_context_stream(ctx,module,parameter,&publicKeyReloaded,&publicKeyReloadedLength);

    REQUIRE(publicKeyReloadedLength == 65);
    REQUIRE(publicKeyReloaded[0] != 0);

    size_t matching = 0;
    for (size_t i = 0;i < 65;++i) {
        if (publicKey[i] == publicKeyReloaded[i]) {
            ++matching;
        }
    }
    REQUIRE(matching == 65);
}


// Key agreement occurs when the ECIESEncryptionProvider class is instantiated
TEST_CASE("OpenSSL ECDH", "[ka][ecdh][ecc][openssl]" ) {
    // Read in rx public key
    std::string rxPrivateKeyDERFilename("ecc_rx_private_key.der");
    uint8_t* rxPublicKey = NULL;
    size_t rxPublicKeyLength = 0;
    crosscryptfs::ecies::ecies_generatepublickey(rxPrivateKeyDERFilename,&rxPublicKey,&rxPublicKeyLength);
    REQUIRE(rxPublicKeyLength == 65); // p256 curve uncompressed public key is 65 bytes
    REQUIRE(rxPublicKey[0] != 0);

    // Read in tx private key
    std::string txPrivateKeyDERFilename("ecc_tx_private_key.der");
    uint8_t* txPrivateKey = NULL;
    size_t txPrivateKeyLength = 0;
    crosscryptfs::ecies::ecies_read_private_key(txPrivateKeyDERFilename,&txPrivateKey,&txPrivateKeyLength);
    REQUIRE(txPrivateKeyLength == 121); // p256 curve uncompressed private key is 121 bytes
    REQUIRE(txPrivateKey[0] != 0);

    // Perform ECDH
    uint8_t* sharedSecret = NULL;
    size_t sharedSecretLength = 0;
    crosscryptfs::ecies::ecies_ka_ecdh_opensslv3(rxPublicKey,rxPublicKeyLength,txPrivateKey,txPrivateKeyLength,&sharedSecret,&sharedSecretLength);

    REQUIRE(sharedSecretLength == 32); // Should be 32 bytes (256 bits)
    REQUIRE(sharedSecret[0] != 0);
}

TEST_CASE("OpenSSL EC KDF","[kdf][x963][ecies][ecc][openssl]") {
    // Read in rx public key
    std::string rxPrivateKeyDERFilename("ecc_rx_private_key.der");
    uint8_t* rxPublicKey = NULL;
    size_t rxPublicKeyLength = 0;
    crosscryptfs::ecies::ecies_generatepublickey(rxPrivateKeyDERFilename,&rxPublicKey,&rxPublicKeyLength);
    REQUIRE(rxPublicKeyLength == 65); // p256 curve uncompressed public key is 65 bytes
    REQUIRE(rxPublicKey[0] != 0);

    // Read in tx private key
    std::string txPrivateKeyDERFilename("ecc_tx_private_key.der");
    uint8_t* txPrivateKey = NULL;
    size_t txPrivateKeyLength = 0;
    crosscryptfs::ecies::ecies_read_private_key(txPrivateKeyDERFilename,&txPrivateKey,&txPrivateKeyLength);
    REQUIRE(txPrivateKeyLength == 121); // p256 curve uncompressed private key is 121 bytes
    REQUIRE(txPrivateKey[0] != 0);

    // Perform ECDH
    uint8_t* sharedSecret = NULL;
    size_t sharedSecretLength = 0;
    crosscryptfs::ecies::ecies_ka_ecdh_opensslv3(rxPublicKey,rxPublicKeyLength,txPrivateKey,txPrivateKeyLength,&sharedSecret,&sharedSecretLength);

    REQUIRE(sharedSecretLength == 32); // Should be 32 bytes (256 bits)
    REQUIRE(sharedSecret[0] != 0);


    // NEW TEST PARTS
    // Generate KDF parts
    uint8_t* keyOut;
    uint8_t* ivOut;
    crosscryptfs::ecies::ecies_kdf_x963sha256_opensslv3(sharedSecret, sharedSecretLength, &keyOut, &ivOut);

    REQUIRE(keyOut[0] != 0);
    REQUIRE(ivOut[0] != 0);
}

TEST_CASE("OpenSSL AES128GCM","[encryption][decryption[ecies][aes][ad][gcm][openssl]") {
    // Use known symmetric key for encryption and decryption of a known message with known outcome
    std::string plaintext("A very long message. Lorem dolar sit amet consecutor");
    std::string secretKeyHex("00000000000000000000000000000000");//("2b7e151628aed2a6abf715892b7e1516");
    std::string expectedCiphertextHex("721df76da28ba73a6b64124da583944a1452989c495f86300343441d176b8f46b740e730ccd60ce56c73e67e7de3eed2ea0a57d4fdba2234fe8b1a64b2191045805a9bba");
        //"3fb75db3326aa18163ada8c97a3bf8d1b4a4ae41b3fbc577c06b4ecfe908d594d3e19b5c3706655e42bdcb32596680a097b35f89b7f1fa7b949b8f0b08eae7c84f9f1c99");
        // text: P7ddszJqoYFjrajJejv40bSkrkGz+8V3wGtOz+kI1ZTT4ZtcNwZlXkK9yzJZZoCgl7Nfibfx+nuU
        // tag: m48LCOrnyE+fHJk=

    // ch33baKLpzprZBJNpYOUShRSmJxJX4YwA0NEHRdrj0a3QOcwzNYM5Wxz5n594+7S6gpX1P26IjT+
    // ixpkshkQRYBam7o=

    uint8_t* secretKey = NULL;
    size_t secretKeyLength = 0;
    hex_to_binary(secretKeyHex.c_str(),&secretKey,&secretKeyLength);
    REQUIRE(NULL != secretKey);
    REQUIRE(16 == secretKeyLength);

    uint8_t* expectedCiphertext = NULL;
    size_t expectedCiphertextLength = 0;
    hex_to_binary(expectedCiphertextHex.c_str(),&expectedCiphertext,&expectedCiphertextLength);
    REQUIRE(NULL != expectedCiphertext);
    REQUIRE(68 == expectedCiphertextLength);

    uint8_t* plaintextBinary = NULL;
    size_t plaintextBinaryLength = 0;
    string_to_binary(plaintext.c_str(),&plaintextBinary,&plaintextBinaryLength);
    REQUIRE(NULL != plaintextBinary);
    REQUIRE(52 == plaintextBinaryLength);

    char* plaintextReencodded = NULL;
    size_t plaintextReencoddedLength = 0;
    binary_to_string(plaintextBinary,52,&plaintextReencodded,&plaintextReencoddedLength);
    REQUIRE(NULL != plaintextReencodded);
    REQUIRE(52 == plaintextReencoddedLength);
    size_t matches = 0;
    for (size_t i = 0;i < 52;++i) {
        std::cout << plaintextReencodded[i];
        if (plaintextReencodded[i] == plaintext[i]) {
            ++matches;
        }
    }
    std::cout << std::endl;
    REQUIRE(52 == matches);
    
    // Now perform our encryption
    uint8_t* ciphertext = NULL;
    size_t ciphertextLength = 0;
    uint8_t emptyIv[16];
    for (int i = 0;i < 16;++i) {
        emptyIv[i] = 0;
    }
    crosscryptfs::ecies::ecies_encrypt_aes128gcm_opensslv3(plaintextBinary,plaintextBinaryLength,
        secretKey,emptyIv,NULL,0,&ciphertext,&ciphertextLength);

    // Test ciphertext output
    REQUIRE(NULL != ciphertext);
    REQUIRE(68 == ciphertextLength); // plaintext length plus the 16 byte tag
    // Test each element in its contents
    matches = 0;
    for (size_t i = 0;i < 68;++i) {
        if (expectedCiphertext[i] == ciphertext[i]) {
            ++matches;
        }
    }
    // REQUIRE(68 == matches);

    // Now try decrypting and check it matches the original
    uint8_t* decryptedText = NULL;
    size_t decryptedTextLength = 0;
    crosscryptfs::ecies::ecies_decrypt_aes128gcm_opensslv3(ciphertext,ciphertextLength,
        secretKey,emptyIv,NULL,0,&decryptedText,&decryptedTextLength);
    REQUIRE(NULL != decryptedText);
    REQUIRE(52 == decryptedTextLength);
    matches = 0;
    for (size_t i = 0;i < 52;++i) {
        if (plaintextBinary[i] == decryptedText[i]) {
            ++matches;
        }
    }
    // REQUIRE(52 == matches);

    char* decryptedTextString = NULL;
    size_t decryptedTextStringLength = 0;
    binary_to_string(decryptedText,52,&decryptedTextString,&decryptedTextStringLength);
    REQUIRE(NULL != decryptedTextString);
    REQUIRE(52 == decryptedTextStringLength);
    matches = 0;
    for (size_t i = 0;i < 52;++i) {
        std::cout << decryptedTextString[i];
        if (decryptedTextString[i] == plaintext[i]) {
            ++matches;
        }
    }
    std::cout << std::endl;
    REQUIRE(52 == matches);
}