# crosscryptfs development guide

## Install prereqs

The below instructions are for Ubuntu.

First install apt packages:-

```sh
sudo apt install build-essential cmake libssl-dev doxygen
```

Now install the Catch2 testing framework:-

```sh
git clone https://github.com/catchorg/Catch2.git
cd Catch2
cmake -B build -S . -DBUILD_TESTING=OFF
sudo cmake --build build/ --target install
```

## Building CrossCryptFS

Now finally build the library:-

```sh
git clone https://github.com/adamfowleruk/crosscryptfs.git
cd crosscryptfs
cmake -B build -S .
cmake --build build
```

## Execute apps

There are various apps compiled under subfolders of the `build` folder:-

- crosscryptfs library itself
- crosscryptfs-tests - run the `crosscryptfs-tests` app in this subfolder first as a test suite
- crosscryptfscli - command line interface. See the README.md file in its subfolder for usage
