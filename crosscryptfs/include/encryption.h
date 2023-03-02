//  Copyright 2023 crosscryptfs Project Contributors
//  SPDX-License-Identifier: Apache-2.0
//

#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <iosfwd>
#include <string>

namespace crosscryptfs {

class EncryptionProvider {
public:
    EncryptionProvider() = default;
    virtual ~EncryptionProvider() = default;

    // virtual std::string getShortPrefix() = 0;
    // virtual std::string getLongName() = 0;
    // virtual std::string getShortDescription() = 0;
    
    virtual void setParameterValue(std::string name,std::string value) = 0;
    // virtual void setParameterFile(std::string fileExtension,std::string value) = 0;
    virtual std::string getParameterValues() = 0;

    virtual void setKeyMaterial(std::istream& keyDataIn) = 0;

    // Note that these methods assume the resulting encrypted text also includes any data (aside from settings) required for decryption
    virtual void encryptContent(std::istream& plainIn,std::ostream& encryptedOut) = 0;
    virtual void decryptContent(std::istream& encryptedIn,std::ostream& plainOut) = 0;
};

} // end namespace

#endif