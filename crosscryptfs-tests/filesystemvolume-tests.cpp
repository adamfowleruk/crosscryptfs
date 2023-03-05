//  Copyright 2023 crosscryptfs Project Contributors
//  SPDX-License-Identifier: Apache-2.0
//

#include <catch2/catch_test_macros.hpp>
#include "crosscryptfs/crosscryptfs.h"
#include <filesystem>
#include <string>
#include <sstream>

TEST_CASE("I can create, use, and destroy a FileSystemVolume", "[volume][filesystem]") {

    // Init: Check that folder does not exist, or delete it if it does
    std::string wrappedFolder("filesystemvolume");

    namespace fs = std::filesystem;
    if (fs::exists(wrappedFolder)) {
        std::error_code ec;
        std::uintmax_t numberRemoved = fs::remove_all(wrappedFolder, ec);
    }

    // Create a new File System Volume
    crosscryptfs::FileSystemVolumeProvider volume(wrappedFolder);

    // Add multiple files
    std::string filename(".crosscryptfs/keyfile");
    std::string content;
    int iterations = 10;
    std::string c("somecontent");
    for (int i = 0;i < iterations;++i) {
        content += c;
    }
    std::istringstream input{content};
    crosscryptfs::BasicStringStreamBytesProvider bytes = std::make_shared<crosscryptfs::IStreamBytesProvider<std::istringstream>>(input);
    volume.add(filename, bytes, content.length());

    // Ensure all is as expected
    REQUIRE(fs::exists(wrappedFolder));
    // Verify the list of root folders is one
    // Verify this folder has one file

    // Add additional files
    std::string filename2(".crosscryptfs/settings");
    std::string content2 = "FSVERSION=1.0.0";
    std::istringstream input2{content2};
    crosscryptfs::BasicStringStreamBytesProvider bytes2 = std::make_shared<crosscryptfs::IStreamBytesProvider<std::istringstream>>(input2);
    volume.add(filename2, bytes2, content2.length());

    // Verify the list of root folders is one
    // Verify this folder has two files

    // Verify each of the two files exist by name
    // Verify they are the correct size
    // Verify they have the correct content

    // Delete the first one
    // Verify the list of root folders is one
    // Verify this folder has one file
    // Verify the one file exists by name
    // Verify it has the correct size
    // Verify it has the correct content

    // Request its destruction
    volume.destroy();

    // Verify it no longer exists
    REQUIRE(fs::exists(wrappedFolder) == false);
}
