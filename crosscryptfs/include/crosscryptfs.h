//  Copyright 2023 crosscryptfs Project Contributors
//  SPDX-License-Identifier: Apache-2.0
//

#ifndef CROSSCRYPTFS_H
#define CROSSCRYPTFS_H

#include <string>

namespace crosscryptfs {

class CrossCryptFS {
public:
    CrossCryptFS(std::string wrappedFolder);
    ~CrossCryptFS();
private:
    class Impl;
    std::unique_ptr<Impl> mImpl;
};

} // end of namespace

#endif