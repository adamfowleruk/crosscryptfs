//  Copyright 2023 crosscryptfs Project Contributors
//  SPDX-License-Identifier: Apache-2.0
//
#include "crosscryptfs.h"
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <iostream>

using namespace crosscryptfs;

FileEntry::FileEntry(std::string relativeOnStorage,std::string name,FileType type)
    : relativePathOnStorage(relativeOnStorage),
      name(name),
      type(type)
{
    ;
}






class CrossCryptFS::Impl {
public:
    Impl(std::string wrappedFolder);
    ~Impl();

    std::string m_wrapped;
};

CrossCryptFS::Impl::Impl(std::string wrappedFolder)
    : m_wrapped(wrappedFolder)
{
    ;
}

CrossCryptFS::Impl::~Impl() {}


// Class method implementations

CrossCryptFS::CrossCryptFS(std::string wrappedFolder)
  : mImpl(std::make_unique<CrossCryptFS::Impl>(wrappedFolder))
{
    namespace fs = std::filesystem;
    if (!fs::exists(wrappedFolder)) {
        fs::create_directories(wrappedFolder);
    }
}

CrossCryptFS::~CrossCryptFS() {}


void
CrossCryptFS::importFile(std::string fromFileFull, std::string toFileRelative)
{
    namespace fs = std::filesystem;
    // TODO check settings to see if we encrypt or just copy raw (as below)
    std::ifstream src(fromFileFull);
    fs::path dest(mImpl->m_wrapped);
    dest /= toFileRelative;
    std::ofstream internalFile(dest);
    std::string line;
    if (src && internalFile) {
        while (std::getline(src, line)) {
            internalFile << line << "\n";
        }
    }
    src.close();
    internalFile.close();
}

void
CrossCryptFS::exportFile(std::string toFileFull, std::string fromFileRelative)
{
    namespace fs = std::filesystem;
    // TODO check settings to see if we encrypt or just copy raw (as below)
    std::ofstream destFile(toFileFull);
    fs::path internalPath(mImpl->m_wrapped);
    internalPath /= fromFileRelative;
    std::ifstream internalFile(internalPath);
    std::string line;
    if (internalFile && destFile) {
        while (std::getline(internalFile, line)) {
            destFile << line << "\n";
        }
    }
    internalFile.close();
    destFile.close();
}

void
CrossCryptFS::list(std::vector<FileEntry>& addTo, std::string relativeFolderName)
{
    // TODO use relativeFolderName rather than assuming the root folder
    namespace fs = std::filesystem;
    fs::path fp(mImpl->m_wrapped);
    for (auto& p : fs::directory_iterator(fp)) {
        if (p.exists() && p.is_regular_file()) {
            addTo.emplace_back(p.path().filename(),p.path().filename(),FileType::file);
        }
    }       
}
