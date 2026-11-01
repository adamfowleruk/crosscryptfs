# CrossCrypt CLI

The new CLI backed by C to replace the crosscryptfs C++ CLI (in the crosscryptfscli folder).

This is the future CLI doing forward. Shortened name reflects that it is used for all CrossCrypt CLI actions, not just those for the high level folder filesystem support.

## Running the C CLI

```sh
cd cmake-build-debug/crosscrypt
openssl ecparam -name prime256v1 -noout -genkey -conv_form uncompressed -outform DER -out ecc_key.der
```

Then invoke crosscrypt C cli using something like:-

```sh
cd cmake-build-debug/crosscrypt
./crosscrypt ./ecc_key.der
```

Note: It's crosscrypt.exe on Windows
