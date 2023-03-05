//  Copyright 2023 crosscryptfs Project Contributors
//  SPDX-License-Identifier: Apache-2.0
//

#include "io.h"
#include "volume.h"
#include "filesystemvolume.h"
#include <string>
#include <fstream>
#include <istream>
#include <iostream>
#include <filesystem>

namespace crosscryptfs {

class FileSystemVolumeProvider::Impl {
public:
    Impl(std::string wrapped) noexcept;
    ~Impl() noexcept;

    std::string wrappedFolder;
};

FileSystemVolumeProvider::Impl::Impl(std::string wrapped) noexcept
    : wrappedFolder(wrapped)
{
    ;
}

FileSystemVolumeProvider::Impl::~Impl() noexcept {}

// Public ABI implementation class methods

FileSystemVolumeProvider::FileSystemVolumeProvider(std::string wrappedFolder) noexcept
    : mImpl(std::make_unique<FileSystemVolumeProvider::Impl>(wrappedFolder))
{
    ;
}

FileSystemVolumeProvider::~FileSystemVolumeProvider() noexcept {}

void
FileSystemVolumeProvider::add(const std::string relativePath,BytesProvider src, const size_t length) noexcept
{
    namespace fs = std::filesystem;
    if (!fs::exists(mImpl->wrappedFolder + "/" + relativePath)) { // TODO cross platform
        fs::create_directories(mImpl->wrappedFolder + "/" + relativePath); // TODO cross platform
    }

    ByteArray bytes;
    std::ofstream outfile(mImpl->wrappedFolder + "/" + relativePath); // TODO cross platform
    if (outfile) {
        size_t read = src->getNextBytes(bytes);
        while (0 < read) {
            outfile << bytes;
        }
        outfile.close();
    }
}

void
FileSystemVolumeProvider::destroy() noexcept
{
    namespace fs = std::filesystem;
    if (fs::exists(mImpl->wrappedFolder)) {
        std::error_code ec;
        std::uintmax_t numberRemoved = fs::remove_all(mImpl->wrappedFolder, ec);
    }
}

} // end namespace