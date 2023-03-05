#!/bin/sh

echo "Generating 'external' EC private keys for transmitter and receiver/recovery"

mkdir -p ../build/crosscryptfs-tests
cd ../build/crosscryptfs-tests

openssl ecparam -name prime256v1 -noout -genkey -conv_form uncompressed -outform DER -out ecc_tx_private_key.der
openssl ecparam -name prime256v1 -noout -genkey -conv_form uncompressed -outform DER -out ecc_rx_private_key.der