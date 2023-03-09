//  Copyright 2023 crosscryptfs Project Contributors
//  SPDX-License-Identifier: Apache-2.0
//

#include "noencryption.h"
#include <string>
#include <istream>
#include <iostream>

namespace crosscryptfs {

class NoEncryptionProvider::Impl {
public:
    Impl();
    ~Impl();

    // TODO state values here
};

NoEncryptionProvider::Impl::Impl()
{
    ;
}

NoEncryptionProvider::Impl::~Impl() {}

// Public ABI implementation class methods

NoEncryptionProvider::NoEncryptionProvider()
    : mImpl(std::make_unique<NoEncryptionProvider::Impl>())
{
    ;
}

NoEncryptionProvider::~NoEncryptionProvider() {}

void
NoEncryptionProvider::setParameterValue(std::string name,std::string value)
{
    // N/A
}

std::string
NoEncryptionProvider::getParameterValues()
{
    return std::string("");
}

void
NoEncryptionProvider::setKeyMaterial(std::istream& keyDataIn)
{
    // N/A
}

void
NoEncryptionProvider::encryptContent(std::istream& plainIn,std::ostream& encryptedOut)
{
    // pass file content straight out
    std::string line;
    if (plainIn && encryptedOut) {
        while (std::getline(plainIn, line)) {
            encryptedOut << line << "\n";
        }
    }
}

void
NoEncryptionProvider::decryptContent(std::istream& encryptedIn,std::ostream& plainOut)
{
    // pass file content straight out
    std::string line;
    if (encryptedIn && plainOut) {
        while (std::getline(encryptedIn, line)) {
            plainOut << line << "\n";
        }
    }
}

} // end namespace