//  Copyright 2023 crosscryptfs Project Contributors
//  SPDX-License-Identifier: Apache-2.0
//

#include <catch2/catch_test_macros.hpp>
#include "crosscryptfs/crosscryptfs.h"
#include <filesystem>

// TEST_CASE("I can import a folder to a FileSystemVolume using an importer", "[import][volume][filesystemvolume][filesystem]") {
//     std::string wrappedFolder("imported");
//     namespace fs = std::filesystem;
//     if (fs::exists(wrappedFolder)) {
//         std::error_code ec;
//         std::uintmax_t numberRemoved = fs::remove_all(wrappedFolder, ec);
//     }

//     // Create a new File System Volume
//     crosscryptfs::FileSystemVolumeProvider volume(wrappedFolder);

//     // Create the import operation
//     crosscryptfs::CrossCryptFS ccfs(volume);
//     crosscryptfs::ImportOperation importer(ccfs,"../../crosscryptfs-tests/data/sourcefolder");

//     // Execute the import operation until completion
//     importer.runToCompletion();

//     // Check the contents of the volume
//     REQUIRE(fs::exists(wrappedFolder));
//     std::vector<crosscryptfs::FileEntry> firstFolders;
//     // Verify the list of root folders is one
//     volume.list(firstFolders,"");
//     REQUIRE(firstFolders.size() == 1);
//     REQUIRE(firstFolders.at(0).name == "sometext.txt");

//     // Request its destruction
//     volume.destroy();

//     // Verify it no longer exists
//     REQUIRE(fs::exists(wrappedFolder) == false);
// }
