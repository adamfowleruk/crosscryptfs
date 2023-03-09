//  Copyright 2023 crosscryptfs Project Contributors
//  SPDX-License-Identifier: Apache-2.0
//

#ifndef IO_H
#define IO_H

#include <cstdint>
#include <iosfwd>
#include <memory>

namespace crosscryptfs {

// Low-level C style functions first


// High level C++ classes
#define BYTEARRAYSIZE 2048
using ByteArray = char[BYTEARRAYSIZE]; // TODO check this works with g++

class IBytesProvider {
public:
    IBytesProvider() noexcept = default;
    virtual ~IBytesProvider() noexcept = default;
    IBytesProvider(const IBytesProvider&) = delete;
    IBytesProvider(IBytesProvider&&) = delete;
    IBytesProvider& operator=(const IBytesProvider&) = delete;
    IBytesProvider& operator=(IBytesProvider&&) = delete;

    virtual size_t getNextBytes(ByteArray& bytes) noexcept = 0;
};

using BytesProvider = std::shared_ptr<IBytesProvider>;

template <typename SrcT>
class IStreamBytesProvider: public IBytesProvider {
public:
    IStreamBytesProvider(SrcT& source) noexcept
        : sourceStream(source)
    {
        ;
    }
    ~IStreamBytesProvider() noexcept {};
    IStreamBytesProvider(const IStreamBytesProvider&) = delete;
    IStreamBytesProvider(IStreamBytesProvider&&) = delete;
    IStreamBytesProvider& operator=(const IStreamBytesProvider&) = delete;
    IStreamBytesProvider& operator=(IStreamBytesProvider&&) = delete;

    size_t getNextBytes(ByteArray& bytes) noexcept override
    {
        sourceStream.read(bytes,BYTEARRAYSIZE);
        return sourceStream.gcount();
    }

private:
    SrcT& sourceStream;
};

// Instantiation declarations for common types in the translation unit
// using StreamBytesProvider = std::shared_ptr<IStreamBytesProvider>;
using BasicStringStreamBytesProvider = std::shared_ptr<IStreamBytesProvider<std::istringstream>>;

} // end namespace

#endif