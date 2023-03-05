# C++ classes within CrossCryptFS

This document describes the high level C++ convenience classes provided for
developers to create application on. Lower level C functions will provide
much of the basic functionality. These classes allow for an easier mental
model.

The basic class list is as follows:-

- EncryptionProvider - Provides encryptContent and decryptContent functions
  - Is passed a set of settings (which usually come from settings files)
- VolumeProvider - Represents the outer layer of a CrossCryptFS managed volume.
  - This may be encrypted, and so each VolumeProvider will have one 
  EncryptionProvider associated with it.
  - Includes access to the outer settings files and encrypted inner keyfile
- CrossCryptFS - Represents the inner file system post keyfile decryption
  - Hosts the main file system-wide operations such as import, export, and 
  reencrypt
  - Has an associated EncryptionProvider for encrypting filename and content
  - This is where file level encryption is performed
  - This is also where large file splitting and small file joining and 
  compression is performed

## EncryptionProvider

Provides encryption of arbitrary byte sequences. All EncryptionProviders can be
used for outer Volume and inner CrossCryptFS encryption.

Provided schemes include:-

- NoEncryption - Just a representation of files unencrypted. Useful for just
moving files around or for testing
- ECIES - Provides ECIES encryption
  - Uses a P256 curve with ECDH for the KA, X9.63 SHA256 as the KDF, and
  AES-GCM for encryption

## VolumeProvider

Provides access to a list of files and basic information such as their name,
folder, and file size.

Provided schemes include:-

- FileSystem - Uses a system provided file system to list and query files
  - Used for the default volume to access file level encryption in CrossCryptFS
- EncryptedFile - Uses a single file whose contents are encoded in a very 
simple format to represent a whole file system
  - Used for the default Keyfile format in CrossCryptFS

Currently CrossCryptFS is implemented as a pseudo file system layer on top of
an existing file system like ExFAT to provide file-level encryption. In future
CrossCryptFS may provide file system and block storage device drivers, in which
case the following VolumeProviders will also be supported:-

- BlockDevice - Wraps an OS low-level disc device
  - Read/writes raw blocks to represent the outer full disc encryption wrapper
  - All key material held on a hardware TPM managed by the BIOS
  - The BIOS could use this layer to mount a system disc drive
  - An OS could instead use this layer to mount a non-bootable disc volume
  - Would effectively provide a 'file' per partition within the block device
- BlockFileSystem - Provides the inner file system wrapper within a BlockDevice
  - Would provide FileSystem level encryption via an OS one layer above the 
  hardware

In the above schemes we would in essence provide:-

- No encryption, just encapsulation, via the FileSystem provider
- File level encryption via the FileSystem provider
  - Akin to ExtFS
  - Requires an existing OS supported File System such as ext4, ExFAT, etc.
- Single-file volume encyption via the EncryptedFile provider
  - Akin to TrueCrypt and VeraCrypt
- File system encryption via BlockFileSystem
  - Akin to eCryptFS
- Full disc encryption via BlockDevice
  - Akin to BitLocker

Given that each layer in the above list is more complex to design software for
and potentially more destructive if implemented incorrectly, we're approaching
CrossCryptFS from the top (no encryption) down toward full disk encryption
over time.