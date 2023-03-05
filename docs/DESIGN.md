# crosscryptfs Design

The design of CrossCryptFS is inspired by the 
[design document of EncFS](https://github.com/vgough/encfs/blob/master/DESIGN.md).

No code is shared between the projects, but I liked the simplicity of 
performing file level encryption as it could be layed on top of existing file
systems like ExFAT, and so could allow a tool in user-land without kernel
level privileges to be used.

## High level design

We shall aim in our first version to provide an encryption layer in user space
rather than an encrypted native disc file system. We can add this option in
future once the principle is proven. We shall layer this as a folder within
an ExFAT file system initially.

A single symmetric encryption/decryption key is used for each volume of the 
file system. This key is used to encrypt both file (and folder) names, and file
data itself.

The key itself is encrypted using a key mechanism held off disk. This allows an
external KEK/TEK to be used to manage this external key. It also allows the 
encryption key to be changed without having to decrypt and re-encrypt the whole
drive - just simply replace the encrypted key file.

As the root of the volume itself may have different settings such as for file
splitting, reconstitution, and the internal encryption mechanism used, a 
settings folder at the root of each volume called .crosscryptfs shall be 
created. This folder has the following contents:-

- keyfile - The Encrypted keyfile for this volume
- settings - a "KEY=VALUE" set of settings for CrossCryptFS - a simple format 
to minimise dependencies

Other files may exist within this folder depending on usage needs. If files
exist for a particular encryption algorithm or base file system (E.g. ExFAT)
then they should be named lowercase with no delimeters for that need to 
prevent inadvertant name clashes.

## Implementation details

The rest of this document describes high level features. If you wish to see
details on a specific implementation then please see the
[C++ Classes](CLASSES.md) file.

## Settings file

The settings file shall specify the method and strength used to encrypt 
information on the drive. It shall not contain information on the method and
strength used to encrypt the keyfile itself - this should be passed to us
from an external settings mechanism, perhaps protected on a hardware TPM.

As a minimum the file shall have the following settings:-

- FSVERSION=1.0.0 - The crosscrypt File System format version, which may not 
follow the same versioning semantics as this project
- ENCRYPTION=ECIESX963SHA256AESGCM - The encryption mechanism in use on the file 
system
- NAMEHEADER=base64(BINARY) - The IV, and perhaps other data, used for file
and folder name encryption. Acts as the 'header' for folder names.

*UNSUPPORTED TODAY* 
Other metadata could be included in future, and custom metadata is supported,
allowing users of CrossCryptFS to include additional storage metadata such as
the data owner, lost and found contact details, and so on.

Some optional settings we may choose to support include:-

- FILESYSTEM=ExFAT - The filesystem used on the drive, if not obtainable from
system calls
- BLOCKSIZE=1024 - Block size in bytes in use, if not obtainable from system
calls (some discs may prefer 512 or 2048 for block size)
- DISCTECH=SSD - The physical storage media in use
- MAXFILESIZE=2000000 - Maximum individual target encrypted file size in bytes.
After this files will either be split, or the operation will fail
- SPLITFILES=false - Splitting large files is not supported yet
- MINFILESIZE=0 - File size below which to merge multiple files in the same
folder and unmerge transparently
- COMPRESSION=None - The file compression mechanism to use, if any (E.g. gzip).

These may allow CrossCryptFS to provide better read and write speeds in some
scenarios. Where not provided, the above values shall be presumed.

## Keyfile

Separating out the encryption of the keyfile from the encryption of individual
files allows the outer encryption keys to be changed in a single operation
without the needs to decrypt and re-encrypt every file.

The settings file in the outer `.crosscryptfs` folder specifies enough settings
to allow crosscryptfs to decrypt the Keyfile only. This keyfile contains all
information necessary to decrypt the contents of the filesystem itself. This 
includes:-

- The `.crosscryptfs` settings file in `KEY=VALUE` format
- Any binary settings file for the chosen encryption mechanism 
(E.g. ecies.rxpublickey)

As an example, for an ECIES encryption mechanism where the information is held
purely in a non-TPM OpenSSL v3 ECIES scheme on the filesystem, the encrypted
keyfile would have the following contents:-

- Settings file
  - FSVERSION=1.0.0
  - ENCRYPTION=ECIESX963SHA256AESGCM
  - PROVIDER=OPENSSLV3
  - NAMEHEADER=base64(BINARY)
- ecies.rxpublickey
  - If the provider was not OpenSSLv3, E.g. Apple TPM, then this may instead
  be just a setting in the settings file with the name of the key in the TPM
- ecies.txprivatekey (which the txpublickey can be generated from)
  - If the provider was not OpenSSLv3, E.g. Apple TPM, then this may instead
  be just a setting in the settings file with the name of the key in the TPM

The format of this file shall consist of the following, for each file:-

- A uint64_t number for the length of the filename
- The filename in whatever format is supported (E.g. char string)
- A uint64_t number for the length of the file
- The files data
- Format repeats until all files are encoded

This shall be represented inside a CrossCryptFS instance as a virtual volume
to be operated upon just as if those files existed unencrypted on the 
containing disc. 

This is a simple lowest common denominator format that is easily to implement
on any platform. No permissions or file metadata are included because the 
files merely describe crosscryptfs settings, and thus if you have permission
to access and decrypt the keyfile then you can access all of these files.

Note that the format does not require the settings file to be the first file
in the encrypted volume, and nor does it specify the ordering of the keys
within the settings file. Ideally this should be unpredictable on each
encoding of the keyfile to prevent a crib being formed.

## Encrypted file format

The file shall consist of a header large enough to hold an IV for each file
that is encrypted. The size of this is determined by the encryption mechanism
setting but should be only as large as necessary to hold the IV so as to
minimise overhead for small files. Other data may also be held depending on
the encryption algorithm used. E.g. GCM check code.

After this header, the remainder of the file shall consist of the encrypted
bytes of the compressed contents of the file. If compression is not used, then
this shall just be an encryption of the raw file bytes.

*UNSUPPORTED TODAY*
If splitting is used, there will be FILENAME.part0, FILENAME.part1, and so on, 
files used. In this case, each file holds an IV of the file part in a header
followed by the encryption of the `MAXFILESIZE-HEADERSIZE` bytes for each part.
The final part shall be less than or equal to `MAXFILESIZE-HEADERSIZE` bytes in
size.

*UNSUPPORTED TODAY*
If merging of small files is used, the folder that contains merged files shall
include a `.merged.meta` file which shall have multiple lines of the format  
`base64<ENCFILENAME>=base64<enc<filename1>>:204,base64<enc<filename2>>:354,...`, 
where ENCFILENAME is the encrypted file name stored in the FS (and likely a
V4 UUID), and filename1, filename2 etc, are the original file names prior
to merging, and the number component is the size in bytes of the files
post-encryption content.

## Encrypted file and folder names

File and folder names are encrypted using the same encryption mechanism, but
using the IV (and perhaps other metadata) needed by the algorithm. This is held
in the mandatory `NAMEHEADER` setting in the settings file, encoded as base64.

## Encryption schemes supported

Below are the initial encryption schemes we will support. They can be extended
in future.

## ECIES with X9.63 SHA256 AES-GCM and a P256 curve

Elliptic Curve Integrated Encryption Scheme used with AES-GCM and a P256 curve
has become the new default in government security specifications. It is also
widely supported on mobile devices, laptops and servers.

This scheme is names "ECIESX963SHA256AESGCM" in the settings file.
On Mac OS X this schems is referred to as
`SecKeyAlgorithm.eciesEncryptionStandardX963SHA256AESGCM`.

The initial scheme we will support is as follows:-

- A shared secret value external to CrossCrypt but provided to our CLI
- An ephemeral public key generated by the volume creator and stored
unencrypted in `.crosscryptfs/ecies.publickey`
- The above three are used to generate the kENC and IV
- The CrossCrypt File IV shall be used as the 'Authentication Data' in this
scheme rather than the IV, as the IV comes from the last 16 bytes of the
X9.63 Key Derivation Function (KDF)
- AES-128-GCM is used as the symmetric encryption/decryption algorithm
- This results in an 'encrypted message' and a 'mac code' concatenated
together. This forms the 'encrypted file text' or 'encrypted file name'
in the result as used by CrossCryptFS for its operations

Platforms that support this mechanism:-

- Cross platform - OpenSSL v3 library
- iOS 10.7 and above
- Mac OS X native (with TPM) TBD
- Android TBD
- Windows TBD
- Linux (without TPM?) - via OpenSSL v3
- Mac (without TPM) - via OpenSSL v3