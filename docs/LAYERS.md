# Inner and Outer CrossCryptFS layers

This document aims to describe the two layers of any protected data in 
CrossCryptFS:-

- Outer Layer consisting of key material used to access the inner layer, but 
which is encrypted using information external to CrossCryptFS (E.g. a KEK held
and controlled in a TPM).
- Inner Layer which uses decrypted settings and key material to encrypt, 
decrypt, and operate upon the actual stored file data and metadata.

We shall first describe the inner layer as that explains why an outer layer is
required.

## Inner Layer

This consists of the encrypted data held within a CrossCryptFS managed store,
alongside some CrossCryptFS managed settings and key material used to encrypt
and decrypt the data and metadata.

In CrossCryptFS file orientated and file system modes, the inner layer data is
a set of files with associated metadata which are also encrypted, alongside
per-file or file-metadata CrossCryptFS validation and management metadata.

In CrossCryptFS block or volume orientated modes, the inner layer data is a set
of fixed sized blocks which are encrypted, alongside per-block CrossCryptFS 
validation and management metadata.

In order to manage the inner layer, CrossCryptFS inner layers need to manage
the following data:-

(The below descriptions mention inner 'files'. They should be taken to also
refer to 'blocks' when the inner layer is a block orientated device. File
is used as an easy to comprehend concept.)

- The plaintext filename and file contents for each file
- An authentication code for each file to detect corruption or deliberate
manipulation
- Where encryption is employed, a per-file IV (where the IV can be
specified independent of the algorithm) or AAD (where it cannot)
- The 'keyfile' - Data used by the chosen encryption mechanism to encrypt and
decrypt the inner layer. This varies per encryption provider, but can be
thought of in the general case as a symmetric key.

For a 'NoEncryption' file orientated store, the file contents are the same as
the source file with no overhead. For an ECIES X9.63 SHA256 AES-GCM encrypted
store, each file has the overhead of AAD (optional) and the GCM tag. The
public key is held within a settings file and is shared amongst all files
encrypted with the same receiver/recovery public key rather than being
included in the ECIES cryptogram as is the normal case. A GCM tag is 16 bytes.

(Note: This is why CrossCryptFS aims to introduce transparent 'small file' 
support in order to minimise such overhead for small files. E.g. those smaller
than 160 bytes. Any such CrossCryptFS per-file data will be held at the start
of each file in a 'header' which is never passed to the EncryptionProvider).

Note that in the case that ECIES is used on the inner layer, a certain amount
of minimum information will need to be available to encrypt new files or modify
files. These are below:-

- The 'transmitters' (or data owners) EC ephemeral private key
  - held in `ecies.txprivatekey` by default
- The 'receivers' (or recovery key holders) EC ephemeral public key
  - held in `ecies.rxpubliekey` by default
- AAD used for file content and metadata
  - held in the `NAMEHEADER` setting in the `settings` file by default

## The rationale for an outer layer

The above three pieces of information must be kept secure. Whilst it can be
argued that such information belongs in a TPM, there are advantages to encoding
key material separately and having an inner and outer layer:-

- The outer public and private keys used by transfer participants can be
changed without the data needing to be decrypted and reencrypted
  - This allows for simple key rotation on physical hardware
  - Very useful for when drives or files are moved between systems or where
  a motherboard with a TPM chip has failed - the recipient public key can be 
  used to recover the CrossCryptFS Inner Layer, effectively becoming a
  recovery key
- The inner layer can make use of state of the art or emerging encryption
mechanisms that the original hardware manufacturer did not anticipate
  - Useful to stay up to date with encryption standards
  - Also useful to address any discovered weaknesses in encryption standards
  after hardware is released
  - Allows the outer layer to specify multiple inner layers. This is useful
  because many encryption standards recommend that data beyond a certain size
  is not encrypted with the same key, and this for AES-GCM is only around
  62 GiB. When we're dealing with 8TB of human genome sequence data, this
  starts to become a real concern.
- Transferring data between secured high performance compute sites
  - HPC may use raw disc volumes to achieve required computational performance
  processing rates
  - These sites are typically therefore secured very well with physical
  security and limited, or absent, network connectivity
  - Large network file transfers to a single machine's block device for 
  transport is time consuming and error prone. Having a mechanism that can
  restart a transfer at the file that failed is useful. Potentially it's also 
  useful to extract part of the shared files on each cluster machine as an
  individual inner layer, then simply copy the final inner layers to a fast
  local external disc over, for example, Thunderbolt 3. This simple local file
  copy is much quicker than encryption and copy over the network.
- Allows the CrossCryptFS CLI to be used in user-land and transported with data
  - Some systems do not support TPM or the latest encryption algorithms
  - Some users do not have permission to mount drives, or use particular Linux
  FUSE file system binaries
  - Some users do not have permission to install new applications on systems
  - Having the CLI include all binaries (such as OpenSSLv3) to perform all
  operations on the inner file format allows for maximum flexibility in both
  the transmitters and receives IT systems whilst still enabling secure data
  transfer between institutions - a key aim of the CrossCryptFS project.

## Outer Layer

For simplicity, the outer layer is represented as a single encrypted
file and a metadata settings file to describe it. (Although if the outer layer
settings are known to both parties, this would not need to be stored.) 
This single file is effectively a 'volume' that contains the secured key 
material for the inner layer only.

Assuming that a separate ECIES round is used on the inner layer, entirely under
the management of CrossCryptFS, this single file volume would contain the 
inner layer's settings file, txprivatekey and rxpublickey, and optionally the
rxprivatekey too (although this should also be kept elsewhere too to allow for
file recovery).

The location of the outer layer's public and private key material to encrypt or
decrypt the inner layer's keyfile volume is out of scope to CrossCryptFS.
The CrossCryptFS CLI will have command line options to stream these settings
into its invocation, to look in well-known areas such as a home drive folder
or Apple's KeyChain, or via command line prompting as a fall back.

(Note: Any future device driver for CrossCryptFS would support the same
settings and keydata streaming mechanism allowing BIOS or an OS to use a
CrossCryptFS volume as the primary boot disc/partition).

This format is the `EncryptedFile` Volume Provider mentioned in the
[C++ Classes](CLASSES.md) file.