//  Copyright 2023 crosscryptfs Project Contributors
//  SPDX-License-Identifier: Apache-2.0
//

#ifndef NOENCRYPTION_H
#define NOENCRYPTION_H

#include "encryption.h"
#include <istream>
#include <iostream>

namespace crosscryptfs {

/*!
 * \brief Provides no encryption and thus allows simple disk to disk file import/export
 *
 * \note Used primarily for testing but also useful for unencrypted file backups, or
 * to allow large file splitting and small file joining and compression via CrossCryptFS
 * utility functions.
 */
class NoEncryptionProvider : public EncryptionProvider {
public:
    NoEncryptionProvider();
    ~NoEncryptionProvider();
    NoEncryptionProvider(const NoEncryptionProvider&) = delete;
    NoEncryptionProvider(NoEncryptionProvider&&) = delete;
    NoEncryptionProvider& operator=(const NoEncryptionProvider&) = delete;
    NoEncryptionProvider& operator=(NoEncryptionProvider&&) = delete;
    
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