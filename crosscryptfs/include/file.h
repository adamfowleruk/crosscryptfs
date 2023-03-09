//  Copyright 2023 crosscryptfs Project Contributors
//  SPDX-License-Identifier: Apache-2.0
//

#ifndef FILE_H
#define FILE_H

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
    FileEntry(const FileEntry& copyFrom) = default;
    FileEntry(FileEntry&& moveFrom) = default;
    FileEntry& operator=(const FileEntry&) = default;
    FileEntry& operator=(FileEntry&&) = default;

    std::string relativePathOnStorage; // path including encrypted file name (the 'relative file name' used in API calls)
    std::string name; // the unencrypted file name without path (for display purposes)
    FileType type;
};

} // end namespace

#endif