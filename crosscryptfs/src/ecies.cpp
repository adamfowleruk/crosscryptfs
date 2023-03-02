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
    // TODO make this do encryption
}

void
ECIESEncryptionProvider::decryptContent(std::istream& encryptedIn,std::ostream& plainOut)
{
    // TODO make this do decryption
}

// TODO any ECIES Specific methods below here