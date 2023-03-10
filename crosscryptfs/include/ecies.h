//  Copyright 2023 crosscryptfs Project Contributors
//  SPDX-License-Identifier: Apache-2.0
//

#ifndef ECIES_H
#define ECIES_H

#include <memory>
#include "encryption.h"

namespace crosscryptfs {

namespace ecies
{

// Ensures FIPS mode and required cyphers are present
int ecies_verify_platform();

void ecies_read_private_key(std::string privateKeyDERFilename, uint8_t** privateKeyOut, size_t* privateKeyLengthOut);

// TODO use c strings for file names
void ecies_generatepublickey(std::string privateKeyDERFilename, uint8_t** publicKeyOut, size_t* publicKeyLengthOut);

// TODO utility functions to validate curves and points for known issues (See security research on known problem point selection)

// Simplified call for known P256 curve (and thus known lengths)
void ecies_ka_ecdh_opensslv3(uint8_t* rxPubKeyIn, size_t rxPubKeyLengthIn, 
    uint8_t* txEphPrivKeyIn, size_t txEphPrivKeyLengthIn, 
    uint8_t** sharedSecretOut, size_t* sharedSecretLengthOut);

// These two functions are split out as the KDF only needs running once for a given pair of keys
void ecies_kdf_x963sha256_opensslv3( 
    uint8_t* sharedSecretIn, size_t sharedSecretLengthIn, 
    uint8_t** kEncOut16bytes, uint8_t** ivOut16bytes);

void ecies_encrypt_aes128gcm_opensslv3(uint8_t* plaintextIn, size_t plaintextLengthIn, 
    uint8_t* kEncIn16bytes, uint8_t* ivIn16bytes,
    uint8_t* additionalAuthDataIn, size_t additionalAuthDataLengthIn, 
    uint8_t** cryptogramWithTagOut, size_t* cryptogramWithTagLengthOut);

void ecies_decrypt_aes128gcm_opensslv3(uint8_t* ciphertextIn, size_t ciphertextLengthIn, 
    uint8_t* kEncIn16bytes, uint8_t* ivIn16bytes,
    uint8_t* additionalAuthDataIn, size_t additionalAuthDataLengthIn, 
    uint8_t** plaintextOut, size_t* plaintextLengthOut);

// // TODO simplified call for known P256 curve (and thus known lengths)
// // Uses the above two functions in order
// void ecies_encrypt_x963sha256aesgcm_opensslv3(uint8_t* txEphPubKeyIn, uint8_t txEphPubKeyLengthIn, 
//     uint8_t* sharedSecretIn, uint8_t sharedSecretLengthIn,
//     uint8_t* plaintextIn, uint8_t plaintextLengthIn, 
//     uint8_t* additionalAuthDataIn, uint8_t additionalAuthDataLengthIn, 
//     uint8_t** cryptogramOut, uint8_t* cryptogramLengthOut, 
//     uint8_t** tagOut, uint8_t* tagLengthOut);



// // Below are the end to end functions that are equivalent of each other on different platforms
// // The OpenSSL variety uses all of the above functions
// void ecies_encrypt_ecdhx963sha256aesgcm_opensslv3();

// // The apple variety hands off the entire sequence to the TPM chip (Apple T2 or similar)
// void ecies_encrypt_ecdhx963sha256aesgcm_appletpm();

// // A placeholder for an equivalent function on Windows
// void ecies_encrypt_ecdhx963sha256aesgcm_windowstpm();

// // A placeholder for an equivalent function on Linux
// void ecies_encrypt_ecdhx963sha256aesgcm_linuxtpm();

} // namespace ecies
    
/*!
 * \brief Provides ECIES Encryption using ECDH HMAC-SHA256 X9.62 using the prime256v1 curve and 128-bit AES GCM.
 */
class ECIESEncryptionProvider : public EncryptionProvider {
public:
    ECIESEncryptionProvider();
    ~ECIESEncryptionProvider();
    ECIESEncryptionProvider(const ECIESEncryptionProvider&) = delete;
    ECIESEncryptionProvider(ECIESEncryptionProvider&&) = delete;
    ECIESEncryptionProvider& operator=(const ECIESEncryptionProvider&) = delete;
    ECIESEncryptionProvider& operator=(ECIESEncryptionProvider&&) = delete;
    
    void setParameterValue(std::string name,std::string value) override;
    std::string getParameterValues() override;

    void setKeyMaterial(std::istream& keyDataIn) override;

    // Note that these methods assume the resulting encrypted text also includes any data (aside from settings) required for decryption
    void encryptContent(std::istream& plainIn,std::ostream& encryptedOut) override;
    void decryptContent(std::istream& encryptedIn,std::ostream& plainOut) override;

    // TODO any ECIES Specific methods below here
    
private:
    class Impl;
    std::unique_ptr<Impl> mImpl;
};

} // end namespace

#endif