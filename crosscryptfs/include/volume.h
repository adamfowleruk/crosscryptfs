//  Copyright 2023 crosscryptfs Project Contributors
//  SPDX-License-Identifier: Apache-2.0
//

#ifndef VOLUME_H
#define VOLUME_H

#include <string>
#include "io.h"
#include "file.h"

namespace crosscryptfs {

// Low-level C style functions first


// High level C++ classes

/*!
 * \brief A high level virtual interface class representing a store of files and folders in a hierarchy
 */
class VolumeProvider {
public:
    VolumeProvider() noexcept = default;
    virtual ~VolumeProvider() noexcept = default;
    VolumeProvider(const VolumeProvider&) = delete;
    VolumeProvider(VolumeProvider&&) = delete;

    VolumeProvider& operator=(const VolumeProvider&) = delete;
    VolumeProvider& operator=(VolumeProvider&&) = delete;

    virtual void add(const std::string relativePath, BytesProvider src, const size_t length) noexcept = 0;
    virtual std::ostream&& addStream(const std::string relativePath) noexcept = 0;
    virtual void remove(const std::string relativePath) noexcept = 0;
    virtual void destroy() noexcept = 0;
    virtual void list(std::vector<FileEntry>& addTo, std::string relativeFolderName) noexcept = 0;

};

} // end namespace

#endif