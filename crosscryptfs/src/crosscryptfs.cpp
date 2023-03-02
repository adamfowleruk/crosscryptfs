//  Copyright 2023 crosscryptfs Project Contributors
//  SPDX-License-Identifier: Apache-2.0
//
#include "crosscryptfs.h"
#include <string>

using namespace crosscryptfs;

class CrossCryptFS::Impl {
public:
    Impl(std::string wrappedFolder);
    ~Impl();

private:
    std::string m_wrapped;
};

CrossCryptFS::Impl::Impl(std::string wrappedFolder)
    : m_wrapped(wrappedFolder)
{
    ;
}

CrossCryptFS::Impl::~Impl() {}


// Class method implementations

CrossCryptFS::CrossCryptFS(std::string wrappedFolder)
  : mImpl(std::make_unique<CrossCryptFS::Impl>(wrappedFolder))
{
    ;
}

CrossCryptFS::~CrossCryptFS() {}