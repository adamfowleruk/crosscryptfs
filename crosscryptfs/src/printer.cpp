//  Copyright 2023 crosscryptfs Project Contributors
//  SPDX-License-Identifier: Apache-2.0
//
#include "printer.h"
#include "crosscryptfs.h"
#include <string>

using namespace crosscryptfs;

class MetadataPrinter::Impl {
public:
    Impl();
    ~Impl();
};

MetadataPrinter::Impl::Impl() {}
MetadataPrinter::Impl::~Impl() {}


// Class method implementations

MetadataPrinter::MetadataPrinter()
  : mImpl(std::make_unique<MetadataPrinter::Impl>())
{
    ;
}

MetadataPrinter::~MetadataPrinter() {}

void
MetadataPrinter::print(CrossCryptFS& fs)
{
    // TODO
}