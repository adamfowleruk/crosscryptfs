//  Copyright 2026 crosscryptfs Project Contributors
//  SPDX-License-Identifier: Apache-2.0
//

// The main crosscryptfs C CLI executable app file

#include "crosscryptfs.h"

#include <openssl/bn.h>
#include <openssl/decoder.h>
#include <openssl/ec.h>
#include <openssl/ecdh.h>
#include <openssl/pem.h>
#include <openssl/rand.h>
#include <openssl/evp.h>


int main(int argc, char *argv[]) {
  printf("Entry point\n");

  EC_KEY *ec_key = nullptr; // EC key from keyfile

  // RX
  uint8_t *pubk      = nullptr; // RX pub key
  size_t   pubk_len  = 0;
  uint8_t *privk     = nullptr; // TX priv key
  size_t   privk_len = 0;
  int      curve;

  // TX
  uint8_t *epubk     = nullptr;
  size_t   epubk_len = 0;


  // load ecies pub key
  const EC_GROUP *ec_group = nullptr;
  BIO            *bio_key  = nullptr;
  BIO            *bio_out  = nullptr; /* stdout */

  printf("Loading file\n");

  bio_key = BIO_new_file(argv[1], "r");
  if (bio_key == nullptr) {
    // std::cerr << "Failed to read EC key file '" << argv[1] << std::endl;
    printf("BIO_new_file() failed\n");
    return 1;
  }

  OSSL_DECODER_CTX*    dctx;
  EVP_PKEY*   key   = nullptr;
  dctx = OSSL_DECODER_CTX_new_for_pkey(&key, "DER", nullptr, "EC",
              0, nullptr, nullptr);
  if (dctx == NULL) {
    printf("OSSL_DECODER_CTX_new_for_pkey() failed\n");
    return 1;
  }
  if (OSSL_DECODER_from_bio(dctx, bio_key) != 1) {
    printf("OSSL_DECODER_from_bio() failed\n");
    return 1;
  }
  printf("Getting EC key from bio key\n");
  ec_key = d2i_ECPrivateKey_bio(bio_key, nullptr); // something wrong here!
  if (ec_key == nullptr) {
    // std::cerr << "Failed to parse EC key file '" << argv[1] << std::endl;
    printf("Failed to extract EC key - EC key is null!\n");
    return 1;
  }
  BIO_free(bio_key);

  // We have now loaded our EC Private Key into EVP_PKEY* key
  // We want to extract our public key from this also



  printf("Fetching curve params\n");

  // get curve params
  ec_group = EC_KEY_get0_group(ec_key);

  // int nameOk = EVP_PKEY_get_group_name(key, ec_group, &pubk_len);

  printf("Creating bio wrapper\n");
  // Create bio wrapper
  bio_out = BIO_new_fp(stdout, BIO_NOCLOSE);

  // set point curve
  EC_KEY_set_conv_form(ec_key, POINT_CONVERSION_UNCOMPRESSED);


  printf("Converting curve to binary array format\n");
  // Get curve in binary array format
  ec_group   = EC_KEY_get0_group(ec_key);
  const EC_POINT *pub        = EC_KEY_get0_public_key(ec_key);
  BIGNUM         *pub_bn     = BN_new();
  BN_CTX         *pub_bn_ctx = BN_CTX_new();

  BN_CTX_start(pub_bn_ctx);

  EC_POINT_point2bn(ec_group, pub, POINT_CONVERSION_UNCOMPRESSED,
                    pub_bn, pub_bn_ctx);


  printf("Extracting public key\n");
  pubk_len = BN_num_bytes(pub_bn);
  pubk = (uint8_t*)OPENSSL_malloc(pubk_len);

  if (BN_bn2bin(pub_bn, pubk) != pubk_len) {
      // std::cerr << "Failed to decode pubkey" << std::endl;
    printf("BN_bn2bin() pub_bn failed\n");
      return 1;
  }

  BN_CTX_end(pub_bn_ctx);
  BN_CTX_free(pub_bn_ctx);
  BN_clear_free(pub_bn);

  // get curve name
  curve = EC_GROUP_get_curve_name(ec_group);


  printf("Extracting private key\n");

  // get ec private key
  const BIGNUM *priv = EC_KEY_get0_private_key(ec_key);

  privk_len = BN_num_bytes(priv);
  privk = (uint8_t*)OPENSSL_malloc(privk_len);




  uint8_t *skey      = nullptr; // from ECDH
  size_t   skey_len  = 0;


  // Generate Transmitter sym key
  EC_POINT       *peer_pubk_point = nullptr;

  // Crete empty transmitter key pair
  ec_key = EC_KEY_new_by_curve_name(curve);
  EC_KEY_generate_key(ec_key);
  ec_group = EC_KEY_get0_group(ec_key);

  // buffer for sym key
  skey_len = ((EC_GROUP_get_degree(ec_group) + 7) / 8);
  skey     = (uint8_t*)OPENSSL_malloc(skey_len);

  // Convert pub key to EC point
  BIGNUM   *pubk_bn;
  BN_CTX   *pubk_bn_ctx;

  peer_pubk_point = EC_POINT_new(ec_group);

  pubk_bn = BN_bin2bn(pubk, pubk_len, nullptr);
  pubk_bn_ctx = BN_CTX_new();
  BN_CTX_start(pubk_bn_ctx);

  EC_POINT_bn2point(ec_group, pubk_bn, peer_pubk_point, pubk_bn_ctx);

  BN_CTX_end(pubk_bn_ctx);
  BN_CTX_free(pubk_bn_ctx);
  BN_clear_free(pubk_bn);



  // NOTE: The above would be done external to CrossCryptFS CLI, with the sym
  // key bootstrapped into the CLI eventually by an external mapper
  // (E.g. a MAX OS X wrapper would use Apple's Crypto library to decrypt the
  //  sym key via the T2 TPM module)
  // Note: We may provide a backup method of password encryption when invoking
  //       the CLI in instances where there is no TPM module.


  // Create sym key
  skey_len = ECDH_compute_key(skey, skey_len, peer_pubk_point,
                                ec_key, nullptr);


  const BIGNUM *newpriv = EC_KEY_get0_private_key(ec_key);

  privk_len = BN_num_bytes(newpriv);
  privk = (uint8_t*)OPENSSL_malloc(privk_len);

  if (BN_bn2bin(priv, privk) != privk_len) {
      // std::cerr << "Failed to decode privkey" << std::endl;
    printf("BN_bn2bin() priv failed\n");
      return 1;
  }

  printf("BN_bn2bin() priv succeeded.\n");


  // TODO do something with our symmetric key now


  printf("Done!\n");
  return 0;
}
