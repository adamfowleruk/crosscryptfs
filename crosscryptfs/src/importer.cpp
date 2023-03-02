//  Copyright 2023 crosscryptfs Project Contributors
//  SPDX-License-Identifier: Apache-2.0
//
#include "importer.h"
#include "crosscryptfs.h"
#include <string>

using namespace crosscryptfs;

class ImportOperation::Impl {
public:
    Impl(CrossCryptFS& fs,std::string sourceFolder);
    ~Impl();

// These are public for ease
    CrossCryptFS& m_fs;
    std::string m_sourceFolder;
};

ImportOperation::Impl::Impl(CrossCryptFS& fs,std::string sourceFolder)
    : m_fs(fs),
      m_sourceFolder(sourceFolder)
{
    ;
}

ImportOperation::Impl::~Impl() {}


// Class method implementations

ImportOperation::ImportOperation(CrossCryptFS& fs,std::string sourceFolder)
  : mImpl(std::make_unique<ImportOperation::Impl>(fs,sourceFolder))
{
    ;
}

ImportOperation::~ImportOperation() {}

void
ImportOperation::runToCompletion()
{
    // TODO
}