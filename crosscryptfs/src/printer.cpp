//  Copyright 2023 crosscryptfs Project Contributors
//  SPDX-License-Identifier: Apache-2.0
//
#include "printer.h"
#include "crosscryptfs.h"
#include <string>

#include <iostream>

using namespace crosscryptfs;

class MetadataPrinter::Impl {
public:
    Impl();
    ~Impl();
};

MetadataPrinter::Impl::Impl() {}
MetadataPrinter::Impl::~Impl() {}


// Class method implementations

MetadataPrinter::MetadataPrinter()
  : mImpl(std::make_unique<MetadataPrinter::Impl>())
{
    ;
}

MetadataPrinter::~MetadataPrinter() {}

void
MetadataPrinter::print(CrossCryptFS& fs)
{
    std::vector<FileEntry> contents;
    fs.list(contents,"");
    std::cout << "CrossCryptFS contents:-" << std::endl;
    for (auto& el: contents) {
        std::cout << "\t" << (el.type == FileType::file ? "File" : "Folder") << ": "
                  << el.name << " (" << el.relativePathOnStorage << ")" << std::endl;
    }
}