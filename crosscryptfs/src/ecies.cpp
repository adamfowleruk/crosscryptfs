//  Copyright 2023 crosscryptfs Project Contributors
//  SPDX-License-Identifier: Apache-2.0
//

#include "ecies.h"
#include <iosfwd>
#include <string>

using namespace crosscryptfs;

class ECIESEncryptionProvider::Impl {
public:
    Impl();
    ~Impl();

    // TODO state values here
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

void
ECIESEncryptionProvider::setParameterValue(std::string name,std::string value)
{
    // Set parameters based on config file settings
}

std::string
ECIESEncryptionProvider::getParameterValues()
{
    return std::string("");
}

void
ECIESEncryptionProvider::setKeyMaterial(std::istream& keyDataIn)
{

}

void
ECIESEncryptionProvider::encryptContent(std::istream& plainIn,std::ostream& encryptedOut)
{
    // TODO make this do encryption
}

void
ECIESEncryptionProvider::decryptContent(std::istream& encryptedIn,std::ostream& plainOut)
{
    // TODO make this do decryption
}

// TODO any ECIES Specific methods below here