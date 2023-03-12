//  Copyright 2023 crosscryptfs Project Contributors
//  SPDX-License-Identifier: Apache-2.0
//

#include "crosscryptfs.h"
#include "noencryption.h"
#include "file.h"
#include "volume.h"

#include <vector>
#include <string>
#include <sstream>
#include <istream>
#include <fstream>
#include <iostream>
#include <filesystem>

namespace crosscryptfs {

class CrossCryptFS::Impl {
public:
    Impl(VolumeProvider& wrappedFolder);
    ~Impl();
    Impl(const Impl&) = delete;
    Impl(Impl&&) = delete;

    VolumeProvider& m_wrapped;
    std::shared_ptr<EncryptionProvider> m_provider;
};

CrossCryptFS::Impl::Impl(VolumeProvider& wrappedFolder)
    : m_wrapped(wrappedFolder),
      m_provider(std::make_shared<NoEncryptionProvider>())
{
    ;
}

CrossCryptFS::Impl::~Impl() {}


// Class method implementations

CrossCryptFS::CrossCryptFS(VolumeProvider& wrappedFolder)
  : mImpl(std::make_unique<CrossCryptFS::Impl>(wrappedFolder))
{
    // namespace fs = std::filesystem;
    // if (!fs::exists(wrappedFolder)) {
    //     fs::create_directories(wrappedFolder);
    // }
}

CrossCryptFS::~CrossCryptFS() {}

void
CrossCryptFS::setEncryptionProvider(std::shared_ptr<EncryptionProvider> provider)
{
    mImpl->m_provider = provider;
}

void
CrossCryptFS::importFile(std::string fromFileFull, std::string toFileRelative)
{
    namespace fs = std::filesystem;
    
    std::ifstream src(fromFileFull);
    // fs::path dest(mImpl->m_wrapped);
    // dest /= toFileRelative;
    // std::ofstream internalFile(dest);
    auto&& internalFile = mImpl->m_wrapped.addStream(toFileRelative);

    mImpl->m_provider->encryptContent(src,internalFile);

    src.close();
    // internalFile.close();
}

void
CrossCryptFS::exportFile(std::string toFileFull, std::string fromFileRelative)
{
    namespace fs = std::filesystem;

    std::ofstream destFile(toFileFull);
    // fs::path internalPath(mImpl->m_wrapped);
    // internalPath /= fromFileRelative;
    // std::ifstream internalFile(internalPath);
    // mImpl->m_provider->decryptContent(internalFile,destFile);
    
    // internalFile.close();
    // destFile.close();
}

void
CrossCryptFS::list(std::vector<FileEntry>& addTo, std::string relativeFolderName)
{
    // TODO use relativeFolderName rather than assuming the root folder
    namespace fs = std::filesystem;
    // fs::path fp(mImpl->m_wrapped);
    // for (auto& p : fs::directory_iterator(fp)) {
    //     if (p.exists() && p.is_regular_file()) {
    //         addTo.emplace_back(p.path().filename(),p.path().filename(),FileType::file);
    //     }
    // }       
}

}