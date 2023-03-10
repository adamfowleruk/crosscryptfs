//  Copyright 2023 crosscryptfs Project Contributors
//  SPDX-License-Identifier: Apache-2.0
//

#ifndef EXPORTER_H
#define EXPORTER_H

#include "crosscryptfs.h"
#include <string>

namespace crosscryptfs {

/*!
 * \brief Represents a resumable, error tolerant, file export operation
 */
class ExportOperation {
public:
    ExportOperation(CrossCryptFS& fs, std::string sourceFolder);
    ~ExportOperation();
    ExportOperation(const ExportOperation&) = delete;
    ExportOperation(ExportOperation&&) = delete;
    ExportOperation& operator=(const ExportOperation&) = delete;
    ExportOperation& operator=(ExportOperation&&) = delete;

    void runToCompletion();
private:
    class Impl;
    std::unique_ptr<Impl> mImpl;
};

} // end of namespace

#endif