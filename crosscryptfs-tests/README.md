# crosscryptfs-tests

These are Catch2 v3.0 standalone tests.

## Setup

You need to run the following command before you run the tests:-

```sh
./genkeys.sh
```

You can now build and execute the tests.

The tests binary can be found at `../build/crosscryptfs-tests/crosscryptfs-tests`.

## TODO items

Start simple - no encryption

- FileSystemVolume support for outer and inner layers (unencrypted)
- FileImageVolume support for keyfile and settings information (unencrypted)
- Initialise command that creates these files in-situ
- Import and export commands on the CLI that use this keyfile information (unencrypted)
- Then work on 'inner layer' encryption, as the extensive below list
- Then work on 'outer layer' encryption where the keyfile (encrypted or not) is in a metadata folder, or streamed into the CLI
- Add password protection to the keyfile independent of the external decryption mechanism (applied after decryption of the volume)

- Initialise local command that will generate both private keys and public keys, and set up a home and an encrypted metadata folder.
  - ~/.crosskeysfs/keystore
- Initialise for recipient that takes a receiver public key but generates (if required) the local tx private key.
- Turn CLI into real CLI with 'init' as a command, and --ecies-rxpubkey-der as an optional option

- Add 'encrypt' command to CLI which replicates the current main executable but AFTER init has happened
- Add dummy src data folder to tests folder
- Test noencrypt function and source/destination tests
- Test ecies encryption function for src/dest
- Add actual ECIES encryption
- Ensure we're now using OpenSSL v3 primitives and not earlier lower level functions

- Write gen perf data scripts to generate some large files and many small files, and import each independently then together to calculate throughput

- Ensure binaries are statically binding openssl v3 and can be ran standalone

RELEASE 0.1 HERE FOR MAC ONLY

## RELEASE 0.2 TASKS

- Try importing a large folder onto the SSD

- Add import checkpoint process tracking and --resume option to cli import
- Add --maxattempts=10 and --attempt-delay=3 options
- Do the same for export
- Add report command that checks the current state of import/export
- Track the number of exported/imported files and add this to status report
- Add GCM tag validation checks to export function

RELEASE 0.2 HERE FOR MAC ONLY

## RELEASE 0.3 TASKS

- Try this myself over the network from my Pictures (or similar) folder using noencryption to my storage drive
- Now do the same using ECIES encryption (-scheme=ecies (default) and -scheme=none options)
- Any performance tweaks as necessary

- Compile and test on Linux Manjaro amd64
- Compile and test on Windows 10 amd64

RELEASE VERSION 0.3 FOR ALL PLATFORMS

Ask for others to review and try out (including at University sending/receiving institutions)

## RELEASE 0.4 TASKS

- Create library distribution as a shared library too, not just the CLI
- Document library use and best practices
- Document format versus software release version numbers

## PRE V1 TASKS

- Ask for formal security validation, and find funding for this
- Ask for University sign-off from at least 2 institutions

## Potential future considerations

- Producer/consume pattern of file system walking and file encryption (single threaded in v1)
- CLI options to generate receiver keys with explanation of which to send to others
- GUI for Mac to link in with Apple's KeyChain mechanism for the Outer Layer and expose as a drive
- GUI for Windows to act as a virtual local drive (like SugarSync used to allow) with drive letter
- Use as a replacement for eCryptFS in Ubuntu for home folder encryption