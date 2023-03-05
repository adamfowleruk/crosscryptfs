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


// TEST_CASE("OpenSSL ECDH", "[ka][ecdh][ecc][openssl]" ) {
//     REQUIRE(1 == 1);
// }