//  Copyright 2023 crosscryptfs Project Contributors
//  SPDX-License-Identifier: Apache-2.0
//

#include "context.h"

#include <vector>
#include <string>
#include <istream>
#include <fstream>
#include <iostream>
#include <filesystem>

namespace crosscryptfs {

void
write_context_stream(const std::string contextFolder,const std::string module,const std::string parameter,uint8_t* publicKey,size_t publicKeyLength)
{
    namespace fs = std::filesystem;
    std::string ccfs = contextFolder + "/.crosscryptfs"; // TODO cross platform
    if (!fs::exists(ccfs)) {
        fs::create_directories(ccfs);
    }
    std::string filename = ccfs + "/" + module + "." + parameter; // TODO cross platform
    
    std::ofstream outFile(filename);

    for (size_t i = 0;i < publicKeyLength;++i) {
        outFile << publicKey[i];
    }

    outFile.close();
}

void
read_context_stream(const std::string contextFolder,const std::string module,const std::string parameter,uint8_t** publicKeyOut,size_t* publicKeyLengthOut)
{
    namespace fs = std::filesystem;
    std::string ccfs = contextFolder + "/.crosscryptfs"; // TODO cross platform
    if (!fs::exists(ccfs)) {
        fs::create_directories(ccfs);
    }
    std::string filename = ccfs + "/" + module + "." + parameter; // TODO cross platform

    std::ifstream fin(filename, std::ifstream::binary);
    std::vector<char> buffer (1024,0);

    *publicKeyLengthOut = 0;
    // WARNING We're always assuming it's less than 1024 here
    if(fin.good()) {
        fin.read(buffer.data(), buffer.size());
        std::streamsize s = fin.gcount();
        *publicKeyOut = (uint8_t*)malloc(s * sizeof(std::uint8_t));
        for (size_t i = 0;i < s;++i) {
            *(*publicKeyOut + i) = (std::uint8_t)buffer[i];
        }
        *publicKeyLengthOut += s;
    }
}

} // end namespace
