//  Copyright 2023 crosscryptfs Project Contributors
//  SPDX-License-Identifier: Apache-2.0
//

#ifndef CONTEXT_H
#define CONTEXT_H

#include "encryption.h"
#include <string>
#include <vector>

namespace crosscryptfs {

// Low-level C style functions first

/*! \brief Writes a module's parameter settings file */
void write_context_stream(const std::string contextFolder,const std::string module,const std::string parameter,uint8_t* publicKey,size_t publicKeyLength);

/*! \brief Reads a module's parameter settings file */
void read_context_stream(const std::string contextFolder,const std::string module,const std::string parameter,uint8_t** publicKeyOut,size_t* publicKeyLengthOut);

} // end namespace

#endif