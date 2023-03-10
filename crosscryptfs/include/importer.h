//  Copyright 2023 crosscryptfs Project Contributors
//  SPDX-License-Identifier: Apache-2.0
//

#ifndef IMPORTER_H
#define IMPORTER_H

#include "crosscryptfs.h"
#include <string>

namespace crosscryptfs {

/*!
 * \brief Represents a resumable, error tolerant, file import operation
 */
class ImportOperation {
public:
    ImportOperation(CrossCryptFS& fs, std::string targetFolder);
    ~ImportOperation();
    ImportOperation(const ImportOperation&) = delete;
    ImportOperation(ImportOperation&&) = delete;
    ImportOperation& operator=(const ImportOperation&) = delete;
    ImportOperation& operator=(ImportOperation&&) = delete;

    void runToCompletion();
private:
    class Impl;
    std::unique_ptr<Impl> mImpl;
};

} // end of namespace

#endif