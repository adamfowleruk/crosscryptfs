//  Copyright 2023 crosscryptfs Project Contributors
//  SPDX-License-Identifier: Apache-2.0
//

/*
 * The main executable of the crosscryptfs CLI
 */
#include <iostream>
#include "crosscryptfs/crosscryptfs.h"

using namespace crosscryptfs;

int main(int argc, char* argv[]) {
  // For now just encrypt and decrypt a folder

  // Define our wrapped managed storage folder
  CrossCryptFS encfs(".crosscryptfs");
  MetadataPrinter printer;
  printer.print(encfs);

  // Import / encrypt a source folder
  ImportOperation importer(encfs, "~/Documents/SomeSourceFolder");
  importer.runToCompletion();
  printer.print(encfs);

  // Now export to a destination folder
  ExportOperation exporter(encfs, "~/Documents/SomeTargetFolder");
  exporter.runToCompletion();
  printer.print(encfs);

  // Return success code
  return 0;
}
