//  Copyright 2023 crosscryptfs Project Contributors
//  SPDX-License-Identifier: Apache-2.0
//

#ifndef ECIES_H
#define ECIES_H

#include "encryption.h"

namespace crosscryptfs {

class ECIESEncryptionProvider : public EncryptionProvider {
public:
    ECIESEncryptionProvider();
    ~ECIESEncryptionProvider();
    
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