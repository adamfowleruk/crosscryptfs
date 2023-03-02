# crosscryptcli

Currently this CLI only accepts the parameters for the keyfile. Generate
these by running the below:-

```sh
openssl ecparam -name prime256v1 -noout -genkey -conv_form uncompressed -outform DER -out ecc_key.der
```

Then invoke crosscryptcli using something like:-

```sh
crosscryptcli ./ecc_key.der
```

