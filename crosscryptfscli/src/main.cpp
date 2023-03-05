//  Copyright 2023 crosscryptfs Project Contributors
//  SPDX-License-Identifier: Apache-2.0
//

/*
 * The main executable of the crosscryptfs CLI
 */
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <memory>
#include "crosscryptfs/crosscryptfs.h"

#include <openssl/bn.h>
#include <openssl/ec.h>
#include <openssl/ecdh.h>
#include <openssl/pem.h>
#include <openssl/rand.h>
#include <openssl/evp.h>

using namespace crosscryptfs;

int main(int argc, char* argv[]) {
  // For now just encrypt and decrypt a folder

  // Perform 'out of band' ECIES generation of a private key (left UNENCRYPTED for now)
  // and a public key


  EC_KEY *ec_key = NULL; // EC key from keyfile
  
  // RX
  uint8_t *pubk      = NULL; // RX pub key
  size_t   pubk_len  = 0;
  uint8_t *privk     = NULL; // TX priv key
  size_t   privk_len = 0;
  int      curve;

  // TX
  uint8_t *epubk     = NULL;
  size_t   epubk_len = 0;

  // AES-GCM
  // uint8_t *iv             = NULL;
  // uint8_t  iv_len         = 0;
  // uint8_t *tag            = NULL;
  // uint8_t  tag_len        = 0;
  // uint8_t *ciphertext     = NULL;
  // uint8_t  ciphertext_len = 0;

  if (argc < 2) {
    std::cerr << "ERROR: No EC key specified." << std::endl;
    std::cerr << "Usage: " << argv[0] << " ./ecc_key.der" << std::endl;
    return 1;
  }

  // load ecies pub key
  const EC_GROUP *ec_group = NULL;
  BIO            *bio_key  = NULL;
  BIO            *bio_out  = NULL; /* stdout */

  bio_key = BIO_new_file(argv[1], "r");
  if (bio_key == NULL) {
      std::cerr << "Failed to read EC key file '" << argv[1] << std::endl;
      return 1;
  }
  ec_key = d2i_ECPrivateKey_bio(bio_key, NULL);
  if (ec_key == NULL) {
      std::cerr << "Failed to parse EC key file '" << argv[1] << std::endl;
      return 1;

  }
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

  pubk_len = BN_num_bytes(pub_bn);
  pubk = (std::uint8_t*)OPENSSL_malloc(pubk_len);

  if (BN_bn2bin(pub_bn, pubk) != pubk_len) {
      std::cerr << "Failed to decode pubkey" << std::endl;
      return 1;
  }

  BN_CTX_end(pub_bn_ctx);
  BN_CTX_free(pub_bn_ctx);
  BN_clear_free(pub_bn);

  // get curve name
  curve = EC_GROUP_get_curve_name(ec_group);



  // get ec private key
  const BIGNUM *priv = EC_KEY_get0_private_key(ec_key);

  privk_len = BN_num_bytes(priv);
  privk = (std::uint8_t*)OPENSSL_malloc(privk_len);




  uint8_t *skey      = NULL; // from ECDH
  size_t   skey_len  = 0;


  // Generate Transmitter sym key
  EC_POINT       *peer_pubk_point = NULL;

  // Crete empty transmitter key pair
  ec_key = EC_KEY_new_by_curve_name(curve);
  EC_KEY_generate_key(ec_key);
  ec_group = EC_KEY_get0_group(ec_key);

  // buffer for sym key
  skey_len = ((EC_GROUP_get_degree(ec_group) + 7) / 8);
  skey     = (std::uint8_t*)OPENSSL_malloc(skey_len);

  // Convert pub key to EC point
  BIGNUM   *pubk_bn;
  BN_CTX   *pubk_bn_ctx;

  peer_pubk_point = EC_POINT_new(ec_group);

  pubk_bn = BN_bin2bn(pubk, pubk_len, NULL);
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
                                ec_key, NULL);

  const BIGNUM *newpriv = EC_KEY_get0_private_key(ec_key);

  privk_len = BN_num_bytes(newpriv);
  privk = (std::uint8_t*)OPENSSL_malloc(privk_len);

  if (BN_bn2bin(priv, privk) != privk_len) {
      std::cerr << "Failed to decode privkey" << std::endl;
      return 1;
  }

  // Note that the public key is in the file passed to this main command
  namespace fs = std::filesystem;
  if (!fs::exists("encryptedfolder/.crosscryptfs")) {
      fs::create_directories("encryptedfolder/.crosscryptfs");
  }

  // save the private key (privk) to .crosscryptfs/ecies.txprivkey for now - THIS WILL BE EXTERNAL LATER in a TPM or similar
  std::cout << "transmitter private key length: " << privk_len << std::endl;
  std::ofstream privkeyos("encryptedfolder/.crosscryptfs/ecies.txprivatekey");
  for (std::size_t i = 0; i < privk_len;++i) {
    privkeyos << privk[i];
  }
  privkeyos.close();

  // save the rx pub key in binary format for convenience
  std::cout << "receiver public key length: " << pubk_len << std::endl;
  std::ofstream pubkeyos("encryptedfolder/.crosscryptfs/ecies.rxpublickey");
  for (std::size_t i = 0; i < pubk_len;++i) {
    pubkeyos << pubk[i];
  }
  pubkeyos.close();

  // TODO the X9.63 steps are currently missing from this code - we're instead
  // directly using the sym key as the kENC rather than using this with the rx
  // public key and the X9.63 KDF to generate a 16 byte IV and a 16 byte kENC.
  // I.e. today this isn't compatible with Apple's ECIES mechanism.

  // save the sym key (skey) UNENCRYPTED for now to .crosscryptfs/keyfile
  // WARNING: Actual sym key MUST be held in memory in future.
  //          This ECIES keyfile will be XORed with a randomly generated
  //          keyfile (allowing for the txprivkey or rxpubkey to be changed 
  //          without full a decryption/encryption cycle of all files)
  std::cout << "symmetric key length: " << skey_len << std::endl;
  std::ofstream keyfileos("encryptedfolder/.crosscryptfs/keyfile");
  for (std::size_t i = 0; i < skey_len;++i) {
    keyfileos << skey[i];
  }
  keyfileos.close();






  // Define our wrapped managed storage folder
  CrossCryptFS encfs("encryptedfolder");

  // TODO don't bother with private key - just load sym key (ECIES Shared Secret Z) from the command line like recipient public key

  // std::shared_ptr<EncryptionProvider> enc = std::make_shared<NoEncryptionProvider>();
  std::shared_ptr<EncryptionProvider> enc = std::make_shared<ECIESEncryptionProvider>();
  // Configure ECIES provider with key material and strength settings
  std::ifstream eciestxprivkey("encryptedfolder/.crosscryptfs/ecies.txprivkey");
  std::stringstream eciesTxPrivKeyAsString;
  std::string line;
  if (eciestxprivkey) {
    while (std::getline(eciestxprivkey, line)) {
        eciesTxPrivKeyAsString << line; // only ever one line
    }
  }
  eciestxprivkey.close();
  enc->setParameterValue("txprivkey",eciesTxPrivKeyAsString.str());
  std::ifstream keyfile("encryptedfolder/.crosscryptfs/keyfile");
  enc->setKeyMaterial(keyfile);
  keyfile.close();

  encfs.setEncryptionProvider(enc);

  MetadataPrinter printer;
  std::cout << "Content before import (empty):-" << std::endl;
  printer.print(encfs);

  // Import / encrypt a source folder
  ImportOperation importer(encfs, "/Users/adamfowler/Documents/SomeSourceFolder");
  importer.runToCompletion();
  std::cout << std::endl << "Content after import (encrypted content):-" << std::endl;
  printer.print(encfs);

  // Now export to a destination folder
  ExportOperation exporter(encfs, "/Users/adamfowler/Documents/SomeTargetFolder");
  exporter.runToCompletion();
  std::cout << std::endl << "Content after export (internally we still have encrypted content):-" << std::endl;
  printer.print(encfs);

  // Return success code
  return 0;
}
