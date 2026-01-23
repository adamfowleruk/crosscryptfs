# TODO work list

## Current status

As of 23 Jan 2026, the tests all execute and succeed. Some code is commented out in ecies.cpp. This looks like why crosscryptfscli doesn't copy content, as the encryptContent function is effectively a no-op. The equivalent decryptContent function is also not implemented yet.

Looks like the unencrypted FS was working on the CLI, but plugging in encryption has not been completed yet.

Several warnings during building about deprecated openssl V3 API being used which need to be addressed ASAP.

## Priority TODO items

- [ ] Replace deprecated function calls - See https://docs.openssl.org/3.0/man7/migration_guide/#low-level-apis
- [ ] Shell out tests for encryptContent and decryypt Content
- [ ] Add test for end to end file copy
- [ ] Update crosscryptfscli so this works too
- [ ] Ensure all functions in crosscryptfscli have tests
- [ ] Rename crosscryptfscli to crosscrypt or something shorter (not cc though obvs)
- [ ] Implement file name encryption
- [ ] Add novice mode 'COPY THIS ENCRYPTED DATA FOLDER TO DISC' and 'TRANSMIT THIS TO THE RECEIVER SECURELY VIA ANOTHER METHOD' folders, or similar
- [ ] Add CLI library, with help and instructions
- [ ] Move project to Bilberry Software org
- [ ] Build doxygen or mkdocs website for the project and put live
- [ ] Multi-platform testing
  - [ ] Ubuntu WSL
  - [ ] Linux native on ext4 and other common FS (TBD)
  - [ ] Windows 11 x64 on NTFS and FAT32
  - [ ] Raspberry Pi 4 and 5 and Raspberry Pi OS
  - [ ] Mac Mini M1 on common FS (TBD)
  - [ ] HPC cluster testing
- [ ] Do a V1 release
- [ ] Get people to test and eval V1

## Future TODO items

- [ ] Milestone: Create TrueCrypt/VeraCrypt like encrypted file volume support
  - [ ] Add Windows file mount support
  - [ ] Add Linux file mount support (FUSE)
  - [ ] Add Mac file mount support
- [ ] Milestone: Block FileSystem support (Commercial only?)
  - [ ] Add Windows file mount support
  - [ ] Add Linux file mount support (FUSE)
  - [ ] Add Mac file mount support
