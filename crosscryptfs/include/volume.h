//  Copyright 2023 crosscryptfs Project Contributors
//  SPDX-License-Identifier: Apache-2.0
//

#ifndef VOLUME_H
#define VOLUME_H

#include <string>
#include "io.h"

namespace crosscryptfs {

// Low-level C style functions first


// High level C++ classes

class VolumeProvider {
public:
    VolumeProvider() noexcept = default;
    virtual ~VolumeProvider() noexcept = default;

    virtual void add(const std::string relativePath, BytesProvider src, const size_t length) noexcept = 0;
    virtual void destroy() noexcept = 0;
};

} // end namespace

#endif