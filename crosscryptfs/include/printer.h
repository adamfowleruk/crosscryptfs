//  Copyright 2023 crosscryptfs Project Contributors
//  SPDX-License-Identifier: Apache-2.0
//

#ifndef PRINTER_H
#define PRINTER_H

#include "crosscryptfs.h"

namespace crosscryptfs {

class MetadataPrinter {
public:
    MetadataPrinter();
    ~MetadataPrinter();

    void print(CrossCryptFS& fs);
private:
    class Impl;
    std::unique_ptr<Impl> mImpl;
};

} // end of namespace

#endif