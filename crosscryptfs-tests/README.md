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

- Try importing a large folder onto the SSD

- Add import checkpoint process tracking and --resume option to cli import
- Add --maxattempts=10 and --attempt-delay=3 options
- Do the same for export
- Add report command that checks the current state of import/export
- Track the number of exported/imported files and add this to status report
- Add GCM tag validation checks to export function

- Try this myself over the network from my Pictures (or similar) folder using noencryption to my storage drive
- Now do the same using ECIES encryption (-scheme=ecies (default) and -scheme=none options)

- Add sym key password protection

- Ensure binaries are statically binding openssl v3 and can be ran standalone

- Compile and test on Linux Manjaro amd64
- Compile and test on Windows 10 amd64

- RELEASE VERSION 1.0