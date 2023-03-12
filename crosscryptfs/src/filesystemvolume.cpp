//  Copyright 2023 crosscryptfs Project Contributors
//  SPDX-License-Identifier: Apache-2.0
//

#include "io.h"
#include "volume.h"
#include "filesystemvolume.h"
#include <string>
#include <sstream>
#include <fstream>
#include <istream>
#include <iostream>
#include <filesystem>

namespace crosscryptfs {

class FileSystemVolumeProvider::Impl {
public:
    Impl(std::string wrapped) noexcept;
    ~Impl() noexcept;
    Impl(const Impl&) = delete;
    Impl(Impl&&) = delete;

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

std::ostream&&
FileSystemVolumeProvider::addStream(const std::string relativePath) noexcept
{
    namespace fs = std::filesystem;
    if (!fs::exists(mImpl->wrappedFolder + "/" + relativePath)) { // TODO cross platform
        fs::create_directories(mImpl->wrappedFolder + "/" + relativePath); // TODO cross platform
    }
    return std::move(std::ofstream(mImpl->wrappedFolder + "/" + relativePath));
    // std::ofstream ofs(mImpl->wrappedFolder + "/" + relativePath);
    // return std::ostream(ofs.rdbuf()); // TODO cross platform
}

void
FileSystemVolumeProvider::remove(const std::string relativePath) noexcept
{
    namespace fs = std::filesystem;
    std::string toRemove = mImpl->wrappedFolder + "/" + relativePath;
    if (fs::exists(toRemove)) {
        std::error_code ec;
        std::uintmax_t numberRemoved = fs::remove_all(toRemove, ec);
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

void
FileSystemVolumeProvider::list(std::vector<FileEntry>& addTo, std::string relativeFolderName) noexcept
{
    // TODO use relativeFolderName rather than assuming the root folder
    namespace fs = std::filesystem;
    fs::path fp(mImpl->wrappedFolder + "/" + relativeFolderName);
    for (auto& p : fs::directory_iterator(fp)) {
        if (p.exists()) {
            if (p.is_regular_file()) {
                addTo.emplace_back(p.path().filename().string(),p.path().filename().string(),FileType::file);
            } else if (p.is_directory()) {
                addTo.emplace_back(p.path().filename().string(),p.path().filename().string(),FileType::folder);
            }
        }
    }   
}


} // end namespace