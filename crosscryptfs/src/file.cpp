//  Copyright 2023 crosscryptfs Project Contributors
//  SPDX-License-Identifier: Apache-2.0
//

#include "file.h"

#include <string>

namespace crosscryptfs {

FileEntry::FileEntry(std::string relativeOnStorage,std::string name,FileType type)
    : relativePathOnStorage(relativeOnStorage),
      name(name),
      type(type)
{
    ;
}

} // end namespace