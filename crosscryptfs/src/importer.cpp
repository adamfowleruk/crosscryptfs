//  Copyright 2023 crosscryptfs Project Contributors
//  SPDX-License-Identifier: Apache-2.0
//

#include "importer.h"
#include "crosscryptfs.h"
#include <string>
#include <filesystem>

namespace crosscryptfs {

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
    namespace fs = std::filesystem;
    // List all files immediately below the sourceFolder
    fs::path fp(mImpl->m_sourceFolder);
    for (auto& p : fs::directory_iterator(fp)) {
        if (p.exists() && p.is_regular_file()) {
            // Import each one into fileystem
            mImpl->m_fs.importFile(p.path(), p.path().filename());
        }
    }
}

} // end namespace