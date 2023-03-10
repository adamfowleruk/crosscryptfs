//  Copyright 2023 crosscryptfs Project Contributors
//  SPDX-License-Identifier: Apache-2.0
//

#ifndef CROSSCRYPTFS_H
#define CROSSCRYPTFS_H

#include "file.h"
#include "context.h"
#include "encryption.h"
#include <string>
#include <vector>

/*! \brief The CrossCryptFS namespace where all our classes and functions exist. */
namespace crosscryptfs {

// Low-level C style functions first






// Now C++ wrappers

/*!
 * \brief The main utility class for CrossCryptFS
 */
class CrossCryptFS {
public:
    CrossCryptFS(std::string wrappedFolder);
    ~CrossCryptFS();
    CrossCryptFS(const CrossCryptFS&) = delete;
    CrossCryptFS(CrossCryptFS&&) = delete;
    CrossCryptFS& operator=(const CrossCryptFS&) = delete;
    CrossCryptFS& operator=(CrossCryptFS&&) = delete;

    void setEncryptionProvider(std::shared_ptr<EncryptionProvider> provider);

    void importFile(std::string fromFileFull, std::string toFileRelative);
    void exportFile(std::string toFileFull, std::string fromFileRelative);

    void list(std::vector<FileEntry>& addTo, std::string relativeFolderName);

private:
    class Impl;
    std::unique_ptr<Impl> mImpl;
};


} // end of namespace

#endif