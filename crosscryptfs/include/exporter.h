//  Copyright 2023 crosscryptfs Project Contributors
//  SPDX-License-Identifier: Apache-2.0
//

#ifndef EXPORTER_H
#define EXPORTER_H

#include "crosscryptfs.h"
#include <string>

namespace crosscryptfs {

class ExportOperation {
public:
    ExportOperation(CrossCryptFS& fs, std::string sourceFolder);
    ~ExportOperation();

    void runToCompletion();
private:
    class Impl;
    std::unique_ptr<Impl> mImpl;
};

} // end of namespace

#endif