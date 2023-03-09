//  Copyright 2023 crosscryptfs Project Contributors
//  SPDX-License-Identifier: Apache-2.0
//

#include "exporter.h"
#include "crosscryptfs.h"
#include <string>
#include <filesystem>

namespace crosscryptfs {

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
    namespace fs = std::filesystem;
    if (!fs::exists(mImpl->m_targetFolder)) {
        fs::create_directories(mImpl->m_targetFolder);
    }

    std::vector<FileEntry> contents;
    mImpl->m_fs.list(contents,"");
    fs::path rootFolder(mImpl->m_targetFolder);
    for (auto& el: contents) {
        fs::path relPath = rootFolder / el.name;
        mImpl->m_fs.exportFile(relPath,el.relativePathOnStorage);
    }
}

} // end namespace