#!/bin/sh

echo "Generating 'external' EC private keys for transmitter and receiver/recovery"

mkdir -p ../build/crosscryptlib-tests
cd ../build/crosscryptlib-tests

openssl ecparam -name prime256v1 -noout -genkey -conv_form uncompressed -outform DER -out ecc_tx_private_key.der
openssl ecparam -name prime256v1 -noout -genkey -conv_form uncompressed -outform DER -out ecc_rx_private_key.der