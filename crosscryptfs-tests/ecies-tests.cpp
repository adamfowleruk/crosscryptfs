//  Copyright 2023 crosscryptfs Project Contributors
//  SPDX-License-Identifier: Apache-2.0
//

#include <catch2/catch_test_macros.hpp>
#include "crosscryptfs/crosscryptfs.h"

// TEST_CASE("OpenSSL FIPS module available", "[ecc][openssl][fips]") {
//     int result = crosscryptfs::ecies::ecies_verify_platform();

//     REQUIRE(0 == result);
// }

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