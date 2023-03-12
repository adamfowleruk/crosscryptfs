//  Copyright 2023 crosscryptfs Project Contributors
//  SPDX-License-Identifier: Apache-2.0
//

#include "ecies.h"
#include <string>
#include <iostream>

#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/ec.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/ossl_typ.h>
#include <openssl/kdf.h>
#include <openssl/core_names.h>

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

EVP_PKEY*
ecies_read_private_key_file(std::string privateKeyDERFilename)
{
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

    return pkey;
}

void
ecies_read_private_key(std::string privateKeyDERFilename, uint8_t** privateKeyOut, size_t* privateKeyLengthOut)
{
    EVP_PKEY* pkey = ecies_read_private_key_file(privateKeyDERFilename);

    *privateKeyLengthOut = i2d_PrivateKey(pkey,privateKeyOut);

	EVP_PKEY_free(pkey);
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
    EVP_PKEY* pkey = ecies_read_private_key_file(privateKeyDERFilename);

    *publicKeyLengthOut = i2d_PublicKey(pkey,publicKeyOut);

	EVP_PKEY_free(pkey);

}

void
ecies_generate_private_key(EVP_PKEY** privateKeyOut)
{
    // NB below is how to generate a new set of params and a new private key
    /* Create the context for generating the parameters */
    EVP_PKEY_CTX* pctx;
    if(!(pctx = EVP_PKEY_CTX_new_id(EVP_PKEY_EC, NULL))) {
        std::cerr << "Key param context creation failed" << std::endl;
        return;
    };
    if(!EVP_PKEY_paramgen_init(pctx)) {
        std::cerr << "Key paramgen creation failed" << std::endl;
        return;
    };

    /* Set the paramgen parameters according to the type */
    /* Use the NID_X9_62_prime256v1 named curve - defined in obj_mac.h */
    if(!EVP_PKEY_CTX_set_ec_paramgen_curve_nid(pctx, NID_X9_62_prime256v1)) {
        std::cerr << "Key param context set curve failed" << std::endl;
        return;
    };

    /* Generate parameters */
    EVP_PKEY* params = NULL;
    if (!EVP_PKEY_paramgen(pctx, &params)) {
        std::cerr << "Key param generation failed" << std::endl;
        return;
    };

    /* Generate the key */
    EVP_PKEY_CTX* kctx = NULL;
    if(!(kctx = EVP_PKEY_CTX_new(params, NULL))) {
        std::cerr << "Key context generation failed" << std::endl;
        return;
    };

    if(!EVP_PKEY_keygen_init(kctx)) {
        std::cerr << "Key generation init failed" << std::endl;
        return;
    };

    if (!EVP_PKEY_keygen(kctx, privateKeyOut)) {
        std::cerr << "Key generation failed" << std::endl;
        return;
    };

	EVP_PKEY_CTX_free(kctx);
	EVP_PKEY_CTX_free(pctx);
}

void
ecies_ka_ecdh_opensslv3(uint8_t* rxPubKeyIn, size_t rxPubKeyLengthIn, 
    uint8_t* txEphPrivKeyIn, size_t txEphPrivKeyLengthIn, 
    uint8_t** sharedSecretOut, size_t* sharedSecretLengthOut)
{
    // Read in RX public key from bytes
    EVP_PKEY* rxPublicKey = NULL;
    EVP_PKEY* txPrivateKey = NULL;
    EVP_PKEY* templateKey = NULL;
    // Need a PKEY to specify curve parameters
    ecies_generate_private_key(&templateKey);
    if (NULL == templateKey) {
        std::cerr << "Template key is null" << std::endl;
        return;
    }
    rxPublicKey = d2i_PublicKey(EVP_PKEY_EC, &templateKey, (const unsigned char**)&rxPubKeyIn, rxPubKeyLengthIn);

    // // Read in tx private key from bytes
    txPrivateKey = d2i_PrivateKey(EVP_PKEY_EC, &templateKey, (const unsigned char**)&txEphPrivKeyIn, txEphPrivKeyLengthIn);

    // Agree shared secret bytes
    // See https://wiki.openssl.org/index.php/Elliptic_Curve_Diffie_Hellman#Using_ECDH_in_OpenSSL

	EVP_PKEY_CTX *pctx, *kctx;
	EVP_PKEY_CTX *ctx;
	// EVP_PKEY *params = NULL;
	// /* Create the context for parameter generation */
	// if(NULL == (pctx = EVP_PKEY_CTX_new_id(EVP_PKEY_EC, NULL))) {};

	// /* Initialise the parameter generation */
	// if(1 != EVP_PKEY_paramgen_init(pctx)) {};

	// /* We're going to use the ANSI X9.62 Prime 256v1 curve */
	// if(1 != EVP_PKEY_CTX_set_ec_paramgen_curve_nid(pctx, NID_X9_62_prime256v1)) {};

	// /* Create the parameter object params */
	// if (!EVP_PKEY_paramgen(pctx, &params)) {};

    // We load our tx private key from elsewhere
	// /* Create the context for the key generation */
	// if(NULL == (kctx = EVP_PKEY_CTX_new(params, NULL))) {};
	// /* Generate the key */
	// if(1 != EVP_PKEY_keygen_init(kctx)) {};
	// if (1 != EVP_PKEY_keygen(kctx, &txPrivateKey)) {};

	/* Create the context for the shared secret derivation */
	if(NULL == (ctx = EVP_PKEY_CTX_new(txPrivateKey, NULL))) {}; // TODO REQUIRE FIPS ENGINE

	/* Initialise */
	if(1 != EVP_PKEY_derive_init(ctx)) {};

	/* Provide the peer public key */
	if(1 != EVP_PKEY_derive_set_peer(ctx, rxPublicKey)) {};

	/* Determine buffer length for shared secret */
	if(1 != EVP_PKEY_derive(ctx, NULL, sharedSecretLengthOut)) {};

	/* Create the buffer */
	if(NULL == (*sharedSecretOut = (uint8_t*) OPENSSL_malloc(*sharedSecretLengthOut))) {};

	/* Derive the shared secret */
	if(1 != (EVP_PKEY_derive(ctx, *sharedSecretOut, sharedSecretLengthOut))) {};

	EVP_PKEY_CTX_free(ctx);
	EVP_PKEY_free(rxPublicKey);
	EVP_PKEY_free(txPrivateKey);
	// EVP_PKEY_free(templateKey); // freed already during key generation
	// EVP_PKEY_CTX_free(kctx);
	// EVP_PKEY_free(params);
	// EVP_PKEY_CTX_free(pctx);

	/* Never use a derived secret directly. Typically it is passed
	 * through some hash function to produce a key */
    // Note pass sharedSecretKey through HMAC_SHA256 in KDF
}

void
ecies_kdf_x963sha256_opensslv3( // Note that the shared secret contains the tx public key info in ECIES
    uint8_t* sharedSecretIn, size_t sharedSecretLengthIn, 
    uint8_t** kEncOut16bytes, uint8_t** ivOut16bytes)
{
    // See https://www.openssl.org/docs/manmaster/man7/EVP_KDF-X963.html
    EVP_KDF *kdf;
    EVP_KDF_CTX *kctx;
    unsigned char out[32];

    OSSL_PARAM params[4], *p = params;

    kdf = EVP_KDF_fetch(NULL, "X963KDF", NULL);
    kctx = EVP_KDF_CTX_new(kdf);
    EVP_KDF_free(kdf);

    *p++ = OSSL_PARAM_construct_utf8_string(OSSL_KDF_PARAM_DIGEST,
                                            SN_sha256, strlen(SN_sha256)); // TODO WHERE ARE THESE DERIVED FROM?
    *p++ = OSSL_PARAM_construct_octet_string(OSSL_KDF_PARAM_SECRET,
                                            (unsigned char*)sharedSecretIn, sharedSecretLengthIn);
    *p++ = OSSL_PARAM_construct_octet_string(OSSL_KDF_PARAM_INFO,
                                            (char*)"label", (size_t)5);
    // Note: We do not provide a salt (No need for ECIES)
    *p = OSSL_PARAM_construct_end();
    if (EVP_KDF_derive(kctx, out, sizeof(out), params) <= 0) {
        // error("EVP_KDF_derive");
        std::cerr << "EC KDF failed" << std::endl;
        return;
    }

    // Copy over bytes
    *ivOut16bytes = new uint8_t[16];
    *kEncOut16bytes = new uint8_t[16];
    for (size_t i = 0;i < 16;++i) {
        (*kEncOut16bytes)[i] = out[i];
    }
    for (size_t i = 0;i < 16;++i) {
        (*ivOut16bytes)[i] = out[i + 16];
    }

    EVP_KDF_CTX_free(kctx);
}

void
ecies_encrypt_aes128gcm_opensslv3(uint8_t* plaintextIn, size_t plaintextLengthIn, 
    uint8_t* kEncIn16bytes, uint8_t* ivIn16bytes, 
    uint8_t* additionalAuthDataIn, size_t additionalAuthDataLengthIn, 
    uint8_t** cryptogramWithTagOut, size_t* cryptogramWithTagLengthOut)
{
    // See https://wiki.openssl.org/index.php/EVP_Authenticated_Encryption_and_Decryption#Authenticated_Encryption_using_GCM_mode
    EVP_CIPHER_CTX *ctx;

    int len;

    int ciphertext_len;

    unsigned char emptyIv[16];
    for (size_t i = 0;i < 16;++i) {
        emptyIv[i] = 0;
    }


    /* Create and initialise the context */
    if(!(ctx = EVP_CIPHER_CTX_new())) {
        std::cerr << "Failed to create cipher context" << std::endl;
        return;
    }

    /* Initialise the encryption operation. */
    if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_gcm(), NULL, NULL, NULL)) {
        std::cerr << "Failed to initialise encryption for aes 128 gcm" << std::endl;
        return;
    }

    // Remove all padding
    if(!EVP_CIPHER_CTX_set_padding(ctx, 0)) {
        std::cerr << "Failed to set padding to zero" << std::endl;
        return;
    }

    /*
     * Set IV length for ECIES to 16
     */
    if(1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, 16, NULL)) {
        std::cerr << "Failed to set IV length to 16 for ECIES" << std::endl;
        return;
    }

    /* Initialise key and IV */
    if(1 != EVP_EncryptInit_ex(ctx, NULL, NULL, (const unsigned char*)kEncIn16bytes, emptyIv /*(const unsigned char*)ivIn16bytes*/)) {
        std::cerr << "Failed to set key and iv" << std::endl;
        return;
    }

    /*
     * Provide any AAD data. This can be called zero or more times as
     * required
     */
    if (0 != additionalAuthDataLengthIn && NULL != additionalAuthDataIn) {
        if(1 != EVP_EncryptUpdate(ctx, NULL, &len, (const unsigned char*)additionalAuthDataIn, additionalAuthDataLengthIn)) {
            std::cerr << "Failed to set additional authentication data" << std::endl;
            return;
        }
    }

    *cryptogramWithTagOut = new uint8_t[plaintextLengthIn + 16];

    /*
     * Provide the message to be encrypted, and obtain the encrypted output.
     * EVP_EncryptUpdate can be called multiple times if necessary
     */
    if(1 != EVP_EncryptUpdate(ctx, (unsigned char*)*cryptogramWithTagOut, &len, (const unsigned char*)plaintextIn, plaintextLengthIn)) {
        std::cerr << "Failed to perform encryption" << std::endl;
        return;
    }
    ciphertext_len = len;

    /*
     * Finalise the encryption. Normally ciphertext bytes may be written at
     * this stage, but this does not occur in GCM mode
     */
    if(1 != EVP_EncryptFinal_ex(ctx, ((unsigned char*)*cryptogramWithTagOut) + len, &len)) {
        std::cerr << "Failed to complete encryption" << std::endl;
        return;
    }
    ciphertext_len += len;

    /* Get the tag */
    if(1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, 16, ((unsigned char*)*cryptogramWithTagOut) + ciphertext_len)) {
        std::cerr << "Failed to generate tag" << std::endl;
        return;
    }
    ciphertext_len += 16;
    (*cryptogramWithTagLengthOut) = ciphertext_len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);
}

void
ecies_decrypt_aes128gcm_opensslv3(uint8_t* ciphertextIn, size_t ciphertextLengthIn, 
    uint8_t* kEncIn16bytes, uint8_t* ivIn16bytes,
    uint8_t* additionalAuthDataIn, size_t additionalAuthDataLengthIn, 
    uint8_t** plaintextOut, size_t* plaintextLengthOut)
{
    // See https://wiki.openssl.org/index.php/EVP_Authenticated_Encryption_and_Decryption#Authenticated_Decryption_using_GCM_mode

    EVP_CIPHER_CTX *ctx;
    int len;
    int ret;

    unsigned char emptyIv[16];
    for (size_t i = 0;i < 16;++i) {
        emptyIv[i] = 0;
    }

    /* Create and initialise the context */
    if(!(ctx = EVP_CIPHER_CTX_new())) {
        std::cerr << "Failed to create cipher context" << std::endl;
        return;
    }

    /* Initialise the decryption operation. */
    if(!EVP_DecryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, NULL, NULL)) {
        std::cerr << "Failed to initialise decryption" << std::endl;
        return;
    }

    // Remove all padding
    if(!EVP_CIPHER_CTX_set_padding(ctx, 0)) {
        std::cerr << "Failed to set padding to zero" << std::endl;
        return;
    }

    /* Set IV length. Not necessary if this is 12 bytes (96 bits). (Note that ECIES uses 16 bytes) */
    if(!EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, 16, NULL)) {
        std::cerr << "Failed to set IV length" << std::endl;
        return;
    }

    /* Initialise key and IV */
    if(!EVP_DecryptInit_ex(ctx, NULL, NULL, (const unsigned char*)kEncIn16bytes, emptyIv /*(const unsigned char*)ivIn16bytes*/)) {
        std::cerr << "Failed to set key and iv" << std::endl;
        return;
    }

    /*
     * Provide any AAD data. This can be called zero or more times as
     * required
     */
    if (0 != additionalAuthDataLengthIn && NULL != additionalAuthDataIn) {
        if(!EVP_DecryptUpdate(ctx, NULL, &len, (const unsigned char*)additionalAuthDataIn, additionalAuthDataLengthIn)) {
            std::cerr << "Failed to set authentication data" << std::endl;
            return;
        }
    }

    *plaintextOut = new uint8_t[ciphertextLengthIn - 16];

    /*
     * Provide the message to be decrypted, and obtain the plaintext output.
     * EVP_DecryptUpdate can be called multiple times if necessary
     */
    if(!EVP_DecryptUpdate(ctx, (unsigned char*)*plaintextOut, &len, (const unsigned char*)ciphertextIn, ciphertextLengthIn - 16)) {
        std::cerr << "Failed to do decryption" << std::endl;
        return;
    }
    *plaintextLengthOut = len;

    /* Set expected tag value. Works in OpenSSL 1.0.1d and later */
    if(!EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_TAG, 16, ciphertextIn + ciphertextLengthIn - 16)) {
        std::cerr << "Failed to set GCM tag" << std::endl;
        return;
    }

    /*
     * Finalise the decryption. A positive return value indicates success,
     * anything else is a failure - the plaintext is not trustworthy.
     */
    ret = EVP_DecryptFinal_ex(ctx, ((unsigned char*)*plaintextOut) + len, &len);

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    if(ret > 0) {
        /* Success */
        *plaintextLengthOut += len;
    } else {
        /* Verify failed */
        std::cerr << "Failed to verify tag" << std::endl;
    }

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