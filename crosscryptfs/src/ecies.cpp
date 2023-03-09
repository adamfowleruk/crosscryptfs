//  Copyright 2023 crosscryptfs Project Contributors
//  SPDX-License-Identifier: Apache-2.0
//

#include "ecies.h"
#include <string>

#include <openssl/bn.h>
#include <openssl/ec.h>
#include <openssl/ecdh.h>
#include <openssl/pem.h>
#include <openssl/rand.h>
#include <openssl/evp.h>

#include <openssl/provider.h>

namespace crosscryptfs {

namespace ecies {

int
ecies_verify_platform()
{
    OSSL_PROVIDER *fips;
    OSSL_PROVIDER *base;

    fips = OSSL_PROVIDER_load(NULL, "fips");
    if (fips == NULL) {
        printf("Failed to load FIPS provider\n");
        return EXIT_FAILURE;
    }
    return 0;
}

void
ecies_generatepublickey(std::string privateKeyDERFilename, uint8_t** publicKeyOut, size_t* publicKeyLengthOut)
{

  EC_KEY *ec_key = NULL; // EC key from keyfile
  
  // RX
//   uint8_t *pubk      = NULL; // RX pub key
//   size_t   pubk_len  = 0;
  uint8_t *privk     = NULL; // TX priv key
  size_t   privk_len = 0;
  int      curve;

  // load ecies pub key
  const EC_GROUP *ec_group = NULL;
  BIO            *bio_key  = NULL;
  BIO            *bio_out  = NULL; /* stdout */

  bio_key = BIO_new_file(privateKeyDERFilename.c_str(), "r");
//   if (bio_key == NULL) {
//       std::cerr << "Failed to read EC key file '" << privateKeyFilename << std::endl;
//     //   return 1;
//   }
  ec_key = d2i_ECPrivateKey_bio(bio_key, NULL);
//   if (ec_key == NULL) {
//       std::cerr << "Failed to parse EC key file '" << privateKeyFilename << std::endl;
//     //   return 1;
//   }
  BIO_free(bio_key);

  // get curve params
  ec_group = EC_KEY_get0_group(ec_key);

  // Create bio wrapper
  bio_out = BIO_new_fp(stdout, BIO_NOCLOSE);

  // set point curve
  EC_KEY_set_conv_form(ec_key, POINT_CONVERSION_UNCOMPRESSED);

  // Get curve in binary array format
  ec_group   = EC_KEY_get0_group(ec_key);
  const EC_POINT *pub        = EC_KEY_get0_public_key(ec_key);
  BIGNUM         *pub_bn     = BN_new();
  BN_CTX         *pub_bn_ctx = BN_CTX_new();

  BN_CTX_start(pub_bn_ctx);

  EC_POINT_point2bn(ec_group, pub, POINT_CONVERSION_UNCOMPRESSED,
                    pub_bn, pub_bn_ctx);

  *publicKeyLengthOut = BN_num_bytes(pub_bn);
  *publicKeyOut = (std::uint8_t*)OPENSSL_malloc(*publicKeyLengthOut);

//   if (BN_bn2bin(pub_bn, pubk) != pubk_len) {
//       std::cerr << "Failed to decode pubkey" << std::endl;
//       return 1;
//   }
}

} // end ecies namespace






class ECIESEncryptionProvider::Impl {
public:
    Impl();
    ~Impl();

    // TODO state values here
    uint8_t skey[512];
    uint8_t skey_len;

};

ECIESEncryptionProvider::Impl::Impl()
{
    ;
}

ECIESEncryptionProvider::Impl::~Impl() {}

// Public ABI implementation class methods

ECIESEncryptionProvider::ECIESEncryptionProvider()
    : mImpl(std::make_unique<ECIESEncryptionProvider::Impl>())
{
    ;
}

ECIESEncryptionProvider::~ECIESEncryptionProvider() {}

void
ECIESEncryptionProvider::setParameterValue(std::string name,std::string value)
{
    // Set parameters based on config file settings
    // get the txprivkey which we're storing in PLAINTEXT for now
}

std::string
ECIESEncryptionProvider::getParameterValues()
{
    return std::string("");
}

void
ECIESEncryptionProvider::setKeyMaterial(std::istream& keyDataIn)
{
    // Copy the key material to a useful format now (as the stream is closed after this function returns)
}

void
ECIESEncryptionProvider::encryptContent(std::istream& plainIn,std::ostream& encryptedOut)
{        
    // EVP_CIPHER_CTX *ctx;
    // int len = 0;

    // /* Allocate buffers for the IV, tag, and ciphertext. */
    // int iv_len = 12;
    // uint8_t* iv = OPENSSL_malloc(iv_len);
    // int tag_len = 12;
    // uint8_t* tag = OPENSSL_malloc(tag_len);
    // // uint8_t* ciphertext = OPENSSL_malloc((256 + 0xf) & ~0xf);

    // /* Initialize the context and encryption operation. */
    // ctx = EVP_CIPHER_CTX_new();
    // EVP_EncryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, NULL, NULL);

    // /* Generate a new random IV. */
    // RAND_pseudo_bytes(iv, iv_len);
    // for (int i = 0;i < iv_len;++i) {
    //     encryptedOut << iv[i];
    // }

    // /* Prime the key and IV. */
    // EVP_EncryptInit_ex(ctx, NULL, NULL, skey, iv);

    // /* Prime with any additional authentication data. */
    // // if (aad && aad_len)
    // //     EVP_EncryptUpdate(ctx, NULL, &len, aad, aad_len);

    // /* Encrypt the data. */
    // // Stream input stream into cipher, and stream out to encrypted out
    // int ciphertext_len = 0;
    // std::uint8_t ciphertext[256];
    // std::uint8_t plaintext[256];
    // do {
    //     // read into plaintext buffer up to plaintext_len
    //     // if plaintext_len == 256
    //     EVP_EncryptUpdate(ctx, &ciphertext, &len, plaintext, plaintext_len);
    //     // else (< 256)

    //     for (int i = 0;i < len;++i) {
    //         encryptedOut << ciphertext[i];
    //     }
    //     ciphertext_len += len;
    // } while (len > 0);

    // /* Finalize the encryption session. */
    // EVP_EncryptFinal_ex(ctx, &ciphertext, &len);
    // for (int i = 0;i < len;++i) {
    //     encryptedOut << ciphertext[i];
    // }
    // ciphertext_len += len;

    // /* Get the authentication tag. */
    // EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, tag_len, tag);
    // for (int i = 0;i < tag_len;++i) {
    //     encryptedOut << tag[i];
    // }
    // ciphertext_len += tag_len;

    // EVP_CIPHER_CTX_free(ctx);
}

void
ECIESEncryptionProvider::decryptContent(std::istream& encryptedIn,std::ostream& plainOut)
{
    // TODO make this do decryption
}

// TODO any ECIES Specific methods below here


}