//  Copyright 2023 crosscryptfs Project Contributors
//  SPDX-License-Identifier: Apache-2.0
//

#ifndef CROSSCRYPTFS_H
#define CROSSCRYPTFS_H

#include "context.h"
#include "encryption.h"
#include <string>
#include <vector>

namespace crosscryptfs {

// Low-level C style functions first






// Now C++ wrappers

enum class FileType {
    file,
    folder
};

struct FileEntry {
    FileEntry(std::string relativeOnStorage,std::string name,FileType type);
    ~FileEntry() = default;
    std::string relativePathOnStorage; // path including encrypted file name (the 'relative file name' used in API calls)
    std::string name; // the unencrypted file name without path 9for display purposes)
    FileType type;
};

class CrossCryptFS {
public:
    CrossCryptFS(std::string wrappedFolder);
    ~CrossCryptFS();

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