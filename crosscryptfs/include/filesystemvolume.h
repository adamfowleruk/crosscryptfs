//  Copyright 2023 crosscryptfs Project Contributors
//  SPDX-License-Identifier: Apache-2.0
//

#ifndef FILESYSTEMVOLUME_H
#define FILESYSTEMVOLUME_H

#include <memory>
#include <string>
#include "volume.h"
#include "io.h"

namespace crosscryptfs {

// Low-level C style functions first


// High level C++ classes

class FileSystemVolumeProvider : public VolumeProvider {
public:
    FileSystemVolumeProvider(std::string wrappedFolder) noexcept;
    ~FileSystemVolumeProvider() noexcept;
    FileSystemVolumeProvider(const FileSystemVolumeProvider&) = delete;
    FileSystemVolumeProvider(FileSystemVolumeProvider&&) = delete;
    FileSystemVolumeProvider& operator=(const FileSystemVolumeProvider&) = delete;
    FileSystemVolumeProvider& operator=(FileSystemVolumeProvider&&) = delete;

    void add(const std::string relativePath,BytesProvider src, const size_t length) noexcept override;
    void destroy() noexcept override;

private:
    class Impl;
    std::unique_ptr<Impl> mImpl;
};

} // end namespace

#endif