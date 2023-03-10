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

/*!
 * \brief File types supported for import/export in CrossCryptFS
 */
enum class FileType {
    /*! \brief A real file (not sym link) */
    file,
    /*! \brief A directory */
    folder
};

/*!
 * \brief Represents a File or Folder stored within a CrossCryptFS managed Volume.
 */
struct FileEntry {
    /*! \brief Creates a new description of a file or folder within a CrossCryptFS Volume */
    FileEntry(std::string relativeOnStorage,std::string name,FileType type) noexcept;
    /*! \brief Default destructor */
    ~FileEntry() noexcept = default;
    /*! \brief Copy Constructor */
    FileEntry(const FileEntry& copyFrom) noexcept = default;
    /*! \brief Move Constructor */
    FileEntry(FileEntry&& moveFrom) noexcept = default;
    /*! \brief Copy Assignment */
    FileEntry& operator=(const FileEntry&) noexcept = default;
    /*! \brief Move Assignment */
    FileEntry& operator=(FileEntry&&) noexcept = default;

    /*! \brief Path within the Volume including encrypted file name (the 'relative file name' used in API calls) */
    std::string relativePathOnStorage;
    /*! \brief The unencrypted file name without path (for display purposes) */
    std::string name;
    /*! \brief The type of file (File or Folder) */
    FileType type;
};

} // end namespace

#endif