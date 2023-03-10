//  Copyright 2023 crosscryptfs Project Contributors
//  SPDX-License-Identifier: Apache-2.0
//

#include "ecies.h"
#include <string>

// #include <openssl/bn.h>
// #include <openssl/ec.h>
// #include <openssl/ecdh.h>
// #include <openssl/pem.h>
// #include <openssl/rand.h>
// #include <openssl/evp.h>

// #include <openssl/provider.h>

#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/pem.h>

namespace crosscryptfs {

namespace ecies {

int
ecies_verify_platform()
{
    OSSL_PROVIDER *fips;
    OSSL_PROVIDER *base;

    // fips = OSSL_PROVIDER_load(NULL, "fips");
    // if (fips == NULL) {
    //     printf("Failed to load FIPS provider\n");
    //     return EXIT_FAILURE;
    // }
    return 0;
}

void
ecies_generatepublickey(std::string privateKeyDERFilename, uint8_t** publicKeyOut, size_t* publicKeyLengthOut)
{
    // Old OpenSSL v1.1.0 way of doing things:-
    // EC_KEY *ec_key = NULL; // EC key from keyfile
    
    // // RX
    // //   uint8_t *pubk      = NULL; // RX pub key
    // //   size_t   pubk_len  = 0;
    // uint8_t *privk     = NULL; // TX priv key
    // size_t   privk_len = 0;
    // int      curve;

    // // load ecies pub key
    // const EC_GROUP *ec_group = NULL;
    // BIO            *bio_key  = NULL;
    // BIO            *bio_out  = NULL; /* stdout */

    // bio_key = BIO_new_file(privateKeyDERFilename.c_str(), "r");
    // //   if (bio_key == NULL) {
    // //       std::cerr << "Failed to read EC key file '" << privateKeyFilename << std::endl;
    // //     //   return 1;
    // //   }
    // ec_key = d2i_ECPrivateKey_bio(bio_key, NULL);
    // //   if (ec_key == NULL) {
    // //       std::cerr << "Failed to parse EC key file '" << privateKeyFilename << std::endl;
    // //     //   return 1;
    // //   }
    // BIO_free(bio_key);

    // // get curve params
    // ec_group = EC_KEY_get0_group(ec_key);

    // // Create bio wrapper
    // bio_out = BIO_new_fp(stdout, BIO_NOCLOSE);

    // // set point curve
    // EC_KEY_set_conv_form(ec_key, POINT_CONVERSION_UNCOMPRESSED);

    // // Get curve in binary array format
    // ec_group   = EC_KEY_get0_group(ec_key);
    // const EC_POINT *pub        = EC_KEY_get0_public_key(ec_key);
    // BIGNUM         *pub_bn     = BN_new();
    // BN_CTX         *pub_bn_ctx = BN_CTX_new();

    // BN_CTX_start(pub_bn_ctx);

    // EC_POINT_point2bn(ec_group, pub, POINT_CONVERSION_UNCOMPRESSED,
    //                     pub_bn, pub_bn_ctx);

    // *publicKeyLengthOut = BN_num_bytes(pub_bn);
    // *publicKeyOut = (std::uint8_t*)OPENSSL_malloc(*publicKeyLengthOut);

//   if (BN_bn2bin(pub_bn, pubk) != pubk_len) {
//       std::cerr << "Failed to decode pubkey" << std::endl;
//       return 1;
//   }


    // New OpenSSL V3 way of doing things
    // Load DER file
    BIO *bio_key  = NULL;
    bio_key = BIO_new_file(privateKeyDERFilename.c_str(), "r");
    //   if (bio_key == NULL) {
    //       std::cerr << "Failed to read EC key file '" << privateKeyFilename << std::endl;
    //     //   return 1;
    //   }
    EVP_PKEY* pkey;
    pkey = d2i_PrivateKey_bio(bio_key, NULL);
    //   if (ec_key == NULL) {
    //       std::cerr << "Failed to parse EC key file '" << privateKeyFilename << std::endl;
    //     //   return 1;
    //   }
    BIO_free(bio_key);

    // NB below is how to generate a new set of params and a new private key
    /* Create the context for generating the parameters */
    // EVP_PKEY_CTX* pctx;
    // if(!(pctx = EVP_PKEY_CTX_new_id(EVP_PKEY_EC, NULL))) return;
    // if(!EVP_PKEY_paramgen_init(pctx)) return;

    // /* Set the paramgen parameters according to the type */
    // /* Use the NID_X9_62_prime256v1 named curve - defined in obj_mac.h */
    // if(!EVP_PKEY_CTX_set_ec_paramgen_curve_nid(pctx, NID_X9_62_prime256v1)) return;	

    // /* Generate parameters */
    // if (!EVP_PKEY_paramgen(pctx, NULL)) return;

    // /* Generate the key */
    // EVP_PKEY* pubkey;
    // if (!EVP_PKEY_keygen(pctx, &pubkey)) return;

    *publicKeyLengthOut = i2d_PublicKey(pkey,publicKeyOut);

	EVP_PKEY_free(pkey);

}



} // end ecies namespace






class ECIESEncryptionProvider::Impl {
public:
    Impl();
    ~Impl();
    Impl(const Impl&) = delete;
    Impl(Impl&&) = delete;

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
    // Also see https://wiki.openssl.org/index.php/EVP_Authenticated_Encryption_and_Decryption#Authenticated_Encryption_using_GCM_mode

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