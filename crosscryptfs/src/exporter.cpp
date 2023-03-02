//  Copyright 2023 crosscryptfs Project Contributors
//  SPDX-License-Identifier: Apache-2.0
//
#include "exporter.h"
#include "crosscryptfs.h"
#include <string>

using namespace crosscryptfs;

class ExportOperation::Impl {
public:
    Impl(CrossCryptFS& fs,std::string targetFolder);
    ~Impl();

// These are public for ease
    CrossCryptFS& m_fs;
    std::string m_targetFolder;
};

ExportOperation::Impl::Impl(CrossCryptFS& fs,std::string targetFolder)
    : m_fs(fs),
      m_targetFolder(targetFolder)
{
    ;
}

ExportOperation::Impl::~Impl() {}

// Class method implementations

ExportOperation::ExportOperation(CrossCryptFS& fs,std::string targetFolder)
  : mImpl(std::make_unique<ExportOperation::Impl>(fs,targetFolder))
{
    ;
}

ExportOperation::~ExportOperation() {}

void
ExportOperation::runToCompletion()
{
    // TODO
}
