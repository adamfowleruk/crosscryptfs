//  Copyright 2023 crosscryptfs Project Contributors
//  SPDX-License-Identifier: Apache-2.0
//

#ifndef FILESYSTEMVOLUME_H
#define FILESYSTEMVOLUME_H

#include <memory>
#include <string>
#include "volume.h"
#include "io.h"
#include <sstream>

namespace crosscryptfs {

// Low-level C style functions first


// High level C++ classes

/*!
 * \brief Manages a local (mounted), raw and unencrypted, File System folder and exposes it as a CrossCryptFS VolumeProvider
 */
class FileSystemVolumeProvider : public VolumeProvider {
public:
    FileSystemVolumeProvider(std::string wrappedFolder) noexcept;
    ~FileSystemVolumeProvider() noexcept;
    FileSystemVolumeProvider(const FileSystemVolumeProvider&) = delete;
    FileSystemVolumeProvider(FileSystemVolumeProvider&&) = delete;
    FileSystemVolumeProvider& operator=(const FileSystemVolumeProvider&) = delete;
    FileSystemVolumeProvider& operator=(FileSystemVolumeProvider&&) = delete;

    void add(const std::string relativePath,BytesProvider src, const size_t length) noexcept override;
    std::ostream&& addStream(const std::string relativePath) noexcept override;
    void remove(const std::string relativePath) noexcept override;
    void destroy() noexcept override;
    void list(std::vector<FileEntry>& addTo, std::string relativeFolderName) noexcept override;


private:
    class Impl;
    std::unique_ptr<Impl> mImpl;
};

} // end namespace

#endif